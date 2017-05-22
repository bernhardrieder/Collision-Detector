#pragma once

#include "SteeringBehaviourHelper.h"

class SteeringBehaviour
{
public:
	SteeringBehaviour(Kinematic* character);

	virtual ~SteeringBehaviour() {}

	virtual void GetSteering(SteeringOutput* output) = 0;
	void SetCharacter(Kinematic* character);

protected:
	Kinematic* Character;
};

class Align : public SteeringBehaviour
{
public:
	Align(Kinematic* character, Kinematic* target, float max_angular_acceleration, float max_rotation, float target_radius, float slow_radius, float time_to_target);
	void GetSteering(SteeringOutput* output) override;

protected:
	Kinematic* Target;

	float MaxAngularAcceleration;
	float MaxRotation;

	float TargetRadius;
	float SlowRadius;

	float TimeToTarget = 0.1f;

private:
	static float mapToRange(float rotation);
};

class Face : public Align
{
	Face(Kinematic* character, Kinematic* target, float max_angular_acceleration, float max_rotatiom, float target_radius, float slow_radius, float time_to_target);
	void GetSteering(SteeringOutput* output) override;

protected:
	Kinematic* Target;
};