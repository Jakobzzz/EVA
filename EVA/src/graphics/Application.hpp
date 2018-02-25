/*
	Assembly class of the engine
*/

#pragma once
#include <wrl.h>
#include <memory>

using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace eva
{
	class Shader;
	class Buffer;
	class Model;
	class RenderTexture;

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
		void UpdateEditor();
		void RenderMainWindow();
		void RenderScene();
		void CreateObjects();
		void CreateDeviceD3D(HWND hWnd);
		void CreateRenderTarget();
		void CreateSceneDock();

	private:
		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		std::unique_ptr<Shader> m_shaders;
		std::unique_ptr<Buffer> m_buffer;
		std::unique_ptr<Model> m_model;
		std::unique_ptr<RenderTexture> m_sceneTexture;

	private:
		WNDCLASSEX m_windowClass;
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_deviceContext;
		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID3D11RenderTargetView> m_mainRenderTargetView;
	};
}