#include "pch.h"
#include "CollisionVisualizer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

CollisionVisualizer::CollisionVisualizer()
{
}


CollisionVisualizer::~CollisionVisualizer()
{
}

void CollisionVisualizer::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(deviceContext);
	m_states = std::make_unique<CommonStates>(device);

	/******************* simple shader *******************/
	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	DX::ThrowIfFailed(
		device->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			m_inputLayout.ReleaseAndGetAddressOf()));
}

void CollisionVisualizer::Render(const std::vector<CollisionObject>& collidables, ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	deviceContext->RSSetState(m_states->CullNone());

	m_effect->Apply(deviceContext);

	deviceContext->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();

	for(auto& obj : collidables)
	{
		if (!obj.HasCollision())
			continue;
		for(auto& collision : obj.Collisions)
		{
			switch(collision.LastDetectedType)
			{
			case BoundingVolume: drawBoundingSphere(obj, deviceContext, camera); break;
			case AABB: drawAABB(obj, deviceContext, camera); break;
			case OBB: drawOBB(obj, deviceContext, camera); break;
			case MinkovskySum: drawMinkovskySum(obj, deviceContext, camera); break;
			case None: 
			default: break;
			}
			
		}
	}
	//m_effect->SetMatrices(m_transform.Matrices.CalculateWorld(), camera.GetView(), camera.GetProj());

	//m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vertices[0], m_vertices.size());

	m_batch->End();
}

void CollisionVisualizer::initializeBoundingSphere()
{
	auto vertices = createCircleVerticesLineStrip(1.f);
	for (auto& vertex : vertices)
		m_verticesBoundingSphere.push_back(DirectX::VertexPositionColor({vertex.x, vertex.y, 0.f}, m_boundingSphereColor));
}

void CollisionVisualizer::initializeAABB()
{
	//todo
}

void CollisionVisualizer::initializeOBB()
{
	//todo
}

void CollisionVisualizer::drawBoundingSphere(const CollisionObject& obj, ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	//todo
}

void CollisionVisualizer::drawAABB(const CollisionObject& obj, ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	//todo
}

void CollisionVisualizer::drawOBB(const CollisionObject& obj, ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	//todo
}

void CollisionVisualizer::drawMinkovskySum(const CollisionObject& obj, ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	//todo
}

std::vector<Vector2> CollisionVisualizer::createCircleVerticesLineStrip(const float& radius) const
{
	std::vector<Vector2> vertices;
	createCircleVertices(vertices, radius, 100);
	vertices.push_back(vertices[0]);
	return vertices;
}

std::vector<Vector2> CollisionVisualizer::createCircleVerticesTriangleFan(const DirectX::SimpleMath::Vector2& center, const float& radius) const
{
	std::vector<Vector2> vertices;
	vertices.push_back(center);
	createCircleVertices(vertices, radius, 100);
	return vertices;
}

void CollisionVisualizer::createCircleVertices(std::vector<DirectX::SimpleMath::Vector2>& vertices, const float& radius, const float& resolution) const
{
	for (float i = 0; i <= resolution; i += 1.0f)
	{
		float t = XM_PI * 2 * i / resolution;
		vertices.push_back(Vector2(radius * cos(t), radius * -sin(t)));
	}
}
