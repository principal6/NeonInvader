#pragma once

#include "CEntity.h"

class CEntityPool
{
public:
	CEntityPool(CDirectX* DirectX) : m_PtrDirectX{ DirectX } {};
	virtual ~CEntityPool() {};

	virtual CEntity* CreateEntity();
	virtual CEntity* GetEntity(size_t Index);

	virtual void ApplyPhysics(float DeltaTime);
	virtual void DrawEntitiesInAddedOrder();

public:
	bool FineCollisionDetected{ false };

protected:
	virtual void DetectCoarseCollision();
	virtual void DetectFineCollision();
	
	virtual bool IsPointAInsideBoxB(const XMFLOAT2& PointA, CEntity* EntityB);

protected:
	CDirectX*							m_PtrDirectX{};

	vector<unique_ptr<CEntity>>			m_vEntities{};
	vector<pair<CEntity*, CEntity*>>	m_vFineCollisionPairs{};
};