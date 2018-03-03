#include <utils/RenderTexture.hpp>
#include <assert.h>

namespace eva
{
	RenderTexture::RenderTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext) : m_device(device), m_deviceContext(deviceContext), 
																							   m_width(0), m_height(0)
	{
	}

	void RenderTexture::CreateRenderTarget(UINT width, UINT height, bool multiSampling)
	{
		m_width = width;
		m_height = height;
		m_multiSampling = multiSampling;

		//Reset resources
		m_renderTargetTexture.Reset();
		m_depthStencilTexture.Reset();
		m_renderTargetView.Reset();
		m_depthStencilView.Reset();
		m_shaderResourceView.Reset();

		//Create resources
		CreateRenderTargetView();
		CreateDepthStencilView();
		CreateViewport();
	}

	void RenderTexture::CreateRenderTargetView()
	{
		//Fill in the texture description
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		textureDesc.Width = m_width;
		textureDesc.Height = m_height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		m_multiSampling ? textureDesc.SampleDesc.Count = 4 : textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		//Create the texture
		assert(!m_device->CreateTexture2D(&textureDesc, nullptr, m_renderTargetTexture.GetAddressOf()));

		//Fill in the render target view description
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = textureDesc.Format;
		m_multiSampling ? renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS : renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		//Create the render target view with the texture
		assert(!m_device->CreateRenderTargetView(m_renderTargetTexture.Get(), &renderTargetViewDesc, m_renderTargetView.GetAddressOf()));

		//Fill in the shader resource view description
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		m_multiSampling ? shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS : shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//Create shader resource view which can now be used as a texture
		assert(!m_device->CreateShaderResourceView(m_renderTargetTexture.Get(), &shaderResourceViewDesc, m_shaderResourceView.GetAddressOf()));
	}

	void RenderTexture::CreateDepthStencilView()
	{
		//Fill in description of the depth stencil texture
		D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
		depthStencilDesc.Width = m_width;
		depthStencilDesc.Height = m_height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		m_multiSampling ? depthStencilDesc.SampleDesc.Count = 4 : depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		//Create texture
		assert(!m_device->CreateTexture2D(&depthStencilDesc, NULL, m_depthStencilTexture.GetAddressOf()));

		//Set up the depth stencil view description
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		m_multiSampling ? depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS : depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//Create the depth stencil view from the texture and description
		assert(!m_device->CreateDepthStencilView(m_depthStencilTexture.Get(), &depthStencilViewDesc, m_depthStencilView.GetAddressOf()));
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
		m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
		m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
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