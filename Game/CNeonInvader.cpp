#include "CNeonInvader.h"

void CNeonInvader::PositionEnemyInsideScreen(SEnemy& Enemy, const CEntity* EntityMainShip)
{
	int direction{ rand() % 4 };
	switch (direction)
	{
	case 0:
		Enemy.PtrEntity->WorldPosition.x = ((float)(rand() % 101) / 100.0f) * m_WindowSize.x - (m_WindowSize.x / 2);
		Enemy.PtrEntity->WorldPosition.y = -m_WindowSize.y / 2 - KEnemySpawnBoundary;
		break;
	case 1:
		Enemy.PtrEntity->WorldPosition.x = -m_WindowSize.x / 2 - KEnemySpawnBoundary;
		Enemy.PtrEntity->WorldPosition.y = ((float)(rand() % 101) / 100.0f) * m_WindowSize.y - (m_WindowSize.y / 2);
		break;
	case 2:
		Enemy.PtrEntity->WorldPosition.x = ((float)(rand() % 101) / 100.0f) * m_WindowSize.x - (m_WindowSize.x / 2);
		Enemy.PtrEntity->WorldPosition.y = +m_WindowSize.y / 2 + KEnemySpawnBoundary;
		break;
	case 3:
		Enemy.PtrEntity->WorldPosition.x = +m_WindowSize.x / 2 + KEnemySpawnBoundary;
		Enemy.PtrEntity->WorldPosition.y = ((float)(rand() % 101) / 100.0f) * m_WindowSize.y - (m_WindowSize.y / 2);
		break;
	default:
		break;
	}

	XMVECTOR dir{ EntityMainShip->WorldPosition.x - Enemy.PtrEntity->WorldPosition.x,
		EntityMainShip->WorldPosition.y - Enemy.PtrEntity->WorldPosition.y, 0.0f, 0.0f };
	dir = XMVector2Normalize(dir);

	float cos{ XMVectorGetX(XMVector2Dot(dir, { 0.0f, 1.0f, 0.0f, 0.0f })) };
	float angle{ acos(cos) };
	if (XMVectorGetX(dir) > 0) angle = XM_2PI - angle;
	Enemy.PtrEntity->RotationAngle = angle;

	Enemy.PtrEntity->SetLinearVelocity(dir * Enemy.SpeedFactor);
}

void CNeonInvader::SpawnEnemy(vector<SEnemy>& vEnemies, EEnemyType Type, int Life, float SpeedFactor, const CEntity* EntityMainShip)
{
	for (size_t i = 0; i < m_CurrentMaxEnemyCount; ++i)
	{
		SEnemy& enemy{ vEnemies[i] };

		if (enemy.Dead)
		{
			enemy.Life = Life;
			enemy.SpeedFactor = SpeedFactor;

			PositionEnemyInsideScreen(enemy, EntityMainShip);

			switch (Type)
			{
			case EEnemyType::Small:
				enemy.PtrEntity->SetCollisionBox(XMFLOAT2(16, 12));
				enemy.PtrEntity->SetRectangleUVRange(XMFLOAT2(110, 40), XMFLOAT2(110, 80));
				break;
			case EEnemyType::Normal:
				enemy.PtrEntity->SetCollisionBox(XMFLOAT2(27, 20));
				enemy.PtrEntity->SetRectangleUVRange(XMFLOAT2(220, 40), XMFLOAT2(110, 80));
				break;
			case EEnemyType::Big:
				enemy.PtrEntity->SetCollisionBox(XMFLOAT2(42, 31));
				enemy.PtrEntity->SetRectangleUVRange(XMFLOAT2(330, 40), XMFLOAT2(110, 80));
				break;
			default:
				break;
			}

			enemy.PtrEntity->Visible = true;
			enemy.Dead = false;

			break;
		}
	}
}

void CNeonInvader::RepositionEnemiesOutOfScreen(vector<SEnemy>& vEnemies, const CEntity* EntityMainShip)
{
	for (auto& enemy : vEnemies)
	{
		if (enemy.PtrEntity->WorldPosition.x < -m_WindowSize.x / 2 - KEnemySpawnBoundary ||
			enemy.PtrEntity->WorldPosition.x > +m_WindowSize.x / 2 + KEnemySpawnBoundary ||
			enemy.PtrEntity->WorldPosition.y < -m_WindowSize.y / 2 - KEnemySpawnBoundary ||
			enemy.PtrEntity->WorldPosition.y > +m_WindowSize.y / 2 + KEnemySpawnBoundary)
		{
			PositionEnemyInsideScreen(enemy, EntityMainShip);
		}
	}
}

void CNeonInvader::SetLevel(SLevelData* PtrLevelData, vector<SEnemy>& vEnemies, const CEntity* EntityMainShip)
{
	assert(PtrLevelData);

	m_CurrentEnemyCount = m_CurrentMaxEnemyCount = PtrLevelData->TotalEnemyCount;

	for (int i = 0; i < PtrLevelData->EnemyCountSmall; ++i)
	{
		SpawnEnemy(vEnemies, EEnemyType::Small, 1, PtrLevelData->EnemySpeedFactor, EntityMainShip);
	}

	for (int i = 0; i < PtrLevelData->EnemyCountNormal; ++i)
	{
		SpawnEnemy(vEnemies, EEnemyType::Normal, 1, PtrLevelData->EnemySpeedFactor * KEnemyNormalSpeedFactor, EntityMainShip);
	}

	for (int i = 0; i < PtrLevelData->EnemyCountBig; ++i)
	{
		SpawnEnemy(vEnemies, EEnemyType::Big, 1, PtrLevelData->EnemySpeedFactor * KEnemyBigSpeedFactor, EntityMainShip);
	}
}

void CNeonInvader::SpawnShot(vector<SShot>& vShots, float ShotSpeed, const CEntity* EntityMainShip)
{
	for (size_t i = 0; i < m_CurrentMaxShotCount; ++i)
	{
		SShot& shot{ vShots[i] };
		if (shot.Dead)
		{
			XMMATRIX mat_rot{ XMMatrixRotationZ(EntityMainShip->RotationAngle) };
			XMVECTOR vec{ XMVector2TransformNormal({ 0.0f, 1.0f, 0.0f, 0.0f }, mat_rot) };

			shot.PtrEntity->RotationAngle = EntityMainShip->RotationAngle;
			shot.PtrEntity->SetLinearVelocity(vec * ShotSpeed);

			vec *= 30.0f;
			shot.PtrEntity->WorldPosition.x = EntityMainShip->WorldPosition.x + XMVectorGetX(vec);
			shot.PtrEntity->WorldPosition.y = EntityMainShip->WorldPosition.y + XMVectorGetY(vec);

			shot.PtrEntity->Visible = true;
			shot.Dead = false;

			++m_CurrentShotCount;

			break;
		}
	}
}

void CNeonInvader::ClearDeadShots(vector<SShot>& vShots)
{
	for (auto& i : vShots)
	{
		if (i.Dead) continue;

		if (i.PtrEntity->WorldPosition.x < -m_WindowSize.x / 2 - 20.0f ||
			i.PtrEntity->WorldPosition.x > +m_WindowSize.x / 2 + 20.0f ||
			i.PtrEntity->WorldPosition.y < -m_WindowSize.y / 2 - 20.0f ||
			i.PtrEntity->WorldPosition.y > +m_WindowSize.y / 2 + 20.0f)
		{
			i.PtrEntity->Visible = false;
			i.Dead = true;

			--m_CurrentShotCount;
		}
	}
}