#include "CEntityPool.h"

CEntity* CEntityPool::AddEntity()
{
	m_vEntities.emplace_back(make_unique<CEntity>(m_DirectX->m_Device.Get(), m_DirectX->m_DeviceContext.Get()));
	return m_vEntities.back().get();
}

CEntity* CEntityPool::GetEntity(size_t Index)
{
	return m_vEntities[Index].get();
}

CTexture* CEntityPool::AddSharedTexture(const string& TextureFileName)
{
	m_vSharedTextures.emplace_back(make_unique<CTexture>(m_DirectX->m_Device.Get(), m_DirectX->m_DeviceContext.Get()));

	m_vSharedTextures.back()->CreateTexture(TextureFileName);

	return m_vSharedTextures.back().get();
}

CTexture* CEntityPool::GetSharedTexture(size_t Index)
{
	return m_vSharedTextures[Index].get();
}

void CEntityPool::ApplyPhysics(float DeltaTime)
{
	for (auto& i : m_vEntities)
	{
		if (!i->Visible) continue;

		i->WorldPosition.x += i->LinearVelocity.x * DeltaTime;
		i->WorldPosition.y += i->LinearVelocity.y * DeltaTime;
	}
}

void CEntityPool::DrawEntities()
{
	for (auto& i : m_vEntities)
	{
		if (!i->Visible) continue;

		m_DirectX->UseSampler(i->Sampler);

		XMMATRIX Translation{ XMMatrixTranspose(XMMatrixTranslation(i->WorldPosition.x, i->WorldPosition.y, 0.0f)) };
		XMMATRIX Rotation{ XMMatrixTranspose(XMMatrixRotationZ(i->RotationAngle)) };
		XMMATRIX Scaling{ XMMatrixTranspose(XMMatrixScaling(i->Scalar.x, i->Scalar.y, 1.0f)) };
		i->m_MatrixWorld = Translation * Rotation * Scaling;

		m_DirectX->m_CBWorld->Update(&i->m_MatrixWorld);

		i->Draw();
	}
}