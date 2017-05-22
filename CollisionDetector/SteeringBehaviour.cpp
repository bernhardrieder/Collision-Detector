#include "pch.h"
#include "SteeringBehaviour.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SteeringBehaviour::SteeringBehaviour(Kinematic* character) : Character(character)
{
}

void SteeringBehaviour::SetCharacter(Kinematic* character)
{
	Character = character;
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

void Face::GetSteering(SteeringOutput* output)
{
}
