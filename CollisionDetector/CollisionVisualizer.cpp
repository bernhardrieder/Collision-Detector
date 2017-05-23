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
	for(size_t i = 0; i < 500; ++i)
		addEffect(device, i);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effects[0]->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	DX::ThrowIfFailed(
		device->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			m_inputLayout.ReleaseAndGetAddressOf()));

	initializeBoundingSphere();
	initializeAABB();
	initializeOBB();
}

void CollisionVisualizer::Render(const std::vector<CollisionObject>& collidables, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	deviceContext->RSSetState(m_states->CullNone());

	//m_effects[0]->Apply(deviceContext);
	deviceContext->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();


	for (uint64_t i = 0; i < collidables.size(); ++i)
	{
		if (m_effects.size() <= i)
			addEffect(device, i);
		m_effects[i]->Apply(deviceContext);
		drawBoundingSphere(collidables[i], camera, m_effects[i].get());
	}

	//for(auto& obj : collidables)
	//{
	//	if (!obj.HasCollision())
	//		continue;
	//	for(auto& collision : obj.Collisions)
	//	{
	//		switch(collision.LastDetectedType)
	//		{
	//		case BoundingVolume: 
	//			drawBoundingSphere(obj, camera); 
	//			break;
	//		case AABB: 
	//			drawAABB(obj, camera); 
	//			break;
	//		case OBB: 
	//			drawOBB(obj, camera); 
	//			break;
	//		case MinkovskySum: 
	//			drawMinkovskySum(obj, camera); 
	//			break;
	//		case None: 
	//		default: break;
	//		}
	//		
	//	}
	//}

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

void CollisionVisualizer::addEffect(ID3D11Device* device, const uint64_t& effectIndex)
{
	m_effects.push_back(std::make_unique<BasicEffect>(device));
	m_effects[effectIndex]->SetVertexColorEnabled(true);
}

void CollisionVisualizer::drawBoundingSphere(const CollisionObject& obj, const Camera& camera, DirectX::BasicEffect* effect)
{
	Matrix translation = Matrix::CreateTranslation(obj.Object->GetBoundingSphereTransformed().Center);
	Matrix scale = Matrix::CreateScale(obj.Object->GetBoundingSphereTransformed().Radius);

	effect->SetMatrices(scale*Matrix::Identity*translation, camera.GetView(), camera.GetProj());

	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, &m_verticesBoundingSphere[0], m_verticesBoundingSphere.size());
}

void CollisionVisualizer::drawAABB(const CollisionObject& obj, const Camera& camera, DirectX::BasicEffect* effect)
{
	//todo
}

void CollisionVisualizer::drawOBB(const CollisionObject& obj, const Camera& camera, DirectX::BasicEffect* effect)
{
	//todo
}

void CollisionVisualizer::drawMinkovskySum(const CollisionObject& obj, const Camera& camera, DirectX::BasicEffect* effect)
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
