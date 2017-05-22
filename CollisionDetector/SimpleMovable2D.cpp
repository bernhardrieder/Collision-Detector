#include "pch.h"
#include "SimpleMovable2D.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SimpleMovable2D::~SimpleMovable2D()
{
}

void SimpleMovable2D::SetPosition(const DirectX::SimpleMath::Vector3& newPosition)
{
	m_transform.Position = newPosition;
	m_transform.Matrices.Translation = Matrix::CreateTranslation(m_transform.Position);
}

void SimpleMovable2D::SetRotationAngle(const float& newAngle)
{
	m_transform.RotationAngle = newAngle;
	Vector3::TransformNormal(Vector3::UnitY, Matrix::CreateRotationZ(m_transform.RotationAngle), m_moveDirection);
	m_transform.Matrices.Rotation = Matrix::CreateRotationZ(m_transform.RotationAngle);
}

void SimpleMovable2D::SetMovementSpeed(const float& newMovementSpeed)
{
	m_movementSpeed = newMovementSpeed;
}

void SimpleMovable2D::SetRotationSpeed(const float& newRotationSpeed)
{
	m_rotationSpeed = newRotationSpeed;

}

void SimpleMovable2D::SetScale(const DirectX::SimpleMath::Vector3& newScale)
{
	m_transform.Scale = newScale;
	m_transform.Matrices.Scale = Matrix::CreateScale(m_transform.Scale);
}

void SimpleMovable2D::moveForward(const float& deltaTime)
{
	move(m_moveDirection, m_movementSpeed, deltaTime);

}

void SimpleMovable2D::moveBackwards(const float& deltaTime)
{
	move(-m_moveDirection, m_movementSpeed, deltaTime);

}

void SimpleMovable2D::move(const DirectX::SimpleMath::Vector3& normalizedDirectionVector, const float& speed, const float& deltaTime)
{
	SetPosition(m_transform.Position + normalizedDirectionVector * speed * deltaTime);

}

void SimpleMovable2D::rotateLeft(const float& deltaTime)
{
	rotate(m_rotationSpeed, deltaTime);

}

void SimpleMovable2D::rotateRight(const float& deltaTime)
{
	rotate(-m_rotationSpeed, deltaTime);
}

void SimpleMovable2D::rotate(const float& degrees, const float& deltaTime)
{
	SetRotationAngle(m_transform.RotationAngle + degrees * deltaTime);
}
