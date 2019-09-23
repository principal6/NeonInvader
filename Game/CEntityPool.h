#pragma once

#include "CEntity.h"

class CEntityPool
{
public:
	CEntityPool(CDirectX* DirectX) : m_DirectX{ DirectX } {};
	~CEntityPool() {};

	CEntity* AddEntity();
	CEntity* GetEntity(size_t Index);

	void ApplyPhysics(float DeltaTime);
	void DrawEntitiesInAddedOrder();

private:
	CDirectX*						m_DirectX{};

	vector<unique_ptr<CEntity>>		m_vEntities{};
};