#include "CEntityPool.h"

CEntity* CEntityPool::CreateEntity()
{
	m_vEntities.emplace_back(make_unique<CEntity>(m_PtrDirectX));
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
}

void CEntityPool::DetectFineCollision()
{
	FineCollisionDetected = false;

	for (auto& pair : m_vFineCollisionPairs)
	{
		CEntity* a{ pair.first };
		CEntity* b{ pair.second };

		float dist_ab{ XMFLOAT2GetLength(a->WorldPosition - b->WorldPosition) };
		if (dist_ab <= a->m_FineCollisionRadius + b->m_FineCollisionRadius)
		{
			FineCollisionDetected = true;
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
				FineCollisionDetected = true;
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
				FineCollisionDetected = true;
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