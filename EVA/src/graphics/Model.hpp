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
	class Camera;

	class Model
	{
	public:
		Model() = default;
		Model(Camera* camera, Buffer* buffer, Shader* shader);
		~Model() = default;
		void CreateBuffers();
		void Draw();

	private:
		Buffer * m_buffer;
		Shader * m_shader;
		Camera * m_camera;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_constantBuffer;
	};
}
