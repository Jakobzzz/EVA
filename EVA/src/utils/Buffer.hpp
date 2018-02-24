/*
	Buffer utility class for resource handling
*/

#pragma once
#include <d3d11.h>
#include <assert.h>
#include <utils/D3DUtility.hpp>

namespace eva
{
	class Buffer
	{
	public:
		static void CreateVertexBuffer(const void* data, UINT size, UINT stride, ID3D11Buffer** buffer,
										D3D11_CPU_ACCESS_FLAG flag = D3D11_CPU_ACCESS_READ, D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
		{
			CreateBuffer(data, size, stride, buffer, D3D11_BIND_VERTEX_BUFFER, flag, usage);
		}

		static void CreateIndexBuffer(const void* data, UINT size, UINT stride, ID3D11Buffer** buffer,
										D3D11_CPU_ACCESS_FLAG flag = D3D11_CPU_ACCESS_READ, D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
		{
			CreateBuffer(data, size, stride, buffer, D3D11_BIND_INDEX_BUFFER, flag, usage);
		}

	public:
		static void SetVertexBuffer(ID3D11Buffer** buffer, UINT stride)
		{
			g_deviceContext->IASetVertexBuffers(0, 1, buffer, &stride, 0); //&[0]?
		}

		static void SetIndexBuffer(ID3D11Buffer** buffer)
		{
			g_deviceContext->IASetIndexBuffer(buffer[0], DXGI_FORMAT_R32_UINT, 0);
		}

	private:
		static void CreateBuffer(const void* data, UINT size, UINT stride, ID3D11Buffer** buffer, D3D11_BIND_FLAG bindFlag,
									D3D11_CPU_ACCESS_FLAG flag = D3D11_CPU_ACCESS_READ, D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
		{
			//Fill in buffer description for buffer
			D3D11_BUFFER_DESC bufferDesc = { 0 };
			bufferDesc.BindFlags = bindFlag;
			bufferDesc.Usage = usage;
			bufferDesc.ByteWidth = stride * size;
			bufferDesc.CPUAccessFlags = flag;

			//Fill in subresource data
			D3D11_SUBRESOURCE_DATA subData = { 0 };
			subData.pSysMem = data;
			subData.SysMemPitch = size;
			subData.SysMemSlicePitch = subData.SysMemPitch;

			assert(!g_device->CreateBuffer(&bufferDesc, &subData, buffer));
		}
	};
}
