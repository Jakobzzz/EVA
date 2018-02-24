#include <graphics/Application.hpp>
#include <utils/D3DUtility.hpp>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_dock.h>
#include <d3d11.h>
#include <tchar.h>
#include <DirectXColors.h>

#define WIN32_LEAN_AND_MEAN //Exclude some windows headers

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace eva
{
	Application::Application()
	{
	}

	void Application::Initialize()
	{
		//Create application window
		WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("EVA"), NULL };
		RegisterClassEx(&wc);
		HWND hwnd = CreateWindow(_T("EVA"), _T("EVA engine - DirectX11.0"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

		//Initialize Direct3D
		CreateDeviceD3D(hwnd);

		//Show the window
		ShowWindow(hwnd, SW_SHOWDEFAULT);
		UpdateWindow(hwnd);

		//Setup ImGui binding
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplDX11_Init(hwnd, g_device, g_deviceContext);
		//io.NavFlags |= ImGuiNavFlags_EnableKeyboard;  //Enable Keyboard Controls

		//Setup style and reload saved dock settings
		ImGui::StyleColorsDark();
		ImGui::InitDock();
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
			Render(DirectX::Colors::Aquamarine);
		}
	}

	void Application::Render(const FLOAT* color)
	{
		g_deviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_deviceContext->ClearRenderTargetView(g_mainRenderTargetView, color);

		//Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_swapChain->Present(1, 0); //Present with vsync (60 Hz)
	}

	void Application::ShutDown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();

		SAFE_RELEASE(&g_mainRenderTargetView);
		SAFE_RELEASE(&g_swapChain);
		SAFE_RELEASE(&g_deviceContext);
		SAFE_RELEASE(&g_device);
	}

	void Application::UpdateEditor()
	{
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

			if (ImGui::BeginDock("Scene")) {
				ImGui::Text("I'm Wubugui!");
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Inspector")) {
				ImGui::Text("I'm BentleyBlanks!");
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Log")) {
				ImGui::Text("I'm LonelyWaiting!");
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Test")) {
				ImGui::Text("I'm LonelyWaiting!");
			}
			ImGui::EndDock();

			ImGui::EndDockspace();
		}
		ImGui::End();
	}

	void Application::CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		g_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		g_device->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
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
			D3D11_SDK_VERSION, &sd, &g_swapChain, &g_device, &featureLevel, &g_deviceContext));

		CreateRenderTarget();
	}

	LRESULT Application::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
			if (g_device != NULL && wParam != SIZE_MINIMIZED)
			{
				ImGui_ImplDX11_InvalidateDeviceObjects();
				SAFE_RELEASE(&g_mainRenderTargetView);
				g_swapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
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