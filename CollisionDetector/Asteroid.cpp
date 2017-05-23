#include "pch.h"
#include "Asteroid.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace SteeringBehaviours;

Asteroid::Asteroid()
{
	m_mersenneTwisterEngine = std::mt19937_64(m_randomDevice());
	m_randomRotationAngleDistribution = std::uniform_real_distribution<float>(-1.f, 1.f);
	m_randomRotationSpeed = m_randomRotationAngleDistribution(m_mersenneTwisterEngine);

	auto& mt = m_mersenneTwisterEngine;
	auto maxAngularAccelerationDist = std::uniform_real_distribution<float>(5.f, 100.f);
	auto maxRotationDist = std::uniform_real_distribution<float>(5.f, 100.f);
	auto wanderOffsetDist = std::uniform_real_distribution<float>(5.f, 100.f);
	auto wanderRadiusDist = std::uniform_real_distribution<float>(5.f, 100.f);
	auto wanderRateDist = std::uniform_real_distribution<float>(5.f, 100.f);
	auto maxLinearAccelerationDist = std::uniform_real_distribution<float>(5.f, 50.f);
	m_wanderBehaviour = std::make_unique<Wander>(&m_kinematic, maxAngularAccelerationDist(mt), maxRotationDist(mt), 0.1f, 10, 0.1f, wanderOffsetDist(mt), wanderRadiusDist(mt), wanderRateDist(mt), maxLinearAccelerationDist(mt));
}

Asteroid::~Asteroid()
{
}

void Asteroid::InitializeRenderable(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::uniform_real_distribution<float>& verticesDistribution)
{
	createConvexHullWithJarvisMarch(verticesDistribution);
	initializeCollider();

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
	simpleWanderAlgorithm(deltaTime);
	updateRandomRotation(deltaTime);
	updateBoundingBoxesTransforms(m_transform.Matrices.Scale, m_transform.Matrices.Rotation, m_transform.Matrices.Translation);
}

void Asteroid::Render(ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	deviceContext->RSSetState(m_states->CullNone());

	deviceContext->IASetInputLayout(m_inputLayout.Get());

	m_effect->SetMatrices(m_transform.Matrices.CalculateWorld(), camera.GetView(), camera.GetProj());
	m_effect->Apply(deviceContext);

	m_batch->Begin();

	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, &m_vertices[0], m_vertices.size());

	m_batch->End();
}

const std::vector<DirectX::SimpleMath::Vector2>& Asteroid::GetVertices() const
{
	return m_verticesPositions;
}

void Asteroid::updateRandomRotation(const float& deltaTime)
{
	m_timeSinceLastMovementSpeedAndRotationChange += deltaTime;
	if(m_timeSinceLastMovementSpeedAndRotationChange >= m_randomMovementSpeedAndRotationChangeTimeThreshold)
	{
		m_randomRotationSpeed = m_randomRotationAngleDistribution(m_mersenneTwisterEngine);
		m_timeSinceLastMovementSpeedAndRotationChange = 0.f;
	}
	rotate(m_randomRotationSpeed, deltaTime);
}

void Asteroid::createConvexHullWithJarvisMarch(const std::uniform_real_distribution<float>& verticesDistribution)
{
	std::vector<Vector2> randomVertices;
	size_t randomVerticesCount = 3;

	for (size_t i = 0; i < randomVerticesCount; ++i)
		randomVertices.push_back(Vector2(verticesDistribution(m_mersenneTwisterEngine), verticesDistribution(m_mersenneTwisterEngine)));

	JarvisMarch jarvisMarch;
	auto hull = jarvisMarch.GetConvexHull(randomVertices);
	for (size_t i = 0; i < hull.size(); ++i)
		m_vertices.push_back(VertexPositionColor(Vector3(hull[i].x, hull[i].y, 0.f), Colors::DarkGray));
	m_vertices.push_back(VertexPositionColor(Vector3(hull[0].x, hull[0].y, 0.f), Colors::DarkGray));

	m_verticesPositions = hull;
}

void Asteroid::simpleWanderAlgorithm(const float& deltaTime)
{
	m_kinematic.Position = m_transform.Position;
	
	SteeringOutput steeringOutput;
	m_wanderBehaviour->GetSteering(&steeringOutput);
	steeringOutput.Linear.z = 0.f;

	m_kinematic.Integrate(steeringOutput, deltaTime);
	m_kinematic.TrimMaxSpeed(25);

	SetPosition(m_kinematic.Position);
}
