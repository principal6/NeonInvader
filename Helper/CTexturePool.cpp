#include "CTexturePool.h"

CTexture* CTexturePool::AddSharedTexture(const string& TextureFileName)
{
	m_vSharedTextures.emplace_back(make_unique<CTexture>(m_Device, m_DeviceContext));

	m_vSharedTextures.back()->CreateTexture(TextureFileName);

	return m_vSharedTextures.back().get();
}

CTexture* CTexturePool::GetSharedTexture(size_t Index)
{
	return m_vSharedTextures[Index].get();
}
