#pragma once
#include "SimpleMovable2D.h"

class Asteroid :
	public SimpleMovable2D
{
public:
	Asteroid();
	~Asteroid();

	void InitializeRenderable(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::uniform_real_distribution<float>& verticesDistribution);
	void InitializeTransform(const std::uniform_real_distribution<float>& positionDistribution, const std::uniform_real_distribution<float>& rotationAngleDistribution, const std::uniform_real_distribution<float>& scaleDistribution);
	void Update(const float& deltaTime);
	void Render(ID3D11DeviceContext* deviceContext, const Camera& camera);

private:
	void updateMovementSpeedAndRotationChange(const float& deltaTime);
	void createConvexHullWithJarvisMarch(const std::uniform_real_distribution<float>& verticesDistribution);
	void simpleWanderAlgorithm(const float& deltaTime);

	float m_timeSinceLastMovementSpeedAndRotationChange = 0.f;
	float m_randomMovementSpeedAndRotationChangeTimeThreshold = 1.f;

	//rendering
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::vector<DirectX::VertexPositionColor> m_vertices;

	//randomness
	std::random_device m_randomDevice;
	std::mt19937_64 m_mersenneTwisterEngine;
	std::uniform_real_distribution<float> m_randomRotationAngleDistribution;
	std::uniform_real_distribution<float> m_randomMovementSpeedDistribution;

};

