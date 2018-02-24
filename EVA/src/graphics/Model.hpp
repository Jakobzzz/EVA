/*
	Basic class to test functionality and render a triangle
*/

#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11Buffer;

namespace eva
{
	class Buffer;
	class Shader;

	class Model
	{
	public:
		Model() = default;
		Model(Buffer* buffer, Shader* shader);
		~Model() = default;
		void CreateBuffers();
		void Draw();

	private:
		Buffer * m_buffer;
		Shader * m_shader;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
	};
}
