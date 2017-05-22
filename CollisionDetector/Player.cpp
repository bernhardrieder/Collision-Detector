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

void Player::Update(float deltaTime)
{
	checkAndProcessKeyboardInput(deltaTime);
}

void Player::Render(ID3D11DeviceContext* deviceContext)
{
}

void Player::AttachFollowingCamera(Camera* camera)
{
	m_followingCamera = camera;
}

void Player::SetPosition(const DirectX::SimpleMath::Vector3& newPosition)
{
	m_position = newPosition;
}

void Player::SetRotationAngle(const float& newAngle)
{
	m_rotationAngle = newAngle;
}

void Player::SetMovementSpeed(const float& newMovementSpeed)
{
	m_movementSpeed = newMovementSpeed;
}

void Player::SetRotationSpeed(const float& newRotationSpeed)
{
	m_rotationSpeed = newRotationSpeed;
}

void Player::checkAndProcessKeyboardInput(float deltaTime)
{
	auto kb = m_keyboard->GetState();

	if (kb.A || kb.Left)
		rotateLeft(deltaTime); //player rotation left
	else if (kb.D || kb.Right)
		rotateRight(deltaTime); //player roation right
	if (kb.W || kb.Up)
		moveForward(deltaTime); //player forward
	else if (kb.S || kb.Down)
		moveBackwards(deltaTime); //player backwards
}

void Player::moveForward(const float& deltaTime)
{
	//todo: inspect Vector3::Up! -> ok for function?
	move(Vector3::Up, m_movementSpeed, deltaTime);
}

void Player::moveBackwards(const float& deltaTime)
{
	//todo: inspect Vector3::Down! -> ok for function?
	move(Vector3::Down, m_movementSpeed, deltaTime);
}

void Player::move(const Vector3& normalizedDirectionVector, const float& speed, const float& deltaTime)
{
	m_position += normalizedDirectionVector * speed * deltaTime;
}

void Player::rotateLeft(const float& deltaTime)
{
	rotate(-m_rotationSpeed, deltaTime);
}

void Player::rotateRight(const float& deltaTime)
{
	rotate(m_rotationSpeed, deltaTime);
}

void Player::rotate(const float& degrees, const float& deltaTime)
{
	m_rotationAngle += degrees * deltaTime;
}
