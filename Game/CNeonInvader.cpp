#include "CNeonInvader.h"

void CNeonInvader::InitGame(int Life)
{
	m_CurrentLife = Life;
	m_GameOver = false;
	m_GameStarted = true;
}

void CNeonInvader::SetGameData(SStageSetData& StageSetData, CEntity* EntityMainSprite, 
	vector<SEnemy>& vEnemies, vector<SShot>& vMainSpriteShots, vector<SShot>& vEnemyShots, vector<SEffect>& vEffects, vector<SItem>& vItems) noexcept
{
	m_PtrStageSet = &StageSetData;
	m_PtrMainSprite = EntityMainSprite;
	m_PtrVEnemies = &vEnemies;
	m_PtrVMainSpriteShots = &vMainSpriteShots;
	m_PtrVEnemyShots = &vEnemyShots;
	m_PtrVEffecs = &vEffects;
	m_PtrVItems = &vItems;

	m_MaxStage = (int)StageSetData.vStages.size() - 1;
}

int CNeonInvader::PositionEntityInsideScreen(CEntity* PtrEntity, int FromDirection)
{
	int direction{};
	if (FromDirection < 0)
	{
		direction = rand() % 4;
	}
	else
	{
		direction = min(FromDirection, 3);
	}
	switch (direction)
	{
	case 0:
		PtrEntity->WorldPosition.x = ((float)(rand() % 101) / 100.0f) * m_WindowSize.x - (m_WindowSize.x / 2);
		PtrEntity->WorldPosition.y = -m_WindowSize.y / 2 - KScreenSpawnBoundary;
		break;
	case 1:
		PtrEntity->WorldPosition.x = -m_WindowSize.x / 2 - KScreenSpawnBoundary;
		PtrEntity->WorldPosition.y = ((float)(rand() % 101) / 100.0f) * m_WindowSize.y - (m_WindowSize.y / 2);
		break;
	case 2:
		PtrEntity->WorldPosition.x = ((float)(rand() % 101) / 100.0f) * m_WindowSize.x - (m_WindowSize.x / 2);
		PtrEntity->WorldPosition.y = +m_WindowSize.y / 2 + KScreenSpawnBoundary;
		break;
	case 3:
		PtrEntity->WorldPosition.x = +m_WindowSize.x / 2 + KScreenSpawnBoundary;
		PtrEntity->WorldPosition.y = ((float)(rand() % 101) / 100.0f) * m_WindowSize.y - (m_WindowSize.y / 2);
		break;
	default:
		break;
	}

	return direction;
}

void CNeonInvader::PositionEnemyInsideScreen(SEnemy& Enemy)
{
	PositionEntityInsideScreen(Enemy.PtrEntity);
	OrientEntityTowardsMainEntity(Enemy.PtrEntity, Enemy.SpeedFactor, true);
}

