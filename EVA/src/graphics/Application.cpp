#include <graphics/Application.hpp>
#include <graphics/Model.hpp>
#include <utils/D3DUtility.hpp>
#include <utils/Shader.hpp>
#include <utils/Buffer.hpp>
#include <utils/RenderTexture.hpp>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_dock.h>
#include <tchar.h>
#include <DirectXColors.h>

#define WIN32_LEAN_AND_MEAN //Exclude some windows headers

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace eva
{
	ComPtr<ID3D11Device> Application::m_device;
	ComPtr<ID3D11DeviceContext> Application::m_deviceContext;
	ComPtr<IDXGISwapChain> Application::m_swapChain;
	ComPtr<ID3D11RenderTargetView> Application::m_mainRenderTargetView;

	Application::Application()
	{
	}

	Application::~Application() = default;

	void Application::LoadShaders()
	{
		m_shaders->LoadShadersFromFile(Shaders::Basic, "src/res/shaders/Primitive.hlsl", VS | PS);
	}

	void Application::CreateObjects()
	{
		m_shaders = std::make_unique<Shader>(m_device.Get(), m_deviceContext.Get());
		m_buffer = std::make_unique<Buffer>(m_device.Get(), m_deviceContext.Get());
		m_sceneTexture = std::make_unique<RenderTexture>(m_device.Get(), m_deviceContext.Get());
		m_model = std::make_unique<Model>(m_buffer.get(), m_shaders.get());
	}

	void Application::Initialize()
	{
		//Create application window
		m_windowClass = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("EVA"), NULL };
		RegisterClassEx(&m_windowClass);
		HWND hwnd = CreateWindow(_T("EVA"), _T("EVA engine - DirectX11.0"), WS_OVERLAPPEDWINDOW, 150, 75, WIDTH, HEIGHT, NULL, NULL, m_windowClass.hInstance, NULL);

		//Initialize Direct3D
		CreateDeviceD3D(hwnd);

		//Show the window
		ShowWindow(hwnd, SW_SHOWDEFAULT);
		UpdateWindow(hwnd);

		//Setup ImGui binding
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplDX11_Init(hwnd, m_device.Get(), m_deviceContext.Get());
		//io.NavFlags |= ImGuiNavFlags_EnableKeyboard;  //Enable Keyboard Controls

		//Setup style and reload saved dock settings
		ImGui::StyleColorsDark();
		ImGui::InitDock();

		//Load resources
		CreateObjects();
		LoadShaders();

		m_model->CreateBuffers();
	}

	void Application::Run()
	{
		Update();
	}

	void Application::Update()
	{
		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}

			ImGui_ImplDX11_NewFrame();
			UpdateEditor();
			RenderMainWindow();
		}
	}

	void Application::RenderMainWindow()
	{
		m_deviceContext->OMSetRenderTargets(1, m_mainRenderTargetView.GetAddressOf(), nullptr);
		m_deviceContext->ClearRenderTargetView(m_mainRenderTargetView.Get(), DirectX::Colors::Black);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		assert(!m_swapChain->Present(1, 0)); //Present with vsync (60 Hz)
	}

	void Application::RenderScene()
	{
		m_sceneTexture->SetRenderTarget(DirectX::Colors::Gray);
		m_model->Draw();
	}

	void Application::ShutDown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();

		m_mainRenderTargetView.Reset();
		m_swapChain.Reset();
		m_deviceContext.Reset();
		m_device.Reset();
		UnregisterClass(_T("EVA"), m_windowClass.hInstance);
	}

	void Application::UpdateEditor()
	{
		//Menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{

				}

				if (ImGui::MenuItem("Open"))
				{

				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		//Begin docking
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		const ImGuiWindowFlags flags = (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoTitleBar);
		const float oldWindowRounding = ImGui::GetStyle().WindowRounding; ImGui::GetStyle().WindowRounding = 0;
		const bool visible = ImGui::Begin("Docking system", nullptr, ImVec2(0, 0), 1.0f, flags);
		ImGui::GetStyle().WindowRounding = oldWindowRounding;
		ImGui::SetWindowPos(ImVec2(0, 10));

		if (visible)
		{
			//Warning: If you remove or rename a dock make sure to
			//check the imgui.ini file and remove/rename entry
			ImGui::BeginDockspace();

			if (ImGui::BeginDock("Scene")) 
			{
				CreateSceneDock();
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Inspector")) 
			{
				ImGui::Text("I'm BentleyBlanks!");
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Log")) 
			{
				ImGui::Text("I'm LonelyWaiting!");
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Test")) 
			{
				ImGui::Text("I'm LonelyWaiting!");
			}
			ImGui::EndDock();

			ImGui::EndDockspace();
		}
		ImGui::End();
	}

	void Application::CreateSceneDock()
	{
		//m_isSceneHovered = ImGui::IsItemHovered();
		ImVec2 size = ImGui::GetContentRegionAvail();
		UINT width = m_sceneTexture->GetWidth();
		UINT height = m_sceneTexture->GetHeight();

		//Resize the render texture
		//Note: resizing the scene during runtime (very) frequently will likely cause a crash due to memory allocations
		if (width != size.x || height != size.y)
			m_sceneTexture->CreateRenderTarget(static_cast<UINT>(size.x), static_cast<UINT>(size.y));

		RenderScene();
		ImGui::Image(m_sceneTexture->GetShaderResourceView(), size);
	}

	void Application::CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		m_device->CreateRenderTargetView(pBackBuffer, nullptr, m_mainRenderTargetView.GetAddressOf());
		SAFE_RELEASE(&pBackBuffer);
	}

	void Application::CreateDeviceD3D(HWND hWnd)
	{
		//Setup swap chain
		DXGI_SWAP_CHAIN_DESC sd = { 0 };
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		assert(!D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, ARRAYSIZE(featureLevelArray),
				D3D11_SDK_VERSION, &sd, m_swapChain.GetAddressOf(), m_device.GetAddressOf(), &featureLevel, m_deviceContext.GetAddressOf()));

		CreateRenderTarget();
	}

	LRESULT Application::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
			if (m_device.Get() != nullptr && wParam != SIZE_MINIMIZED)
			{
				ImGui_ImplDX11_InvalidateDeviceObjects();
				m_mainRenderTargetView.Reset();
				m_swapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				CreateRenderTarget();
				ImGui_ImplDX11_CreateDeviceObjects();
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) //Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}