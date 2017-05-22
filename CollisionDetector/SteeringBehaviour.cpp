#include "pch.h"
#include "SteeringBehaviour.h"

using namespace SteeringBehaviours;
using namespace DirectX;
using namespace DirectX::SimpleMath;

SteeringBehaviour::SteeringBehaviour(Kinematic* character) : 
	m_character(character)
{
}

void SteeringBehaviour::SetCharacter(Kinematic* character)
{
	m_character = character;
}

Align::Align(Kinematic* character, Kinematic* target, const float& max_angular_acceleration, const float& max_rotation, const float& target_radius, const float& slow_radius, const float& time_to_target) :
	SteeringBehaviour(character),
	m_target(target),
	m_maxAngularAcceleration(max_angular_acceleration),
	m_maxRotation(max_rotation),
	m_targetRadius(target_radius),
	m_slowRadius(slow_radius),
	m_timeToTarget(time_to_target)
{
}

void Align::GetSteering(SteeringOutput* output)
{
	float targetRotation;
	float rotation = m_target->Orientation - m_character->Orientation;

	//Map the result to the (-pi, pi) interval
	rotation = mapToRange(rotation);

	float rotationSize = std::abs(rotation);
	if (rotationSize < m_targetRadius)
		return;
	if (rotationSize > m_slowRadius)
		targetRotation = m_maxRotation;
	else
		targetRotation = m_maxRotation * rotationSize / m_slowRadius;

	targetRotation *= rotation / rotationSize;
	output->Angular = targetRotation - m_character->Rotation;
	output->Angular /= m_timeToTarget;

	float angularAcceleration = std::abs(output->Angular);
	if (angularAcceleration > m_maxAngularAcceleration)
	{
		output->Angular /= angularAcceleration;
		output->Angular *= m_maxAngularAcceleration;
	}
}

float Align::mapToRange(float rotation)
{
	if (rotation > XM_PI)
		return rotation - 2 * XM_PI;
	if (rotation < -XM_PI)
		return rotation + 2 * XM_PI;
	return rotation;
}

Face::Face(Kinematic* character, Kinematic* target, const float& max_angular_acceleration, const float& max_rotation, const float& target_radius, const float& slow_radius, const float& time_to_target) :
	Align(character, nullptr, max_angular_acceleration, max_rotation, target_radius, slow_radius, time_to_target),
	m_target(target)
{
}

void Face::GetSteering(SteeringOutput* output)
{
	// 1. calculate the target to delegate to align

	// work out the direction to target
	Vector3 direction = m_target->Position - m_character->Position;

	// check for a zero direction, and make no change if so
	if (direction.Length() < std::numeric_limits<float>::epsilon())
		return; //return target?

	// put the target together
	Align::m_target = m_target;
	Align::m_target->Orientation = std::atan2(-direction.x, direction.y);

	// 2. delegate to align
	Align::GetSteering(output);
}

Wander::Wander(Kinematic* character, const float& max_angular_acceleration, const float& max_rotation, const float& target_radius, const float& slow_radius, const float& time_to_target, const float& wanderOffset, const float& wanderRadius, const float& wanderRate, const float& maxAcceleration) :
	Face(character, nullptr, max_angular_acceleration, max_rotation, target_radius, slow_radius, time_to_target), 
	m_wanderOffset(wanderOffset), 
	m_wanderRadius(wanderRadius), 
	m_wanderRate(wanderRate), 
	m_maxLinearAcceleration(maxAcceleration),
	m_wanderOrientation(character->Orientation)
{
}

void Wander::GetSteering(SteeringOutput* output)
{
	// 1. calculate the target to delegate to face

	// update the wander orientation
	m_wanderOrientation += m_randomDistribution(m_mersenneTwisterEngine) * m_wanderRate;

	Kinematic target;
	// calculate the combined target orientation
	target.Orientation = m_wanderOrientation + m_character->Orientation;

	// calculate the center of the wander circle
	target.Position = m_character->Position + m_wanderOffset * m_character->GetOrientationAsVector();

	// calculate the target location
	target.Position += m_wanderRadius * target.GetOrientationAsVector();

	// 2. delegate to face
	Face::m_target = &target;
	Face::GetSteering(output);

	// 3. now set the linear acceleration to be at full acceleration in the direction of the orientation
	output->Linear = m_maxLinearAcceleration * m_character->GetOrientationAsVector();
}

void Wander::initializeRandomDistribution()
{
	m_mersenneTwisterEngine = std::mt19937_64(m_randomDevice());
	m_randomDistribution = std::uniform_real_distribution<float>(-1, 1);
}
