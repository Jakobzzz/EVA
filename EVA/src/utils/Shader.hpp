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
			Basic
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
	private:
		struct ShaderData
		{
			std::vector<ComPtr<ID3DBlob>> blobs;
			ComPtr<ID3D11VertexShader> vertexShader;
			ComPtr<ID3D11PixelShader> pixelShader;
			ComPtr<ID3D11InputLayout> layout;
			ShaderType type;
		};

	public:
		void LoadShadersFromFile(const Shaders::ID & id, const std::string & shaderPath, ShaderType type);
		void CreateInputLayout(const Shaders::ID & id);
		void SetShaders(const Shaders::ID & id, Topology topology);
		void Unbind(const Shaders::ID & id);

	private:
		std::map<Shaders::ID, ShaderData> m_shaders;
	};
}
