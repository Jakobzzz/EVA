/*
	Assembly class of the engine
*/

#pragma once
#include <wrl.h>
#include <memory>
#include <d3d11.h>

using namespace Microsoft::WRL;

namespace eva
{
	class Shader;
	class Buffer;
	class Model;
	class Camera;

	class Application
	{
	public:
		Application();
		~Application();
		void Initialize();
		void Run();
		void ShutDown();

	private:
		void LoadShaders();
		void Update();
		void PollEvents();
		void RenderMainWindow();
		void RenderScene();
		void CreateObjects();
		void CreateDeviceD3D(HWND hWnd);
		void CreateViewport();

	private:
		void CreateRenderTargetAndDepthStencil();
		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		std::unique_ptr<Shader> m_shaders;
		std::unique_ptr<Buffer> m_buffer;
		std::unique_ptr<Model> m_model;
		std::unique_ptr<Camera> m_camera;

	private:
		static ComPtr<ID3D11Device> m_device;
		static ComPtr<ID3D11DeviceContext> m_deviceContext;

	private:
		D3D11_VIEWPORT m_vp;
		WNDCLASSEX m_windowClass;
		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID3D11RenderTargetView> m_mainRenderTargetView;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	};
}