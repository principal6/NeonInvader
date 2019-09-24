#include "CNeonInvader.h"

void CNeonInvader::InitGame(int Life)
{
	m_CurrentLife = Life;
	m_GameOver = false;
	m_GameStarted = true;
}

void CNeonInvader::SetGameData(CEntity* EntityMainSprite, vector<SEnemy>& vEnemies, vector<SShot>& vShots, vector<SEffect>& vEffects) noexcept
{
	m_PtrMainSprite = EntityMainSprite;
	m_PtrVEnemies = &vEnemies;
	m_PtrVShots = &vShots;
	m_PtrVEffecs = &vEffects;
}

void CNeonInvader::PositionEnemyInsideScreen(SEnemy& Enemy)
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

	XMVECTOR dir{ m_PtrMainSprite->WorldPosition.x - Enemy.PtrEntity->WorldPosition.x,
		m_PtrMainSprite->WorldPosition.y - Enemy.PtrEntity->WorldPosition.y, 0.0f, 0.0f };
	dir = XMVector2Normalize(dir);

	float cos{ XMVectorGetX(XMVector2Dot(dir, { 0.0f, 1.0f, 0.0f, 0.0f })) };
	float angle{ acos(cos) };
	if (XMVectorGetX(dir) > 0) angle = XM_2PI - angle;
	Enemy.PtrEntity->RotationAngle = angle;

	Enemy.PtrEntity->SetLinearVelocity(dir * Enemy.SpeedFactor);
}

void CNeonInvader::SpawnEnemy(EEnemyType Type, int Life, float SpeedFactor)
{
	for (size_t i = 0; i < m_CurrentMaxEnemyCount; ++i)
	{
		SEnemy& enemy{ (*m_PtrVEnemies)[i] };

		if (enemy.Dead)
		{
			enemy.Life = Life;
			enemy.SpeedFactor = SpeedFactor;

			PositionEnemyInsideScreen(enemy);

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

void CNeonInvader::SpawnEffect(const XMFLOAT2& Position)
{
	for (auto& effect : *m_PtrVEffecs)
	{
		if (effect.Dead)
		{
			effect.Dead = false;

			effect.PtrEntity->WorldPosition = Position;
			effect.PtrEntity->Visible = true;
			effect.PtrEntity->SetAnimation(0, true);

			break;
		}
	}
}

void CNeonInvader::RepositionEnemiesOutOfScreen()
{
	for (auto& enemy : *m_PtrVEnemies)
	{
		if (enemy.PtrEntity->WorldPosition.x < -m_WindowSize.x / 2 - KEnemySpawnBoundary ||
			enemy.PtrEntity->WorldPosition.x > +m_WindowSize.x / 2 + KEnemySpawnBoundary ||
			enemy.PtrEntity->WorldPosition.y < -m_WindowSize.y / 2 - KEnemySpawnBoundary ||
			enemy.PtrEntity->WorldPosition.y > +m_WindowSize.y / 2 + KEnemySpawnBoundary)
		{
			PositionEnemyInsideScreen(enemy);
		}
	}
}

void CNeonInvader::SetLevel(SLevelData* PtrLevelData)
{
	assert(PtrLevelData);

	for (auto& enemy : *m_PtrVEnemies)
	{
		enemy.Dead = true;
	}

	m_CurrentEnemyCount = m_CurrentMaxEnemyCount = PtrLevelData->TotalEnemyCount;

	for (int i = 0; i < PtrLevelData->EnemyCountSmall; ++i)
	{
		SpawnEnemy(EEnemyType::Small, 1, PtrLevelData->EnemySpeedFactor);
	}

	for (int i = 0; i < PtrLevelData->EnemyCountNormal; ++i)
	{
		SpawnEnemy(EEnemyType::Normal, 1, PtrLevelData->EnemySpeedFactor * KEnemyNormalSpeedFactor);
	}

	for (int i = 0; i < PtrLevelData->EnemyCountBig; ++i)
	{
		SpawnEnemy(EEnemyType::Big, 1, PtrLevelData->EnemySpeedFactor * KEnemyBigSpeedFactor);
	}
}

void CNeonInvader::SpawnShot(float ShotSpeed)
{
	if (!m_GameStarted) return;
	if (m_GameOver) return;

	for (size_t i = 0; i < m_CurrentMaxShotCount; ++i)
	{
		SShot& shot{ (*m_PtrVShots)[i] };
		if (shot.Dead)
		{
			XMMATRIX mat_rot{ XMMatrixRotationZ(m_PtrMainSprite->RotationAngle) };
			XMVECTOR vec{ XMVector2TransformNormal({ 0.0f, 1.0f, 0.0f, 0.0f }, mat_rot) };

			shot.PtrEntity->RotationAngle = m_PtrMainSprite->RotationAngle;
			shot.PtrEntity->SetLinearVelocity(vec * ShotSpeed);

			vec *= 30.0f;
			shot.PtrEntity->WorldPosition.x = m_PtrMainSprite->WorldPosition.x + XMVectorGetX(vec);
			shot.PtrEntity->WorldPosition.y = m_PtrMainSprite->WorldPosition.y + XMVectorGetY(vec);

			shot.PtrEntity->Visible = true;
			shot.Dead = false;

			++m_CurrentShotCount;

			break;
		}
	}
}

void CNeonInvader::AnimateEffects()
{
	for (auto& effect : *m_PtrVEffecs)
	{
		if (effect.PtrEntity->Visible && effect.PtrEntity->m_vAnimations.size())
		{
			effect.PtrEntity->Animate();
			if (effect.PtrEntity->m_vAnimations[effect.PtrEntity->m_AnimationIndex]->IsOver)
			{
				effect.PtrEntity->Visible = false;
				effect.Dead = true;
			}
		}
	}
}

void CNeonInvader::ExecuteGame()
{
	if (m_GameOver) return;

	if (m_GameStarted)
	{
		ProcessCollision();

		ClearDeadShots();

		RepositionEnemiesOutOfScreen();

		if (m_CurrentLife <= 0)
		{
			SpawnEffect(m_PtrMainSprite->WorldPosition);

			m_GameOver = true;
		}
	}
}

void CNeonInvader::ClearDeadShots()
{
	for (auto& i : *m_PtrVShots)
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

void CNeonInvader::ProcessCollision()
{
	if (m_CollisionIntervalCounter < KCollisionInterval)
	{
		++m_CollisionIntervalCounter;
		return;
	}
	else
	{
		if (m_PtrMainSprite->m_Collided)
		{
			--m_CurrentLife;
			m_CollisionIntervalCounter = 0;
		}

		for (auto& enemy : *m_PtrVEnemies)
		{
			if (enemy.PtrEntity->m_Collided)
			{
				--enemy.Life;

				if (enemy.Life <= 0)
				{
					enemy.Dead = true;
					enemy.PtrEntity->Visible = false;
					--m_CurrentEnemyCount;

					SpawnEffect(enemy.PtrEntity->WorldPosition);
				}
			}
		}

		for (auto& shot : *m_PtrVShots)
		{
			if (shot.PtrEntity->m_Collided)
			{
				--m_CurrentShotCount;
				shot.Dead = true;
				shot.PtrEntity->Visible = false;
			}
		}
	}
}