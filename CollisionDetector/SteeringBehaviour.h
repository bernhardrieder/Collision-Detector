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
		Kinematic* m_character;
	};

	class Align : public SteeringBehaviour
	{
	public:
		Align(Kinematic* character, Kinematic* target, const float& maxAngularAcceleration, const float& maxRotation, const float& targetRadius, const float& slowRadius, const float& timeToTarget);
		void GetSteering(SteeringOutput* output) override;

	protected:
		Kinematic* m_target;

		float m_maxAngularAcceleration;
		float m_maxRotation;

		float m_targetRadius;
		float m_slowRadius;

		float m_timeToTarget = 0.1f;

	private:
		static float mapToRange(float rotation);
	};

	class Face : public Align
	{
	public:
		Face(Kinematic* character, Kinematic* target, const float& maxAngularAcceleration, const float& maxRotation, const float& targetRadius, const float& slowRadius, const float& timeToTarget);
		void GetSteering(SteeringOutput* output) override;

	protected:
		Kinematic* m_target;
	};

	class Wander : public Face
	{
	public:
		Wander(Kinematic* character, const float& maxAngularAcceleration, const float& maxRotation, const float& targetRadius, const float& slowRadius, const float& timeToTarget, const float& wanderOffset, const float& wanderRadius, const float& wanderRate, const float& maxLinearAcceleration);
		void GetSteering(SteeringOutput* output) override;

	protected:
		// holds the radius and forward offset of the wander circle
		float m_wanderOffset;
		float m_wanderRadius;

		// holds the maximum rate at which the wander orientation can change
		float m_wanderRate;

		// holds the maximum acceleration of the character
		float m_maxLinearAcceleration;

		// holds the current orientation of the wander target
		float m_wanderOrientation = 0;

	private:
		void initializeRandomDistribution();

		std::random_device m_randomDevice;
		std::mt19937_64 m_mersenneTwisterEngine;
		std::uniform_real_distribution<float> m_randomDistribution;
	};
}