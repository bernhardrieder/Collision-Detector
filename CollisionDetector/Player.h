#pragma once
class Player
{
public:
	Player();
	~Player();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceConrtext);
	void Update(const float& deltaTime, const DirectX::Keyboard* keyboard);
	void Render(ID3D11DeviceContext* deviceContext, const Camera& camera);

	void AttachFollowingCamera(Camera* camera);
	void SetPosition(const DirectX::SimpleMath::Vector3& newPosition);
	void SetRotationAngle(const float& newAngle);
	void SetMovementSpeed(const float& newMovementSpeed);
	void SetRotationSpeed(const float& newRotationSpeed);
	void SetScale(const DirectX::SimpleMath::Vector3& newScale);

private:
	void checkAndProcessKeyboardInput(const DirectX::Keyboard* keyboard, float deltaTime);
	void moveForward(const float& deltaTime);
	void moveBackwards(const float& deltaTime);
	void move(const DirectX::SimpleMath::Vector3& normalizedDirectionVector, const float& speed, const float& deltaTime);
	void rotateLeft(const float& deltaTime);
	void rotateRight(const float& deltaTime);
	void rotate(const float& degrees, const float& deltaTime);

	float m_movementSpeed = 40.f;
	DirectX::SimpleMath::Vector3 m_upVector = DirectX::SimpleMath::Vector3::Up;
	float m_rotationSpeed = 3.f;
	int m_speedUpFactor = 1;

	DirectX::SimpleMath::Vector3 m_position = DirectX::SimpleMath::Vector3::Zero;
	float m_rotationAngle = 0;
	DirectX::SimpleMath::Vector3 m_scale = DirectX::SimpleMath::Vector3::One;

	Camera* m_followingCamera = nullptr;	

	//rendering
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;	
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::CommonStates> m_states;
};

