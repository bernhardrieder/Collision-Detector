#pragma once

namespace SteeringBehaviours
{
	struct SteeringOutput
	{
		DirectX::SimpleMath::Vector3 Linear;
		float Angular;

		SteeringOutput() : Linear(DirectX::SimpleMath::Vector3::Zero), Angular(0) {}
		SteeringOutput(const DirectX::SimpleMath::Vector3& linear, const float& angular) : Linear(linear), Angular(angular) {}

		bool operator ==(const SteeringOutput& other) const;
		bool operator !=(const SteeringOutput& other) const;

		void Clear();
		float SquareMagnitude() const;
		float Magnitude() const;
	};

	struct Location
	{
		DirectX::SimpleMath::Vector3 Position;
		float Orientation;

		Location() : Position(DirectX::SimpleMath::Vector3::Zero), Orientation(0.0f) {}
		Location(const DirectX::SimpleMath::Vector3& position) : Position(position), Orientation(0.0f) {}
		Location(const DirectX::SimpleMath::Vector3& position, const float& orientation) : Position(position), Orientation(orientation) {}
		Location(const float & x, const float& y, const float& z, const float& orientation) : Position(x, y, z), Orientation(orientation) {}

		bool operator ==(const Location& other) const;
		bool operator !=(const Location& other) const;

		void Clear();
		void Integrate(const DirectX::SimpleMath::Vector3& velocity, const float& rotation, const float& deltaTime);
		DirectX::SimpleMath::Vector3 GetOrientationAsVector() const;
	};

	struct Kinematic : Location
	{
		DirectX::SimpleMath::Vector3 Velocity;
		float Rotation;

		Kinematic() : Location(), Velocity(0, 0, 0), Rotation(0) {}
		Kinematic(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity) : Location(position), Velocity(velocity), Rotation(0) {}
		Kinematic(const Location& loc, const DirectX::SimpleMath::Vector3& velocity) : Location(loc), Velocity(velocity), Rotation(0) {}
		Kinematic(const Location& loc) : Location(loc), Velocity(0, 0, 0), Rotation(0) {}
		Kinematic(const DirectX::SimpleMath::Vector3& position, const float& orientation, const DirectX::SimpleMath::Vector3& velocity, const float& rotation) : Location(position, orientation), Velocity(velocity), Rotation(rotation) {}

		Kinematic& operator =(const Location& other);
		Kinematic& operator =(const Kinematic& other);
		bool operator ==(const Kinematic& other) const;
		bool operator !=(const Kinematic& other) const;
		bool operator <(const Kinematic& other) const;
		void operator += (const Kinematic& other);
		void operator -= (const Kinematic& other);
		void operator *= (float f);

		void Clear();
		void Integrate(float deltaTime);
		void Integrate(const SteeringOutput& steer, const float& deltaTime);
		void TrimMaxSpeed(const float& speed);
	};

	static DirectX::SimpleMath::Vector3 OrientationToVector(float orientation);
}