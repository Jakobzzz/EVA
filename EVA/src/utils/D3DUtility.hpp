/*
	Utility class for D3D functionality
*/

#pragma once
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

static ID3D11Device* g_device = nullptr;
static ID3D11DeviceContext* g_deviceContext = nullptr;
static IDXGISwapChain*  g_swapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

//Release COM-object
#define SAFE_RELEASE(arg) if(*arg != nullptr) { (*arg)->Release(); (*arg) = nullptr; } 

inline std::wstring ToWChar(const std::string & inputString)
{
	return std::wstring(inputString.begin(), inputString.end());
}
