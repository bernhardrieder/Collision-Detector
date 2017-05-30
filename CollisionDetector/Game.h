//
// Game.h
//

#pragma once

#include "StepTimer.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

	float AspectRatio() const;
private:

    void Update(DX::StepTimer const& timer);
	void checkAndProcessKeyboardInput(const float& deltaTime);
	void checkAndProcessMouseInput(const float& deltaTime);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
	void initializeDeviceDependentObjects();
    void CreateResources();
	void initializeWindowSizeDependentObjects();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
	
    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
	Camera m_camera;
	Player m_player;
	const static size_t m_numOfAsteroids = 100;
	Asteroid m_asteroids[m_numOfAsteroids];
	CollisionDetection::CollisionDetector2D m_collisionDetector;
	CollisionVisualizer m_collisionVisualizer;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
};