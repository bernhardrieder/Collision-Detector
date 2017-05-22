#include "pch.h"
#include "Asteroid.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Asteroid::Asteroid()
{
	m_mersenneTwisterEngine = std::mt19937_64(m_randomDevice());
	m_randomMovementSpeedDistribution = std::uniform_real_distribution<float>(10.f, 30.f);
	m_randomRotationAngleDistribution = std::uniform_real_distribution<float>(-10.f, 10.f);

	m_movementSpeed = m_randomMovementSpeedDistribution(m_mersenneTwisterEngine);
}


Asteroid::~Asteroid()
{
}

void Asteroid::InitializeRenderable(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::uniform_real_distribution<float>& verticesDistribution)
{
	createConvexHullWithJarvisMarch(verticesDistribution);

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

void Asteroid::InitializeTransform(const std::uniform_real_distribution<float>& positionDistribution, const std::uniform_real_distribution<float>& rotationAngleDistribution, const std::uniform_real_distribution<float>& scaleDistribution)
{
	SetPosition(Vector3(positionDistribution(m_mersenneTwisterEngine), positionDistribution(m_mersenneTwisterEngine), 0.f));
	SetRotationAngle(rotationAngleDistribution(m_mersenneTwisterEngine));
	SetScale(Vector3::One * scaleDistribution(m_mersenneTwisterEngine));
}

void Asteroid::Update(const float& deltaTime)
{
	updateMovementSpeedAndRotationChange(deltaTime);
	moveForward(deltaTime);
}

void Asteroid::Render(ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	deviceContext->RSSetState(m_states->CullNone());

	m_effect->Apply(deviceContext);

	deviceContext->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();

	auto& m = m_transform.Matrices;
	m_effect->SetMatrices(m.Scale*m.Rotation*m.Translation, camera.GetView(), camera.GetProj());

	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, &m_vertices[0], m_vertices.size());

	m_batch->End();
}

void Asteroid::updateMovementSpeedAndRotationChange(const float& deltaTime)
{
	m_timeSinceLastMovementSpeedAndRotationChange += deltaTime;
	if(m_timeSinceLastMovementSpeedAndRotationChange >= m_randomMovementSpeedAndRotationChangeTimeThreshold)
	{
		rotate(m_randomRotationAngleDistribution(m_mersenneTwisterEngine), deltaTime);
		m_movementSpeed = m_randomMovementSpeedDistribution(m_mersenneTwisterEngine);
		m_timeSinceLastMovementSpeedAndRotationChange = 0.f;
	}
}

void Asteroid::createConvexHullWithJarvisMarch(const std::uniform_real_distribution<float>& verticesDistribution)
{
	std::vector<Vector2> randomVertices;
	size_t randomVerticesCount = 8;

	for (size_t i = 0; i < randomVerticesCount; ++i)
		randomVertices.push_back(Vector2(verticesDistribution(m_mersenneTwisterEngine), verticesDistribution(m_mersenneTwisterEngine)));

	JarvisMarch jarvisMarch;
	auto hull = jarvisMarch.GetConvexHull(randomVertices);
	for (size_t i = 0; i < hull.size(); ++i)
		m_vertices.push_back(VertexPositionColor(Vector3(hull[i].x, hull[i].y, 0.f), Colors::DarkGray));
	m_vertices.push_back(VertexPositionColor(Vector3(hull[0].x, hull[0].y, 0.f), Colors::DarkGray));
}

void Asteroid::simpleWanderAlgorithm(const float& deltaTime)
{
}
