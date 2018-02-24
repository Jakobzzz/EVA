/*
	Assembly class of the engine
*/

#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

namespace eva
{
	class Application
	{
	public:
		Application();
		void Initialize();
		void Run();
		void ShutDown();

	private:
		void Update();
		void UpdateEditor();
		void Render(const FLOAT* color);
		void CreateDeviceD3D(HWND hWnd);

	private:
		static void CreateRenderTarget();
		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		WNDCLASSEX m_windowClass;
	};
}