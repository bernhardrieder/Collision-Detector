#include "pch.h"
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player()
{
}


Player::~Player()
{
}

void Player::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
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

void Player::Update(const float& deltaTime, const Keyboard* keyboard)
{
	checkAndProcessKeyboardInput(keyboard, deltaTime);
}

void Player::Render(ID3D11DeviceContext* deviceContext, const Camera& camera)
{
	deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	deviceContext->RSSetState(m_states->CullNone());

	m_effect->Apply(deviceContext);

	deviceContext->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();

	VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Red);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Red);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Red);
	m_effect->SetMatrices(Matrix::CreateScale(m_transform.Scale)*Matrix::CreateRotationZ(m_transform.RotationAngle)*Matrix::CreateTranslation(m_transform.Position), camera.GetView(), camera.GetProj());

	m_batch->DrawTriangle(v1, v2, v3);

	m_batch->End();
}

void Player::AttachFollowingCamera(Camera* camera)
{
	m_followingCamera = camera;
}

void Player::SetPosition(const Vector3& newPosition)
{
	SimpleMovable2D::SetPosition(newPosition);
	if (m_followingCamera)
		m_followingCamera->SetPosition(newPosition);
}

void Player::checkAndProcessKeyboardInput(const Keyboard* keyboard, float deltaTime)
{
	auto kb = keyboard->GetState();

	if (kb.A || kb.Left)
		rotateLeft(deltaTime);
	else if (kb.D || kb.Right)
		rotateRight(deltaTime);
	if (kb.W || kb.Up)
		moveForward(deltaTime);
	else if (kb.S || kb.Down)
		moveBackwards(deltaTime);

	if (!kb.IsKeyUp(Keyboard::Keys::LeftShift))
		m_movementSpeedUpFactor = 5;
	else
		m_movementSpeedUpFactor = 1;
}

void Player::moveForward(const float& deltaTime)
{
	move(m_moveDirection, m_movementSpeed*m_movementSpeedUpFactor, deltaTime);
}

void Player::moveBackwards(const float& deltaTime)
{
	move(-m_moveDirection, m_movementSpeed*m_movementSpeedUpFactor, deltaTime);
}