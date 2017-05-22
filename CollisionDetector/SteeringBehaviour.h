#pragma once

#include "SteeringBehaviourHelper.h"

namespace SteeringBehaviours
{
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
	public:
		Face(Kinematic* character, Kinematic* target, float max_angular_acceleration, float max_rotation, float target_radius, float slow_radius, float time_to_target);
		void GetSteering(SteeringOutput* output) override;

	protected:
		Kinematic* Target;
	};

	class Wander : public Face
	{
	public:
		Wander(Kinematic* character, float max_angular_acceleration, float max_rotation, float target_radius, float slow_radius, float time_to_target, float wanderOffset, float wanderRadius, float wanderRate, float maxAcceleration);
		void GetSteering(SteeringOutput* output) override;

	protected:
		// holds the radius and forward offset of the wander circle
		float WanderOffset;
		float WanderRadius;

		// holds the maximum rate at which the wander orientation can change
		float WanderRate;

		// holds the maximum acceleration of the character
		float MaxAcceleration;

		// holds the current orientation of the wander target
		float WanderOrientation = 0;
	private:

		//todo: initialize;
		std::random_device m_randomDevice;
		std::mt19937_64 m_mersenneTwisterEngine;
		std::uniform_real_distribution<float> m_randomDistribution;

		void initializeRandomBinomial();
	};
}