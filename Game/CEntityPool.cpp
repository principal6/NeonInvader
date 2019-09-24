#include "CEntityPool.h"

CEntity* CEntityPool::CreateEntity()
{
	m_vEntities.emplace_back(make_unique<CEntity>(m_DirectX));
	return m_vEntities.back().get();
}

CEntity* CEntityPool::GetEntity(size_t Index)
{
	return m_vEntities[Index].get();
}

void CEntityPool::SetMainSpriteEntity(CEntity* PtrEntity)
{
	assert(PtrEntity);

	m_EntityMainSprite = PtrEntity;
}

void CEntityPool::AddMainSpriteShotEntity(CEntity* PtrEntityShot)
{
	assert(PtrEntityShot);

	m_vEntityMainSpriteShots.emplace_back(PtrEntityShot);
}

void CEntityPool::AddEnemyShotEntity(CEntity* PtrEnemyShot)
{
	assert(PtrEnemyShot);

	m_vEntityEnemyShots.emplace_back(PtrEnemyShot);
}

void CEntityPool::AddEnemyEntity(CEntity* PtrEntityEnemy)
{
	assert(PtrEntityEnemy);

	m_vEntityEnemies.emplace_back(PtrEntityEnemy);
}

void CEntityPool::AddItemEntity(CEntity* PtrEntityItem)
{
	assert(PtrEntityItem);

	m_vEntityItems.emplace_back(PtrEntityItem);
}

void CEntityPool::ApplyPhysics(float DeltaTime)
{
	for (auto& i : m_vEntities)
	{
		if (!i->Visible) continue;

		i->WorldPosition.x += i->LinearVelocity.x * DeltaTime;
		i->WorldPosition.y += i->LinearVelocity.y * DeltaTime;
	}

	DetectCoarseCollision();

	DetectFineCollision();
}

void CEntityPool::DrawEntitiesInAddedOrder()
{
	for (auto& i : m_vEntities)
	{
		if (!i->Visible) continue;

		i->Draw();
	}
}

void CEntityPool::DetectCoarseCollision()
{
	m_vFineCollisionPairs.clear();

	m_EntityMainSprite->m_Collided = false;

	// Main sprite vs. Enemy sprites
	for (auto& enemy : m_vEntityEnemies)
	{
		enemy->m_Collided = false;

		if (m_EntityMainSprite->ShouldCollide && enemy->ShouldCollide && m_EntityMainSprite->Visible && enemy->Visible)
		{
			XMFLOAT2 diff{ m_EntityMainSprite->WorldPosition - enemy->WorldPosition };
			float distance{ XMFLOAT2GetLength(diff) };

			if (distance <= m_EntityMainSprite->m_CoarseCollisionRadius + enemy->m_CoarseCollisionRadius)
			{
				m_vFineCollisionPairs.emplace_back(m_EntityMainSprite, enemy);
			}
		}
	}

	// Main sprite vs. Enemy shots
	for (auto& enemy_shot : m_vEntityEnemyShots)
	{
		enemy_shot->m_Collided = false;

		if (enemy_shot->ShouldCollide && m_EntityMainSprite->ShouldCollide && enemy_shot->Visible && m_EntityMainSprite->Visible)
		{
			XMFLOAT2 diff{ enemy_shot->WorldPosition - m_EntityMainSprite->WorldPosition };
			float distance{ XMFLOAT2GetLength(diff) };

			if (distance <= enemy_shot->m_CoarseCollisionRadius + m_EntityMainSprite->m_CoarseCollisionRadius)
			{
				m_vFineCollisionPairs.emplace_back(enemy_shot, m_EntityMainSprite);
			}
		}
	}

	// Main sprite vs. Items
	for (auto& item : m_vEntityItems)
	{
		item->m_Collided = false;

		if (item->ShouldCollide && m_EntityMainSprite->ShouldCollide && item->Visible && m_EntityMainSprite->Visible)
		{
			XMFLOAT2 diff{ item->WorldPosition - m_EntityMainSprite->WorldPosition };
			float distance{ XMFLOAT2GetLength(diff) };

			if (distance <= item->m_CoarseCollisionRadius + m_EntityMainSprite->m_CoarseCollisionRadius)
			{
				m_vFineCollisionPairs.emplace_back(item, m_EntityMainSprite);
			}
		}
	}

	// Main sprite shots vs. Enemy sprites
	for (auto& main_sprite_shot : m_vEntityMainSpriteShots)
	{
		main_sprite_shot->m_Collided = false;

		for (auto& enemy : m_vEntityEnemies)
		{
			if (main_sprite_shot->ShouldCollide && enemy->ShouldCollide && main_sprite_shot->Visible && enemy->Visible)
			{
				XMFLOAT2 diff{ main_sprite_shot->WorldPosition - enemy->WorldPosition };
				float distance{ XMFLOAT2GetLength(diff) };

				if (distance <= main_sprite_shot->m_CoarseCollisionRadius + enemy->m_CoarseCollisionRadius)
				{
					m_vFineCollisionPairs.emplace_back(main_sprite_shot, enemy);
				}
			}
		}
	}
}

