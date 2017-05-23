#pragma once
#include "SimpleMovable2D.h"
#include "Collideable2D.h"

class Player : 
	public SimpleMovable2D, 
	public Collideable2D
{
public:
	Player();
	~Player();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Update(const float& deltaTime, const DirectX::Keyboard* keyboard);
	void Render(ID3D11DeviceContext* deviceContext, const Camera& camera);

	void AttachFollowingCamera(Camera* camera);
	void SetPosition(const DirectX::SimpleMath::Vector3& newPosition) override;

	const std::vector<DirectX::SimpleMath::Vector2>& GetVertices() const override;

private:
	void createMesh();
	void checkAndProcessKeyboardInput(const DirectX::Keyboard* keyboard, float deltaTime);
	void moveForward(const float& deltaTime) override;
	void moveBackwards(const float& deltaTime) override;

	int m_movementSpeedUpFactor = 1;

	Camera* m_followingCamera = nullptr;	

	//rendering
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;	
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::CommonStates> m_states;	
	std::vector<DirectX::VertexPositionColor> m_vertices;
	std::vector<DirectX::SimpleMath::Vector2> m_verticesPositions;
};

