#pragma once
#include "SimpleMovable2D.h"
class Asteroid :
	public SimpleMovable2D
{
public:
	Asteroid();
	~Asteroid();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceConrtext);
	void Update(const float& deltaTime);
	void Render(ID3D11DeviceContext* deviceContext, const Camera& camera);

private:
	//rendering
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::CommonStates> m_states;
};

