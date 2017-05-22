#include "pch.h"
#include "SteeringBehaviourHelper.h"

using namespace SteeringBehaviours;
using namespace DirectX;
using namespace DirectX::SimpleMath;

bool SteeringOutput::operator==(const SteeringOutput& other) const
{
	return Linear == other.Linear && Angular == other.Angular;
}

bool SteeringOutput::operator!=(const SteeringOutput& other) const
{
	return Linear != other.Linear || Angular != other.Angular;
}

void SteeringOutput::Clear()
{
	Linear = DirectX::SimpleMath::Vector3::Zero;
	Angular = 0;
}

float SteeringOutput::SquareMagnitude() const
{
	return Linear.LengthSquared() + Angular * Angular;
}

float SteeringOutput::Magnitude() const
{
	return std::sqrt(SquareMagnitude());
}

bool Location::operator==(const Location& other) const
{
	return Position == other.Position && Orientation == other.Orientation;
}

bool Location::operator!=(const Location& other) const
{
	return Position != other.Position || Orientation != other.Orientation;
}

void Location::Clear()
{
	Position = DirectX::SimpleMath::Vector3::Zero;
	Orientation = 0.0f;
}

void Location::Integrate(const DirectX::SimpleMath::Vector3& velocity, const float& rotation, const float& deltaTime)
{
	Position += (velocity * deltaTime);
	Orientation += (rotation * deltaTime);
	
	Orientation = std::fmod(Orientation, 2 * XM_PI);
}

DirectX::SimpleMath::Vector3 Location::GetOrientationAsVector() const
{
	return OrientationToVector(Orientation);
}

Kinematic& Kinematic::operator=(const Location& other)
{
	Orientation = other.Orientation;
	Position = other.Position;
	return *this;
}

Kinematic& Kinematic::operator=(const Kinematic& other)
{
	Orientation = other.Orientation;
	Position = other.Position;
	Velocity = other.Velocity;
	Rotation = other.Rotation;
	return *this;
}

bool Kinematic::operator==(const Kinematic& other) const
{
	return Position == other.Position && Orientation == other.Orientation && Velocity == other.Velocity && Rotation == other.Rotation;
}

bool Kinematic::operator!=(const Kinematic& other) const
{
	return Position != other.Position || Orientation != other.Orientation || Velocity != other.Velocity || Rotation != other.Rotation;
}

bool Kinematic::operator<(const Kinematic& other) const
{
	return Position.x < other.Position.x;
}

void Kinematic::operator+=(const Kinematic& other)
{
	Position += other.Position;
	Velocity += other.Velocity;
	Rotation += other.Rotation;
	Orientation += other.Orientation;
}

void Kinematic::operator-=(const Kinematic& other)
{
	Position -= other.Position;
	Velocity -= other.Velocity;
	Rotation -= other.Rotation;
	Orientation -= other.Orientation;
}

void Kinematic::operator*=(float f)
{
	Position *= f;
	Velocity *= f;
	Rotation *= f;
	Orientation *= f;
}

void Kinematic::Clear()
{
	Location::Clear();
	Velocity = DirectX::SimpleMath::Vector3::Zero;
	Rotation = 0.0f;
}

void Kinematic::Integrate(float deltaTime)
{
	Location::Integrate(Velocity, Rotation, deltaTime);
}

void Kinematic::Integrate(const SteeringOutput& steer, const float& deltaTime)
{
	Location::Integrate(Velocity, Rotation, deltaTime);

	Velocity += steer.Linear * deltaTime;
	DirectX::SimpleMath::Vector3 velocityNormalized = Velocity;
	velocityNormalized.Normalize();
	Rotation += steer.Angular * deltaTime;
}

void Kinematic::TrimMaxSpeed(const float& speed)
{
	if (Velocity.LengthSquared() > std::pow(speed, 2))
	{
		Velocity.Normalize();
		Velocity *= speed;
	}
}

DirectX::SimpleMath::Vector3 SteeringBehaviours::OrientationToVector(float orientation)
{
	return DirectX::SimpleMath::Vector3(-std::sin(orientation), std::cos(orientation), 0);

}