void CEntityPool::DetectFineCollision()
{
	FineCollision = false;

	for (auto& pair : m_vFineCollisionPairs)
	{
		CEntity* a{ pair.first };
		CEntity* b{ pair.second };

		float dist_ab{ XMFLOAT2GetLength(a->WorldPosition - b->WorldPosition) };
		if (dist_ab <= a->m_FineCollisionRadius + b->m_FineCollisionRadius)
		{
			FineCollision = true;
			a->m_Collided = b->m_Collided = true;

			continue;
		}

		XMMATRIX mat_rot_a{ XMMatrixRotationZ(a->RotationAngle) };
		for (const auto& a_point : a->m_vFineCollisionBoxPoints)
		{
			XMVECTOR rotated_a{ XMVector2TransformCoord(XMVectorSet(a_point.x, a_point.y, 0.0f, 1.0f), mat_rot_a) };
			XMFLOAT2 world_a{ a->WorldPosition };
			world_a.x += XMVectorGetX(rotated_a);
			world_a.y += XMVectorGetY(rotated_a);

			if (IsPointAInsideBoxB(world_a, b))
			{
				FineCollision = true;
				a->m_Collided = b->m_Collided = true;

				continue;
			}
		}

		XMMATRIX mat_rot_b{ XMMatrixRotationZ(b->RotationAngle) };
		for (const auto& b_point : b->m_vFineCollisionBoxPoints)
		{
			XMVECTOR rotated_b{ XMVector2TransformCoord(XMVectorSet(b_point.x, b_point.y, 0.0f, 1.0f), mat_rot_b) };
			XMFLOAT2 world_b{ b->WorldPosition };
			world_b.x += XMVectorGetX(rotated_b);
			world_b.y += XMVectorGetY(rotated_b);

			if (IsPointAInsideBoxB(world_b, a))
			{
				FineCollision = true;
				a->m_Collided = b->m_Collided = true;

				continue;
			}
		}
	}
}

bool CEntityPool::IsPointAInsideBoxB(const XMFLOAT2& PointA, CEntity* EntityB)
{
	static const vector<XMVECTOR> dirs
	{
		XMVectorSet(0.0f, +1.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(+1.0f, 0.0f, 0.0f, 0.0f)
	};

	XMMATRIX b_mat_rot{ XMMatrixRotationZ(EntityB->RotationAngle) };
	XMFLOAT2 dist_b_a{ PointA - EntityB->WorldPosition };
	XMVECTOR vec_b_a{ XMVectorSet(dist_b_a.x, dist_b_a.y, 0.0f, 0.0f) };

	for (size_t i = 0; i < 4; ++i)
	{
		XMVECTOR b_rotated_dir{ XMVector2TransformNormal(dirs[i], b_mat_rot) };
		float dot{ XMVectorGetX(XMVector2Dot(vec_b_a, b_rotated_dir)) };

		if (i % 2 == 0)
		{
			if (dot > EntityB->m_FineCollisionBoxHalfSize.y) return false;
		}
		else
		{
			if (dot > EntityB->m_FineCollisionBoxHalfSize.x) return false;
		}
	}

	return true;
}