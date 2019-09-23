#pragma once

#include "../Helper/CTexture.h"

class CTexturePool
{
public:
	CTexturePool(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) : m_Device{ Device }, m_DeviceContext{ DeviceContext } {}
	~CTexturePool() {}

	CTexture* AddSharedTexture(const string& TextureFileName);
	CTexture* GetSharedTexture(size_t Index);

private:
	ID3D11Device*					m_Device{};
	ID3D11DeviceContext*			m_DeviceContext{};

	vector<unique_ptr<CTexture>>	m_vSharedTextures{};
};