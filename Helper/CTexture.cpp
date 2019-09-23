#include "CTexture.h"

void CTexture::CreateTexture(const string& TextureFileName)
{
	assert(!m_ShaderResourceView);
	assert(!m_Resource);
	
	wstring wfn{ TextureFileName.begin(), TextureFileName.end() };
	if (wfn.substr(wfn.size() - 3) == L"dds")
	{
		assert(SUCCEEDED(CreateDDSTextureFromFile(m_Device, wfn.c_str(), &m_Resource, &m_ShaderResourceView, 0)));
	}
	else
	{
		assert(SUCCEEDED(CreateWICTextureFromFile(m_Device, wfn.c_str(), &m_Resource, &m_ShaderResourceView, 0)));
	}

	auto texture{ static_cast<ID3D11Texture2D*>(m_Resource.Get()) };
	D3D11_TEXTURE2D_DESC texture_desc{};
	texture->GetDesc(&texture_desc);
	m_TextureSize.x = static_cast<float>(texture_desc.Width);
	m_TextureSize.y = static_cast<float>(texture_desc.Height);
}

void CTexture::Use()
{
	m_DeviceContext->PSSetShaderResources(0, 1, m_ShaderResourceView.GetAddressOf());
}