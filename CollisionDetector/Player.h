#pragma once
class Player
{
public:
	Player();
	~Player();

	void Update(float deltaTime);
	void Render(ID3D11DeviceContext* deviceContext);

	void AttachFollowingCamera(Camera* camera);
	void SetPosition(const DirectX::SimpleMath::Vector3& newPosition);
	void SetRotationAngle(const float& newAngle);
	void SetMovementSpeed(const float& newMovementSpeed);
	void SetRotationSpeed(const float& newRotationSpeed);

private:
	void checkAndProcessKeyboardInput(float deltaTime);
	void moveForward(const float& deltaTime);
	void moveBackwards(const float& deltaTime);
	void move(const DirectX::SimpleMath::Vector3& normalizedDirectionVector, const float& speed, const float& deltaTime);
	void rotateLeft(const float& deltaTime);
	void rotateRight(const float& deltaTime);
	void rotate(const float& degrees, const float& deltaTime);

	float m_movementSpeed = 10.f;
	float m_rotationSpeed = 10.f;

	DirectX::SimpleMath::Vector3 m_position;
	float m_rotationAngle = 0;
	const DirectX::SimpleMath::Vector3 m_rotationAxis = DirectX::SimpleMath::Vector3::UnitZ;

	Camera* m_followingCamera = nullptr;	
	std::unique_ptr<DirectX::Keyboard> m_keyboard;


};

