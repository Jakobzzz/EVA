/*
	Resource handler class for all kinds of shaders
*/

#pragma once
#include <d3d11.h>
#include <map>
#include <vector>
#include <string>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace eva
{
	namespace Shaders
	{
		enum ID
		{
			BASIC
		};
	}

	enum ShaderType
	{
		VS = 0,
		PS = 0x1
	};

	DEFINE_ENUM_FLAG_OPERATORS(ShaderType);

	enum Topology
	{
		UNDEFINED = 0,
		POINTS = 1,
		LINES = 2,
		LINE_STRIP = 3,
		TRIANGLES = 4,
		TRIANGLE_STRIP = 5,
		TRIANGLE_FAN = 12,
	};

	class Shader
	{
	public:
		Shader() = default;
		Shader(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		~Shader() = default;

	private:
		struct ShaderData
		{
			ShaderType type;
			std::vector<ComPtr<ID3DBlob>> blobs;
			ComPtr<ID3D11VertexShader> vertexShader = nullptr;
			ComPtr<ID3D11PixelShader> pixelShader = nullptr;
			ComPtr<ID3D11InputLayout> layout = nullptr;
		};

	public:
		void LoadShadersFromFile(const Shaders::ID & id, const std::string & shaderPath, ShaderType type);
		void CreateInputLayout(const Shaders::ID & id);
		void SetShaders(const Shaders::ID & id, Topology topology);
		void Unbind(const Shaders::ID & id);

	private:
		std::map<Shaders::ID, ShaderData> m_shaders;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
	};
}
