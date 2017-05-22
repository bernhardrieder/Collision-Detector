#include "pch.h"
#include "Asteroid.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Asteroid::Asteroid()
{
	m_mersenneTwisterEngine = std::mt19937_64(m_randomDevice());
	m_randomPositionDistribution = std::uniform_real_distribution<float>(-1000.f, 1000.f);
	m_randomMovementSpeedDistribution = std::uniform_real_distribution<float>(10.f, 30.f);
	m_randomRotationAngleDistribution = std::uniform_real_distribution<float>(-10.f, 10.f);
}


Asteroid::~Asteroid()
{
}

void Asteroid::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
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

void Asteroid::SetRandomValues()
{
	SetPosition(Vector3(m_randomPositionDistribution(m_mersenneTwisterEngine), m_randomPositionDistribution(m_mersenneTwisterEngine), 0.f));
	SetRotationAngle(m_randomRotationAngleDistribution(m_mersenneTwisterEngine));
	m_movementSpeed = m_randomMovementSpeedDistribution(m_mersenneTwisterEngine);
	SetScale(Vector3::One * 10);
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

	VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);
	auto& m = m_transform.Matrices;
	m_effect->SetMatrices(m.Scale*m.Rotation*m.Translation, camera.GetView(), camera.GetProj());

	m_batch->DrawTriangle(v1, v2, v3);

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
