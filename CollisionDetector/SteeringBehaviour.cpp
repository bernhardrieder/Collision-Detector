#include "pch.h"
#include "SteeringBehaviour.h"

using namespace SteeringBehaviours;
using namespace DirectX;
using namespace DirectX::SimpleMath;

SteeringBehaviour::SteeringBehaviour(Kinematic* character) : 
	Character(character)
{
}

void SteeringBehaviour::SetCharacter(Kinematic* character)
{
	Character = character;
}

Align::Align(Kinematic* character, Kinematic* target, float max_angular_acceleration, float max_rotation, float target_radius, float slow_radius, float time_to_target) : 
	SteeringBehaviour(character),
	Target(target),
	MaxAngularAcceleration(max_angular_acceleration),
	MaxRotation(max_rotation),
	TargetRadius(target_radius),
	SlowRadius(slow_radius),
	TimeToTarget(time_to_target)
{
}

void Align::GetSteering(SteeringOutput* output)
{
	float targetRotation;
	float rotation = Target->Orientation - Character->Orientation;

	//Map the result to the (-pi, pi) interval
	rotation = mapToRange(rotation);

	float rotationSize = std::abs(rotation);
	if (rotationSize < TargetRadius)
		return;
	if (rotationSize > SlowRadius)
		targetRotation = MaxRotation;
	else
		targetRotation = MaxRotation * rotationSize / SlowRadius;

	targetRotation *= rotation / rotationSize;
	output->Angular = targetRotation - Character->Rotation;
	output->Angular /= TimeToTarget;

	float angularAcceleration = std::abs(output->Angular);
	if (angularAcceleration > MaxAngularAcceleration)
	{
		output->Angular /= angularAcceleration;
		output->Angular *= MaxAngularAcceleration;
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

Face::Face(Kinematic* character, Kinematic* target, float max_angular_acceleration, float max_rotation, float target_radius, float slow_radius, float time_to_target) :
	Align(character, nullptr, max_angular_acceleration, max_rotation, target_radius, slow_radius, time_to_target),
	Target(target)
{
}

void Face::GetSteering(SteeringOutput* output)
{
	// 1. calculate the target to delegate to align

	// work out the direction to target
	Vector3 direction = Target->Position - Character->Position;

	// check for a zero direction, and make no change if so
	if (direction.Length() < std::numeric_limits<float>::epsilon())
		return; //return target?

	// put the target together
	Align::Target = Target;
	Align::Target->Orientation = std::atan2(-direction.x, direction.y);

	// 2. delegate to align
	Align::GetSteering(output);
}

Wander::Wander(Kinematic* character, float max_angular_acceleration, float max_rotation, float target_radius, float slow_radius, float time_to_target, float wanderOffset, float wanderRadius, float wanderRate, float maxAcceleration) :
	Face(character, nullptr, max_angular_acceleration, max_rotation, target_radius, slow_radius, time_to_target), 
	WanderOffset(wanderOffset), 
	WanderRadius(wanderRadius), 
	WanderRate(wanderRate), 
	MaxAcceleration(maxAcceleration)
{
}

void Wander::GetSteering(SteeringOutput* output)
{
	// 1. calculate the target to delegate to face

	// update the wander orientation
	WanderOrientation += m_randomDistribution(m_mersenneTwisterEngine) * WanderRate;

	Kinematic target;
	// calculate the combined target orientation
	target.Orientation = WanderOrientation + Character->Orientation;

	// calculate the center of the wander circle
	target.Position = Character->Position + WanderOffset * Character->GetOrientationAsVector();

	// calculate the target location
	target.Position += WanderRadius * target.GetOrientationAsVector();

	// 2. delegate to face
	Face::Target = &target;
	Face::GetSteering(output);

	// 3. now set the linear acceleration to be at full acceleration in the direction of the orientation
	output->Linear = MaxAcceleration * Character->GetOrientationAsVector();
}

void Wander::initializeRandomBinomial()
{
	m_mersenneTwisterEngine = std::mt19937_64(m_randomDevice());
	m_randomDistribution = std::uniform_real_distribution<float>(-1, 1);
}
