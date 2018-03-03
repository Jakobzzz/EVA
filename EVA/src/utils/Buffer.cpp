#include <utils/Buffer.hpp>
#include <assert.h>
#include <utils/D3DUtility.hpp>

namespace eva
{
	Buffer::Buffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext) : m_device(device), m_deviceContext(deviceContext)
	{
	}

	void Buffer::CreateVertexBuffer(const void * data, UINT size, UINT stride,
									ID3D11Buffer ** buffer, D3D11_CPU_ACCESS_FLAG flag, D3D11_USAGE usage)
	{
		CreateBuffer(data, size, stride, buffer, D3D11_BIND_VERTEX_BUFFER, flag, usage);
	}

	void Buffer::CreateIndexBuffer(const void * data, UINT size, UINT stride, 
									ID3D11Buffer ** buffer, D3D11_CPU_ACCESS_FLAG flag, D3D11_USAGE usage)
	{
		CreateBuffer(data, size, stride, buffer, D3D11_BIND_INDEX_BUFFER, flag, usage);
	}

	void Buffer::CreateConstantBuffer(const void * data, UINT size, UINT stride, 
									  ID3D11Buffer ** buffer, D3D11_CPU_ACCESS_FLAG flag, D3D11_USAGE usage)
	{
		CreateBuffer(data, size, stride, buffer, D3D11_BIND_CONSTANT_BUFFER, flag, usage);
	}

	void Buffer::SetVertexBuffer(ID3D11Buffer ** buffer, UINT stride)
	{
		UINT offset = 0;
		m_deviceContext->IASetVertexBuffers(0, 1, buffer, &stride, &offset);
	}

	void Buffer::SetIndexBuffer(ID3D11Buffer ** buffer)
	{
		m_deviceContext->IASetIndexBuffer(buffer[0], DXGI_FORMAT_R32_UINT, 0);
	}

	void Buffer::SetConstantBuffer(UINT shaderRegister, ID3D11Buffer ** buffer, ShaderType type)
	{
		//Note: Bit flag might change this up a bit
		if(type == VS)
			m_deviceContext->VSSetConstantBuffers(shaderRegister, 1, buffer);
		else if (type == PS)
			m_deviceContext->PSSetConstantBuffers(shaderRegister, 1, buffer);
	}

	void Buffer::Draw(UINT vertexCount)
	{
		m_deviceContext->Draw(vertexCount, 0);
	}

	void Buffer::DrawIndexed(UINT indexCount)
	{
		m_deviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void Buffer::UpdateConstantBuffer(const void * data, ID3D11Buffer ** buffer)
	{
		m_deviceContext->UpdateSubresource(buffer[0], 0, nullptr, data, 0, 0);
	}

	void Buffer::CreateBuffer(const void * data, UINT size, UINT stride, 
							   ID3D11Buffer ** buffer, D3D11_BIND_FLAG bindFlag, D3D11_CPU_ACCESS_FLAG flag, D3D11_USAGE usage)
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
		assert(!m_device->CreateBuffer(&bufferDesc, &subData, buffer));
	}
}