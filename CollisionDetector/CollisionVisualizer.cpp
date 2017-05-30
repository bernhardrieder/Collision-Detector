#include "pch.h"
#include "CollisionVisualizer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace CollisionDetection;

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

	initializeBoundingSphere();
	initializeAABB();
	initializeOBB();
}

void CollisionVisualizer::Render(const std::vector<CollisionObject>& collidables, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	deviceContext->RSSetState(m_states->CullNone());

	deviceContext->IASetInputLayout(m_inputLayout.Get());

	//for (uint64_t i = 0; i < collidables.size(); ++i)
	//{
	//	m_batch->Begin();
	//	drawBoundingSphere(collidables[i], camera, deviceContext);
	//	//drawAABB(collidables[i], camera, deviceContext);
	//	//drawOBB(collidables[i], camera, deviceContext);
	//	m_batch->End();
	//}
	//return;

	for (uint64_t i = 0; i < collidables.size(); ++i)
	{
		if (!collidables[i].HasCollision())
			continue;

		m_batch->Begin();
		for(auto& collision : collidables[i].Collisions)
		{
			switch(collision.LastDetectedType)
			{
			case BoundingVolume: 
				drawBoundingSphere(collidables[i], camera, deviceContext);
				break;
			case AABB: 
				drawAABB(collidables[i], camera, deviceContext);
				break;
			case OBB: 
				drawOBB(collidables[i], camera, deviceContext);
				break;
			case MinkovskiDifference: 
				drawMinkovskiDifference(collidables[i], camera, deviceContext);
				break;
			case None: 
			default: break;
			}
		}
		m_batch->End();
	}
}

void CollisionVisualizer::initializeBoundingSphere()
{
	auto vertices = createCircleVerticesLineStrip(1.f);
	for (auto& vertex : vertices)
		m_verticesBoundingSphere.push_back(DirectX::VertexPositionColor({vertex.x, vertex.y, 0.f}, m_boundingSphereColor));
}

void CollisionVisualizer::initializeAABB()
{
	auto vertices = createBoxVerticesLineStrip();
	for (auto& vertex : vertices)
		m_verticesAABB.push_back(DirectX::VertexPositionColor({ vertex.x, vertex.y, 0.f }, m_aabbColor));
}

void CollisionVisualizer::initializeOBB()
{
	auto vertices = createBoxVerticesLineStrip();
	for (auto& vertex : vertices)
		m_verticesOBB.push_back(DirectX::VertexPositionColor({ vertex.x, vertex.y, 0.f }, m_obbColor));
}

void CollisionVisualizer::drawBoundingSphere(const CollisionObject& obj, const Camera& camera, ID3D11DeviceContext* deviceContext)
{
	Matrix translation = Matrix::CreateTranslation(obj.Object->GetBoundingSphereTransformed().GetCenter());
	Matrix scale = Matrix::CreateScale(obj.Object->GetBoundingSphereTransformed().GetRadius());

	m_effect->SetMatrices(scale*Matrix::Identity*translation, camera.GetView(), camera.GetProj());
	m_effect->Apply(deviceContext);

	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, &m_verticesBoundingSphere[0], m_verticesBoundingSphere.size());
}

void CollisionVisualizer::drawAABB(const CollisionObject& obj, const Camera& camera, ID3D11DeviceContext* deviceContext)
{
	Matrix translation = Matrix::CreateTranslation(obj.Object->GetAxisAlignedBoundingBoxTransformed().Center);
	Matrix scale = Matrix::CreateScale(obj.Object->GetAxisAlignedBoundingBoxTransformed().Extents.x , obj.Object->GetAxisAlignedBoundingBoxTransformed().Extents.y , 1.f);

	m_effect->SetMatrices(scale*Matrix::Identity*translation, camera.GetView(), camera.GetProj());
	m_effect->Apply(deviceContext);

	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, &m_verticesAABB[0], m_verticesAABB.size());
}

void CollisionVisualizer::drawOBB(const CollisionObject& obj, const Camera& camera, ID3D11DeviceContext* deviceContext)
{
	Matrix translation = Matrix::CreateTranslation(obj.Object->GetOrientedBoundingBoxTransformed().Center);
	Matrix scale = Matrix::CreateScale(obj.Object->GetOrientedBoundingBoxTransformed().Extents.x, obj.Object->GetOrientedBoundingBoxTransformed().Extents.y, 1.f);
	const Matrix& rotation = obj.Object->GetLastAppliedRotationMatrix();

	m_effect->SetMatrices(scale*rotation*translation, camera.GetView(), camera.GetProj());
	m_effect->Apply(deviceContext);

	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, &m_verticesOBB[0], m_verticesOBB.size());
}

void CollisionVisualizer::drawMinkovskiDifference(const CollisionObject& obj, const Camera& camera, ID3D11DeviceContext* deviceContext)
{
	//todo
}

std::vector<Vector2> CollisionVisualizer::createCircleVerticesLineStrip(const float& radius) const
{
	std::vector<Vector2> vertices;
	createCircleVertices(vertices, radius, 50);
	vertices.push_back(vertices[0]);
	return vertices;
}

std::vector<Vector2> CollisionVisualizer::createCircleVerticesTriangleFan(const DirectX::SimpleMath::Vector2& center, const float& radius) const
{
	std::vector<Vector2> vertices;
	vertices.push_back(center);
	createCircleVertices(vertices, radius, 50);
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

std::vector<DirectX::SimpleMath::Vector2> CollisionVisualizer::createBoxVerticesLineStrip()
{
	std::vector<Vector2> vertices;
	vertices.push_back({ -1.f, -1.f });
	vertices.push_back({ 1.f, -1.f });
	vertices.push_back({ 1.f, 1.f });
	vertices.push_back({ -1.f, 1.f });
	vertices.push_back(vertices[0]);
	return vertices;
}
