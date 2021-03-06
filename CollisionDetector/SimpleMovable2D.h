#pragma once

struct Transform
{
	DirectX::SimpleMath::Vector3 Position = DirectX::SimpleMath::Vector3::Zero;
	float RotationAngle = 0;
	DirectX::SimpleMath::Vector3 Scale = DirectX::SimpleMath::Vector3::One;

	struct Matrices
	{
		DirectX::SimpleMath::Matrix Translation = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix Rotation = DirectX::SimpleMath::Matrix::Identity;
		DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::Identity;

		DirectX::SimpleMath::Matrix CalculateWorld() { return Scale*Rotation*Translation; }
	} Matrices;


};

class SimpleMovable2D
{
public:
	virtual ~SimpleMovable2D();

	virtual void SetPosition(const DirectX::SimpleMath::Vector3& newPosition);
	virtual void SetRotationAngle(const float& newAngle);
	virtual void SetScale(const DirectX::SimpleMath::Vector3& newScale);

	virtual void SetMovementSpeed(const float& newMovementSpeed);
	virtual void SetRotationSpeed(const float& newRotationSpeed);

protected:
	virtual void moveForward(const float& deltaTime);
	virtual void moveBackwards(const float& deltaTime);
	void move(const DirectX::SimpleMath::Vector3& normalizedDirectionVector, const float& speed, const float& deltaTime);
	virtual void rotateLeft(const float& deltaTime);
	virtual void rotateRight(const float& deltaTime);
	void rotate(const float& degrees, const float& deltaTime);
	
	DirectX::SimpleMath::Vector3 m_moveDirection = DirectX::SimpleMath::Vector3::Up;
	float m_movementSpeed = 40.f;
	float m_rotationSpeed = 3.f;

	Transform m_transform;
};

