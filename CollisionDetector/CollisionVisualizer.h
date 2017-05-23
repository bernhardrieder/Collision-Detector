#pragma once
#include "CollisionDetector2D.h"

class CollisionVisualizer
{
public:
	CollisionVisualizer();
	~CollisionVisualizer();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Render(const std::vector<CollisionObject>& collidables, ID3D11DeviceContext* deviceContext, const Camera& camera);

private:
	void initializeBoundingSphere();
	void initializeAABB();
	void initializeOBB();

	void drawBoundingSphere(const CollisionObject& obj, const Camera& camera);
	void drawAABB(const CollisionObject& obj, const Camera& camera);
	void drawOBB(const CollisionObject& obj, const Camera& camera);
	void drawMinkovskySum(const CollisionObject& obj, const Camera& camera);

	std::vector<DirectX::SimpleMath::Vector2> createCircleVerticesLineStrip(const float& radius) const;
	std::vector<DirectX::SimpleMath::Vector2> createCircleVerticesTriangleFan(const DirectX::SimpleMath::Vector2& center, const float& radius) const;
	void createCircleVertices(std::vector<DirectX::SimpleMath::Vector2>& vertices, const float& radius, const float& resolution) const;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::CommonStates> m_states;

	const DirectX::XMVECTORF32 m_boundingSphereColor = DirectX::Colors::Red;
	const DirectX::XMVECTORF32 m_aabbColor = DirectX::Colors::Blue;
	const DirectX::XMVECTORF32 m_obbColor = DirectX::Colors::Green;
	const DirectX::XMVECTORF32 m_minkovskySumColor = DirectX::Colors::Blue;

	std::vector<DirectX::VertexPositionColor> m_verticesBoundingSphere;
	std::vector<DirectX::VertexPositionColor> m_verticesAABB;
	std::vector<DirectX::VertexPositionColor> m_verticesOBB;
};

