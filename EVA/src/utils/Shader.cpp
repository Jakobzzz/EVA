#include <utils/Shader.hpp>
#include <utils/D3DUtility.hpp>
#include <assert.h>
#include <d3dcompiler.h>

namespace eva
{
	Shader::Shader(ID3D11Device* device, ID3D11DeviceContext* deviceContext) : m_device(device), m_deviceContext(deviceContext)
	{
	}

	void Shader::LoadShadersFromFile(const Shaders::ID & id, const std::string & shaderPath, ShaderType type)
	{
		ShaderData data;
		data.type = type;

		//Compile and create depending on shader type
		if (data.type == (VS | PS))
		{
			data.blobs.resize(2);
			assert(!D3DCompileFromFile(ToWChar(shaderPath).c_str(), nullptr, nullptr, "VS_MAIN", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0, data.blobs[0].GetAddressOf(), nullptr));
			assert(!D3DCompileFromFile(ToWChar(shaderPath).c_str(), nullptr, nullptr, "PS_MAIN", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0, data.blobs[1].GetAddressOf(), nullptr));
			assert(!m_device->CreateVertexShader(data.blobs[0]->GetBufferPointer(), data.blobs[0]->GetBufferSize(), nullptr, data.vertexShader.GetAddressOf()));
			assert(!m_device->CreatePixelShader(data.blobs[1]->GetBufferPointer(), data.blobs[1]->GetBufferSize(), nullptr, data.pixelShader.GetAddressOf()));
		}

		auto inserted = m_shaders.insert(std::make_pair(id, std::move(data)));
		assert(inserted.second);
	}

	void Shader::CreateInputLayout(const Shaders::ID & id)
	{
		auto found = m_shaders.find(id);

		//Hardcoded layout for now
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc;

		if (id == Shaders::ID::BASIC)
		{
			inputElementDesc =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
		}

		//Create the input layout for the vertex shader
		m_device->CreateInputLayout(&inputElementDesc[0], (UINT)inputElementDesc.size(), found->second.blobs[0]->GetBufferPointer(),
									found->second.blobs[0]->GetBufferSize(), found->second.layout.GetAddressOf());

		//Release blobs
		found->second.blobs[0].Reset();
		found->second.blobs[1].Reset();
	}

	void Shader::SetShaders(const Shaders::ID & id, Topology topology)
	{
		auto found = m_shaders.find(id);

		if (found->second.type == (VS | PS))
		{
			m_deviceContext->VSSetShader(found->second.vertexShader.Get(), nullptr, 0);
			m_deviceContext->PSSetShader(found->second.pixelShader.Get(), nullptr, 0);
			m_deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology);
			m_deviceContext->IASetInputLayout(found->second.layout.Get());
		}
	}

	void Shader::Unbind(const Shaders::ID & id)
	{
		auto found = m_shaders.find(id);

		if (found->second.type == (VS | PS))
		{
			m_deviceContext->VSSetShader(nullptr, nullptr, 0);
			m_deviceContext->PSSetShader(nullptr, nullptr, 0);
		}
	}
}