void CNeonInvader::SpawnEnemy(EEnemyType Type, int Life, int ShotInterval, float SpeedFactor)
{
	for (size_t i = 0; i < m_CurrentMaxEnemyCount; ++i)
	{
		SEnemy& enemy{ (*m_PtrVEnemies)[i] };

		if (enemy.Dead)
		{
			enemy.Life = Life;
			enemy.ShotInterval = ShotInterval;
			enemy.ShotIntervalCounter = -rand() % KEnemyShotIntervalDeviance;
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

void CNeonInvader::SpawnEnemyShot(SEnemy& Enemy, float ShotSpeed)
{
	if (!m_GameStarted) return;
	if (m_GameOver) return;

	for (auto& shot : *m_PtrVEnemyShots)
	{
		if (shot.Dead)
		{
			XMMATRIX mat_rot{ XMMatrixRotationZ(Enemy.PtrEntity->RotationAngle) };
			XMVECTOR vec{ XMVector2TransformNormal({ 0.0f, 1.0f, 0.0f, 0.0f }, mat_rot) };

			shot.PtrEntity->RotationAngle = Enemy.PtrEntity->RotationAngle;
			shot.PtrEntity->SetLinearVelocity(vec * ShotSpeed);

			vec *= 30.0f;
			shot.PtrEntity->WorldPosition.x = Enemy.PtrEntity->WorldPosition.x + XMVectorGetX(vec);
			shot.PtrEntity->WorldPosition.y = Enemy.PtrEntity->WorldPosition.y + XMVectorGetY(vec);

			shot.PtrEntity->Visible = true;
			shot.Dead = false;

			break;
		}
	}
}

void CNeonInvader::SpawnEffect(const XMFLOAT2& Position, float Scalar)
{
	for (auto& effect : *m_PtrVEffecs)
	{
		if (effect.Dead)
		{
			effect.Dead = false;

			effect.PtrEntity->Scalar.x = Scalar;
			effect.PtrEntity->Scalar.y = Scalar;
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
		if (enemy.PtrEntity->WorldPosition.x < -m_WindowSize.x / 2 - KScreenSpawnBoundary ||
			enemy.PtrEntity->WorldPosition.x > +m_WindowSize.x / 2 + KScreenSpawnBoundary ||
			enemy.PtrEntity->WorldPosition.y < -m_WindowSize.y / 2 - KScreenSpawnBoundary ||
			enemy.PtrEntity->WorldPosition.y > +m_WindowSize.y / 2 + KScreenSpawnBoundary)
		{
			PositionEnemyInsideScreen(enemy);
		}
	}
}

void CNeonInvader::OrientEntityTowardsMainEntity(CEntity* PtrEntity, float SpeedFactor, bool GraduallyOrient, float RotationSpeedFactor)
{
	XMVECTOR up{ 0.0f, 1.0f, 0.0f, 0.0f };
	XMVECTOR dir{ m_PtrMainSprite->WorldPosition.x - PtrEntity->WorldPosition.x,
			m_PtrMainSprite->WorldPosition.y - PtrEntity->WorldPosition.y, 0.0f, 0.0f };
	dir = XMVector2Normalize(dir);

	float cos{ XMVectorGetX(XMVector2Dot(dir, up)) };
	float angle{ acos(cos) };
	if (XMVectorGetX(dir) > 0) angle = XM_2PI - angle;
	float angle_diff{ angle - PtrEntity->RotationAngle };
	if (angle_diff > XM_PI)
	{
		angle_diff -= XM_2PI;
	}

	if (GraduallyOrient)
	{
		PtrEntity->RotationAngle += angle_diff  * (0.0001f * (SpeedFactor / 3) * RotationSpeedFactor);
	}
	else
	{
		PtrEntity->RotationAngle = angle;
	}

	XMMATRIX mat_rot{ XMMatrixRotationZ(PtrEntity->RotationAngle) };
	up = XMVector2TransformNormal(up, mat_rot);

	PtrEntity->SetLinearVelocity(up * SpeedFactor);
}

void CNeonInvader::ReorientEnemies()
{
	for (auto& enemy : *m_PtrVEnemies)
	{
		if (!enemy.Dead)
		{
			OrientEntityTowardsMainEntity(enemy.PtrEntity, enemy.SpeedFactor, true);
		}
	}
}

void CNeonInvader::ReorientItems()
{
	for (auto& item : *m_PtrVItems)
	{
		if (!item.Dead)
		{
			OrientEntityTowardsMainEntity(item.PtrEntity, KItemSpeedFactor, true, 0.2f);
		}
	}
}

void CNeonInvader::SetStage(int StageID)
{
	if (m_GameOver) return;

	if (StageID > m_MaxStage)
	{
		m_GameOver = true;
		m_GameCompleted = true;
		return;
	}

	SStageData* stage{};
	for (auto& l : (*m_PtrStageSet).vStages)
	{
		if (l.StageID == StageID)
		{
			stage = &l;
		}
	}
	assert(stage);

	m_CurrentStage = StageID;

	for (auto& enemy : *m_PtrVEnemies)
	{
		enemy.Dead = true;
	}

	m_CurrentEnemyCount = m_CurrentMaxEnemyCount = stage->TotalEnemyCount;

	for (int i = 0; i < stage->EnemyCountSmall; ++i)
	{
		SpawnEnemy(EEnemyType::Small, 1, stage->EnemyShotInterval * 3, stage->EnemySpeedFactor);
	}

	for (int i = 0; i < stage->EnemyCountNormal; ++i)
	{
		SpawnEnemy(EEnemyType::Normal, 2, stage->EnemyShotInterval * 2, stage->EnemySpeedFactor * KEnemyNormalSpeedFactor);
	}

	for (int i = 0; i < stage->EnemyCountBig; ++i)
	{
		SpawnEnemy(EEnemyType::Big, 4, stage->EnemyShotInterval, stage->EnemySpeedFactor * KEnemyBigSpeedFactor);
	}

	m_CurrentStageMaxItemSpawningCount = stage->MaxItemSpawningCount;
	m_CurrentStageItemSpawningCount = 0;
}

void CNeonInvader::Reload()
{
	if (m_CurrentReloadIntervalCounter < m_CurrentReloadInterval)
	{
		++m_CurrentReloadIntervalCounter;
	}
}

void CNeonInvader::SpawnMainSpriteShot()
{
	if (!m_GameStarted) return;

	if (m_CurrentReloadIntervalCounter < m_CurrentReloadInterval) return;

	for (size_t i = 0; i < m_CurrentMaxShotCount; ++i)
	{
		SShot& shot{ (*m_PtrVMainSpriteShots)[i] };
		if (shot.Dead)
		{
			XMMATRIX mat_rot{ XMMatrixRotationZ(m_PtrMainSprite->RotationAngle) };
			XMVECTOR vec{ XMVector2TransformNormal({ 0.0f, 1.0f, 0.0f, 0.0f }, mat_rot) };

			shot.PtrEntity->RotationAngle = m_PtrMainSprite->RotationAngle;
			shot.PtrEntity->SetLinearVelocity(vec * m_CurrentBulletSpeed);

			vec *= 30.0f;
			shot.PtrEntity->WorldPosition.x = m_PtrMainSprite->WorldPosition.x + XMVectorGetX(vec);
			shot.PtrEntity->WorldPosition.y = m_PtrMainSprite->WorldPosition.y + XMVectorGetY(vec);

			shot.PtrEntity->Visible = true;
			shot.Dead = false;

			++m_CurrentShotCount;

			break;
		}
	}

	m_CurrentReloadIntervalCounter = 0;
}

void CNeonInvader::SpawnItem()
{
	if (!m_GameStarted) return;
	if (m_GameOver) return;

	if (m_CurrentStageItemSpawningCount >= m_CurrentStageMaxItemSpawningCount) return;

	for (auto& item : *m_PtrVItems)
	{
		if (item.Dead)
		{
			int item_id{ rand() % KItemTypeCount };

			item.PtrEntity->SetRectangleUVRange(XMFLOAT2(60.0f * item_id, 0), XMFLOAT2(60, 60));
			item.eItemType = (EItemType)item_id;

			PositionEntityInsideScreen(item.PtrEntity);

			OrientEntityTowardsMainEntity(item.PtrEntity, KItemSpeedFactor, true, 100.0f);

			item.PtrEntity->Visible = true;
			item.Dead = false;

			++m_CurrentStageItemSpawningCount;

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
		Reload();

		ProcessCollision();

		ClearDeadShots();
		ClearDeadItems();

		RepositionEnemiesOutOfScreen();

		for (auto& enemy : *m_PtrVEnemies)
		{
			if (!enemy.Dead)
			{
				++enemy.ShotIntervalCounter;
				if (enemy.ShotIntervalCounter >= enemy.ShotInterval)
				{
					SpawnEnemyShot(enemy);
					enemy.ShotIntervalCounter = -rand() % KEnemyShotIntervalDeviance;
				}
			}
		}

		if (m_CurrentEnemyCount <= 0)
		{
			SetStage(m_CurrentStage + 1);
		}
	}
}

void CNeonInvader::ClearDeadShots()
{
	for (auto& i : *m_PtrVMainSpriteShots)
	{
		if (i.Dead) continue;

		if (i.PtrEntity->WorldPosition.x < -m_WindowSize.x / 2 - KScreenSpawnBoundary ||
			i.PtrEntity->WorldPosition.x > +m_WindowSize.x / 2 + KScreenSpawnBoundary ||
			i.PtrEntity->WorldPosition.y < -m_WindowSize.y / 2 - KScreenSpawnBoundary ||
			i.PtrEntity->WorldPosition.y > +m_WindowSize.y / 2 + KScreenSpawnBoundary)
		{
			i.PtrEntity->Visible = false;
			i.Dead = true;

			--m_CurrentShotCount;
		}
	}
}

void CNeonInvader::ClearDeadItems()
{
	for (auto& i : *m_PtrVItems)
	{
		if (i.Dead) continue;

		if (i.PtrEntity->WorldPosition.x < -m_WindowSize.x / 2 - KScreenSpawnBoundary ||
			i.PtrEntity->WorldPosition.x > +m_WindowSize.x / 2 + KScreenSpawnBoundary ||
			i.PtrEntity->WorldPosition.y < -m_WindowSize.y / 2 - KScreenSpawnBoundary ||
			i.PtrEntity->WorldPosition.y > +m_WindowSize.y / 2 + KScreenSpawnBoundary)
		{
			i.PtrEntity->Visible = false;
			i.Dead = true;
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
		for (auto& item : *m_PtrVItems)
		{
			if (item.PtrEntity->m_Collided)
			{
				item.Dead = true;
				item.PtrEntity->Visible = false;

				switch (item.eItemType)
				{
				case EItemType::Ammo:
					if (m_CurrentMaxShotCount < KMaxAmmoLimit)
					{
						++m_CurrentMaxShotCount;
					}
					break;
				case EItemType::Shield:
					if (m_CurrentLife < KMaxLifeLimit)
					{
						++m_CurrentLife;
					}
					break;
				case EItemType::BulletSpeed:
					if (m_CurrentBulletSpeed < KMaxBulletSpeed)
					{
						m_CurrentBulletSpeed += 50.0f;
					}
					break;
				case EItemType::ReloadSpeed:
					if (m_CurrentReloadInterval > KMinReloadInterval)
					{
						m_CurrentReloadInterval -= 30;
						m_CurrentReloadInterval = max(m_CurrentReloadInterval, KMinReloadInterval);
					}
					break;
				default:
					break;
				}

				m_PtrMainSprite->m_Collided = false;
			}
		}

		if (m_PtrMainSprite->m_Collided)
		{
			--m_CurrentLife;

			if (m_CurrentLife <= 0)
			{
				SpawnEffect(m_PtrMainSprite->WorldPosition);

				m_GameOver = true;
			}

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

		for (auto& main_sprite_shot : *m_PtrVMainSpriteShots)
		{
			if (main_sprite_shot.PtrEntity->m_Collided)
			{
				--m_CurrentShotCount;
				main_sprite_shot.Dead = true;
				main_sprite_shot.PtrEntity->Visible = false;

				SpawnEffect(main_sprite_shot.PtrEntity->WorldPosition, 0.4f);
			}
		}

		for (auto& enemy_shot : *m_PtrVEnemyShots)
		{
			if (enemy_shot.PtrEntity->m_Collided)
			{
				enemy_shot.Dead = true;
				enemy_shot.PtrEntity->Visible = false;

				SpawnEffect(enemy_shot.PtrEntity->WorldPosition, 0.2f);
			}
		}
	}
}