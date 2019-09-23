#include "CEntityPool.h"

CEntity* CEntityPool::AddEntity()
{
	m_vEntities.emplace_back(make_unique<CEntity>(*m_DirectX));
	return m_vEntities.back().get();
}

CEntity* CEntityPool::GetEntity(size_t Index)
{
	return m_vEntities[Index].get();
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

void CEntityPool::DrawEntitiesInAddedOrder()
{
	for (auto& i : m_vEntities)
	{
		if (!i->Visible) continue;

		i->Draw();
	}
}