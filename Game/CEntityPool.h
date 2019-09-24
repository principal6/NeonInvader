#pragma once

#include "CEntity.h"

class CEntityPool
{
public:
	CEntityPool(CDirectX* DirectX) : m_DirectX{ DirectX } {};
	~CEntityPool() {};

	CEntity* AddEntity();
	CEntity* GetEntity(size_t Index);
	void SetMainSpriteEntity(CEntity* PtrEntity);
	void AddMainSpriteShotEntity(CEntity* PtrEntityShot);
	void AddEnemyEntity(CEntity* PtrEntityEnemy);

	void ApplyPhysics(float DeltaTime);
	void DrawEntitiesInAddedOrder();

public:
	bool FineCollision{ false };

private:
	void DetectCoarseCollision();
	void DetectFineCollision();
	
	bool IsPointAInsideBoxB(const XMFLOAT2& PointA, CEntity* EntityB);

private:
	CDirectX*							m_DirectX{};

	CEntity*							m_EntityMainSprite{};
	vector<CEntity*>					m_vEntityMainSpriteShots{};
	vector<CEntity*>					m_vEntityEnemies{};
	vector<unique_ptr<CEntity>>			m_vEntities{};
	vector<pair<CEntity*, CEntity*>>	m_vFineCollisionPairs{};
};