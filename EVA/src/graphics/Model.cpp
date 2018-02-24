#include <graphics/Model.hpp>
#include <utils/Buffer.hpp>
#include <utils/Shader.hpp>
#include <DirectXMath.h>

using namespace DirectX;

namespace eva
{
	struct Vertex
	{
		XMFLOAT4 position;
	};

	Vertex vertices[] =
	{
		XMFLOAT4(0.0f, 0.5f, 0.5f, 1.f),
		XMFLOAT4(0.5f, -0.5f, 0.5f, 1.f),
		XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.f)
	};

	Model::Model(Buffer * buffer, Shader * shader) : m_buffer(buffer), m_shader(shader)
	{
	}

	void Model::CreateBuffers()
	{
		m_buffer->CreateVertexBuffer(vertices, ARRAYSIZE(vertices), sizeof(Vertex), m_vertexBuffer.GetAddressOf());
		m_shader->CreateInputLayout(Shaders::Basic);
	}

	void Model::Draw()
	{
		m_shader->SetShaders(Shaders::Basic, TRIANGLES);
		m_buffer->SetVertexBuffer(m_vertexBuffer.GetAddressOf(), sizeof(Vertex));
		m_buffer->Draw(ARRAYSIZE(vertices));
		m_shader->Unbind(Shaders::Basic);
	}
}