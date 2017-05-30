#pragma once
#include "CollisionDetector2D.h"

class CollisionVisualizer
{
public:
	CollisionVisualizer();
	~CollisionVisualizer();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Render(const std::vector<CollisionDetection::CollisionObject>& collidables, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const Camera& camera);

private:
	void initializeBoundingSphere();
	void initializeAABB();
	void initializeOBB();

	void drawBoundingSphere(const CollisionDetection::CollisionObject& obj, const Camera& camera, ID3D11DeviceContext* deviceContext);
	void drawAABB(const CollisionDetection::CollisionObject& obj, const Camera& camera, ID3D11DeviceContext* deviceContext);
	void drawOBB(const CollisionDetection::CollisionObject& obj, const Camera& camera, ID3D11DeviceContext* deviceContext);
	void drawMinkovskiDifference(const CollisionDetection::CollisionObject& obj, const Camera& camera, ID3D11DeviceContext* deviceContext);

	std::vector<DirectX::SimpleMath::Vector2> createCircleVerticesLineStrip(const float& radius) const;
	std::vector<DirectX::SimpleMath::Vector2> createCircleVerticesTriangleFan(const DirectX::SimpleMath::Vector2& center, const float& radius) const;
	void createCircleVertices(std::vector<DirectX::SimpleMath::Vector2>& vertices, const float& radius, const float& resolution) const;
	static std::vector<DirectX::SimpleMath::Vector2> createBoxVerticesLineStrip();

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::CommonStates> m_states;

	const DirectX::XMVECTORF32 m_boundingSphereColor = DirectX::Colors::Yellow;
	const DirectX::XMVECTORF32 m_aabbColor = DirectX::Colors::Blue;
	const DirectX::XMVECTORF32 m_obbColor = DirectX::Colors::Green;
	const DirectX::XMVECTORF32 m_minkovskySumColor = DirectX::Colors::White;

	std::vector<DirectX::VertexPositionColor> m_verticesBoundingSphere;
	std::vector<DirectX::VertexPositionColor> m_verticesAABB;
	std::vector<DirectX::VertexPositionColor> m_verticesOBB;
};

