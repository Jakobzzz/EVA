#include <utils/RenderTexture.hpp>
#include <assert.h>

namespace eva
{
	RenderTexture::RenderTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext) : m_device(device), m_deviceContext(deviceContext), 
																							   m_width(0), m_height(0)
	{
	}

	void RenderTexture::CreateRenderTarget(UINT width, UINT height)
	{
		m_width = width;
		m_height = height;

		//Reset resources
		m_renderTargetTexture.Reset();
		m_renderTargetView.Reset();
		m_shaderResourceView.Reset();

		//Fill in the texture description
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		textureDesc.Width = m_width;
		textureDesc.Height = m_height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		//Create the texture
		assert(!m_device->CreateTexture2D(&textureDesc, nullptr, m_renderTargetTexture.GetAddressOf()));

		//Fill in the render target view description
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		//Create the render target view with the texture
		assert(!m_device->CreateRenderTargetView(m_renderTargetTexture.Get(), &renderTargetViewDesc, m_renderTargetView.GetAddressOf()));

		//Fill in the shader resource view description
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//Create shader resource view which can now be used as a texture
		assert(!m_device->CreateShaderResourceView(m_renderTargetTexture.Get(), &shaderResourceViewDesc, m_shaderResourceView.GetAddressOf()));
	
		CreateViewport();
	}

	void RenderTexture::CreateViewport()
	{
		m_vp.Width = static_cast<FLOAT>(m_width);
		m_vp.Height = static_cast<FLOAT>(m_height);
		m_vp.MinDepth = 0.0f;
		m_vp.MaxDepth = 1.0f;
		m_vp.TopLeftX = 0;
		m_vp.TopLeftY = 0;
	}

	void RenderTexture::SetRenderTarget(const FLOAT * color)
	{
		m_deviceContext->RSSetViewports(1, &m_vp);
		m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
		m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
	}

	ID3D11ShaderResourceView * RenderTexture::GetShaderResourceView() const
	{
		return m_shaderResourceView.Get();
	}

	UINT RenderTexture::GetWidth() const
	{
		return m_width;
	}

	UINT RenderTexture::GetHeight() const
	{
		return m_height;
	}
}