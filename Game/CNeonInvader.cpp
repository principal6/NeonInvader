#include "CNeonInvader.h"

void CNeonInvader::CreateGameObjects(CTexture* SpriteSheet, CTexture* Effect, CTexture* ItemSet, CTexture* UISheet)
{
	for (size_t i = 0; i < KMaxMainSpriteShotLimit; ++i)
	{
		m_vMainShipShots.emplace_back();
		m_vMainShipShots.back().PtrEntity = CreateEntity();
		m_vMainShipShots.back().PtrEntity->SetTexture(SpriteSheet);
		m_vMainShipShots.back().PtrEntity->CreateRectangle(XMFLOAT2(110, 40));
		m_vMainShipShots.back().PtrEntity->SetRectangleUVRange(XMFLOAT2(0, 0), XMFLOAT2(110, 40));
		m_vMainShipShots.back().PtrEntity->Sampler = ESampler::Linear;
		m_vMainShipShots.back().PtrEntity->Visible = false;
		m_vMainShipShots.back().PtrEntity->SetCollisionBox(XMFLOAT2(1, 10));
	}

	for (size_t i = 0; i < KMaxEnemyShotLimit; ++i)
	{
		m_vEnemyShots.emplace_back();
		m_vEnemyShots.back().PtrEntity = CreateEntity();
		m_vEnemyShots.back().PtrEntity->SetTexture(SpriteSheet);
		m_vEnemyShots.back().PtrEntity->CreateRectangle(XMFLOAT2(110, 40));
		m_vEnemyShots.back().PtrEntity->SetRectangleUVRange(XMFLOAT2(110, 0), XMFLOAT2(110, 40));
		m_vEnemyShots.back().PtrEntity->Sampler = ESampler::Linear;
		m_vEnemyShots.back().PtrEntity->Visible = false;
		m_vEnemyShots.back().PtrEntity->SetCollisionBox(XMFLOAT2(1, 10));
	}

	for (size_t i = 0; i < KMaxEnemyLimit; ++i)
	{
		m_vEnemies.emplace_back();
		m_vEnemies.back().PtrEntity = CreateEntity();
		m_vEnemies.back().PtrEntity->SetTexture(SpriteSheet);
		m_vEnemies.back().PtrEntity->CreateRectangle(XMFLOAT2(110, 80));
		m_vEnemies.back().PtrEntity->Sampler = ESampler::Linear;
		m_vEnemies.back().PtrEntity->Visible = false;
	}

	for (size_t i = 0; i < KMaxItemLimit; ++i)
	{
		m_vItems.emplace_back();

		m_vItems.back().Dead = true;
		CEntity*& entity{ m_vItems.back().PtrEntity };
		{
			entity = CreateEntity();

			entity->SetTexture(ItemSet);
			entity->CreateRectangle(XMFLOAT2(100, 100));
			entity->SetRectangleUVRange(XMFLOAT2(0, 0), XMFLOAT2(100, 100));
			entity->SetCollisionBox(XMFLOAT2(26, 26));
			entity->Sampler = ESampler::Linear;
			entity->Visible = false;
		}
	}

	m_PtrMainSprite = CreateEntity();
	{
		m_PtrMainSprite->SetTexture(SpriteSheet);
		m_PtrMainSprite->CreateRectangle(XMFLOAT2(110, 80));
		m_PtrMainSprite->SetRectangleUVRange(XMFLOAT2(0, 40), XMFLOAT2(110, 80));
		m_PtrMainSprite->Sampler = ESampler::Linear;
		m_PtrMainSprite->Visible = false;
		m_PtrMainSprite->SetCollisionBox(XMFLOAT2(27, 20));
	}

	for (size_t i = 0; i < KMaxEffectLimit; ++i)
	{
		m_vEffects.emplace_back();

		m_vEffects.back().Dead = true;

		CEntity*& entity{ m_vEffects.back().PtrEntity };
		{
			entity = CreateEntity();

			XMFLOAT2 rect{ XMFLOAT2(196, 190) };
			entity->SetTexture(Effect);
			entity->CreateRectangle(rect);
			entity->ShouldCollide = false;
			entity->Visible = false;

			SAnimation* animation{ entity->AddAnimation("explode") };
			animation->ShouldRepeat = false;
			animation->vFrames.emplace_back(XMFLOAT2(0, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(196, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(392, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(588, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(784, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(980, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1176, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1372, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1568, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1764, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1960, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(2156, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(2352, 0), rect);

			entity->SetAnimation("explode");
		}
	}
	
	for (size_t i = 0; i < KMaxEffectLimit; ++i)
	{
		m_vScores.emplace_back();

		m_vScores.back().Dead = true;

		CEntity*& entity{ m_vScores.back().PtrEntity };
		{
			entity = CreateEntity();

			XMFLOAT2 size{ XMFLOAT2(100, 50) };
			entity->SetTexture(UISheet);
			entity->CreateRectangle(size);
			entity->ShouldCollide = false;
			entity->Visible = false;

			{
				SAnimation* animation{ entity->AddAnimation("5") };
				animation->ShouldRepeat = false;
				animation->vFrames.emplace_back(XMFLOAT2(150, 0), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 1), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 2), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 3), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 4), size);
			}
			{
				SAnimation* animation{ entity->AddAnimation("10") };
				animation->ShouldRepeat = false;
				animation->vFrames.emplace_back(XMFLOAT2(150, 50), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 51), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 52), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 53), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 54), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 55), size);
			}
			{
				SAnimation* animation{ entity->AddAnimation("15") };
				animation->ShouldRepeat = false;
				animation->vFrames.emplace_back(XMFLOAT2(150, 100), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 101), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 102), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 103), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 104), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 105), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 106), size);
			}
			{
				SAnimation* animation{ entity->AddAnimation("20") };
				animation->ShouldRepeat = false;
				animation->vFrames.emplace_back(XMFLOAT2(150, 150), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 151), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 152), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 153), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 154), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 155), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 156), size);
				animation->vFrames.emplace_back(XMFLOAT2(150, 157), size);
			}
		}
	}
}

void CNeonInvader::CreateAudioObjects(const string& AssetDir)
{
	assert(FMOD::System_Create(&m_FMODSystem) == FMOD_OK);

	assert(m_FMODSystem->init(32, FMOD_INIT_NORMAL, nullptr) == FMOD_OK);

	assert(m_FMODSystem->createSound((AssetDir + "chillout_lonley_pad_meditation_karma_ron.mp3").c_str(), FMOD_LOOP_NORMAL, nullptr, &m_SoundBG) == FMOD_OK);

	assert(m_FMODSystem->createSound((AssetDir + "laser_jm_noiz.wav").c_str(), FMOD_DEFAULT | FMOD_NONBLOCKING, nullptr, &m_SoundLaser) == FMOD_OK);

	assert(m_FMODSystem->createSound((AssetDir + "laser_enemy.wav").c_str(), FMOD_DEFAULT | FMOD_NONBLOCKING, nullptr, &m_SoundLaserEnemy) == FMOD_OK);

	assert(m_FMODSystem->createSound((AssetDir + "big_explosion.wav").c_str(), FMOD_DEFAULT | FMOD_NONBLOCKING, nullptr, &m_SoundExplosionBig) == FMOD_OK);

	assert(m_FMODSystem->createSound((AssetDir + "small_explosion.wav").c_str(), FMOD_DEFAULT | FMOD_NONBLOCKING, nullptr, &m_SoundExplosionSmall) == FMOD_OK);

	assert(m_FMODSystem->createSound((AssetDir + "item.wav").c_str(), FMOD_DEFAULT | FMOD_NONBLOCKING, nullptr, &m_SoundItem) == FMOD_OK);

	m_FMODSystem->playSound(m_SoundBG, nullptr, false, &m_FMODChannelBG);
}

void CNeonInvader::ReleaseAudio()
{
	m_SoundBG->release();
	m_SoundLaser->release();
	m_SoundExplosionBig->release();
	m_SoundExplosionSmall->release();

	m_SoundItem->release();

	m_FMODSystem->release();
}

void CNeonInvader::InitGame()
{
	m_Score = 0;
	m_CurrentLife = KMaxLifeLimit;
	m_GameOver = false;
	m_GameCompleted = false;
	m_GameStarted = true;

	m_CurrentBulletSpeed = KDefaultBulletSpeed;
	m_CurrentMaxAmmoCount = KDefaultAmmoCount;
	m_CurrentReloadInterval = KDefaultReloadInterval;
}

void CNeonInvader::SetGameData(SStageSetData& StageSetData) noexcept
{
	m_PtrStageSet = &StageSetData;

	m_MaxStage = (int)StageSetData.vStages.size() - 1;
}

void CNeonInvader::DetectCoarseCollision()
{
	CEntityPool::DetectCoarseCollision();

	// Collision type #01 { Main sprite, Enemy sprites }
	m_PtrMainSprite->m_Collided = false;
	for (auto& enemy : m_vEnemies)
	{
		enemy.PtrEntity->m_Collided = false;

		if (m_PtrMainSprite->ShouldCollide && enemy.PtrEntity->ShouldCollide && m_PtrMainSprite->Visible && enemy.PtrEntity->Visible)
		{
			XMFLOAT2 diff{ m_PtrMainSprite->WorldPosition - enemy.PtrEntity->WorldPosition };
			float distance{ XMFLOAT2GetLength(diff) };

			if (distance <= m_PtrMainSprite->m_CoarseCollisionRadius + enemy.PtrEntity->m_CoarseCollisionRadius)
			{
				m_vFineCollisionPairs.emplace_back(m_PtrMainSprite, enemy.PtrEntity);
			}
		}
	}

	// Collision type #02 { Main sprite, Enemy shots }
	for (auto& enemy_shot : m_vEnemyShots)
	{
		enemy_shot.PtrEntity->m_Collided = false;

		if (enemy_shot.PtrEntity->ShouldCollide && m_PtrMainSprite->ShouldCollide && enemy_shot.PtrEntity->Visible && m_PtrMainSprite->Visible)
		{
			XMFLOAT2 diff{ enemy_shot.PtrEntity->WorldPosition - m_PtrMainSprite->WorldPosition };
			float distance{ XMFLOAT2GetLength(diff) };

			if (distance <= enemy_shot.PtrEntity->m_CoarseCollisionRadius + m_PtrMainSprite->m_CoarseCollisionRadius)
			{
				m_vFineCollisionPairs.emplace_back(enemy_shot.PtrEntity, m_PtrMainSprite);
			}
		}
	}

	// Collision type #03 { Main sprite, Items }
	for (auto& item : m_vItems)
	{
		item.PtrEntity->m_Collided = false;

		if (item.PtrEntity->ShouldCollide && m_PtrMainSprite->ShouldCollide && item.PtrEntity->Visible && m_PtrMainSprite->Visible)
		{
			XMFLOAT2 diff{ item.PtrEntity->WorldPosition - m_PtrMainSprite->WorldPosition };
			float distance{ XMFLOAT2GetLength(diff) };

			if (distance <= item.PtrEntity->m_CoarseCollisionRadius + m_PtrMainSprite->m_CoarseCollisionRadius)
			{
				m_vFineCollisionPairs.emplace_back(item.PtrEntity, m_PtrMainSprite);
			}
		}
	}

	// Collision type #04 { Main sprite shots, Enemy sprites }
	for (auto& main_sprite_shot : m_vMainShipShots)
	{
		main_sprite_shot.PtrEntity->m_Collided = false;

		for (auto& enemy : m_vEnemies)
		{
			if (main_sprite_shot.PtrEntity->ShouldCollide && enemy.PtrEntity->ShouldCollide && main_sprite_shot.PtrEntity->Visible && enemy.PtrEntity->Visible)
			{
				XMFLOAT2 diff{ main_sprite_shot.PtrEntity->WorldPosition - enemy.PtrEntity->WorldPosition };
				float distance{ XMFLOAT2GetLength(diff) };

				if (distance <= main_sprite_shot.PtrEntity->m_CoarseCollisionRadius + enemy.PtrEntity->m_CoarseCollisionRadius)
				{
					m_vFineCollisionPairs.emplace_back(main_sprite_shot.PtrEntity, enemy.PtrEntity);
				}
			}
		}
	}
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
		SEnemy& enemy{ (m_vEnemies)[i] };

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

	for (auto& shot : m_vEnemyShots)
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

			m_FMODSystem->playSound(m_SoundLaserEnemy, nullptr, false, &m_FMODChannelEffects);

			break;
		}
	}
}

void CNeonInvader::SpawnEffect(const XMFLOAT2& Position, float Scalar)
{
	for (auto& effect : m_vEffects)
	{
		if (effect.Dead)
		{
			effect.Dead = false;

			effect.PtrEntity->Scalar.x = Scalar;
			effect.PtrEntity->Scalar.y = Scalar;
			effect.PtrEntity->WorldPosition = Position;
			effect.PtrEntity->Visible = true;
			effect.PtrEntity->SetAnimation(0, true);

			ShakeCamera(KCameraShakeXIntensity * Scalar, KCameraShakeYIntensity * Scalar);

			break;
		}
	}
}

void CNeonInvader::SpawnScore(int Score, const XMFLOAT2& Position)
{
	for (auto& score : m_vScores)
	{
		if (score.Dead)
		{
			score.Dead = false;

			score.PtrEntity->WorldPosition = Position;
			score.PtrEntity->Visible = true;
			score.PtrEntity->SetAnimation(to_string(Score), true);

			break;
		}
	}
}

void CNeonInvader::RepositionEnemiesOutOfScreen()
{
	for (auto& enemy : m_vEnemies)
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
		PtrEntity->RotationAngle += angle_diff  * (0.0002f * (SpeedFactor / 3) * RotationSpeedFactor);
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
	for (auto& enemy : m_vEnemies)
	{
		if (!enemy.Dead)
		{
			OrientEntityTowardsMainEntity(enemy.PtrEntity, enemy.SpeedFactor, true);
		}
	}
}

void CNeonInvader::ReorientItems()
{
	for (auto& item : m_vItems)
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

	for (auto& enemy : m_vEnemies)
	{
		enemy.Dead = true;
		enemy.PtrEntity->Visible = false;
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

bool CNeonInvader::SpawnMainSpriteShot()
{
	if (!m_GameStarted) return false;

	if (m_CurrentReloadIntervalCounter < m_CurrentReloadInterval) return false;

	bool result{ false };
	for (size_t i = 0; i < m_CurrentMaxAmmoCount; ++i)
	{
		SShot& shot{ m_vMainShipShots[i] };
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

			++m_CurrentUsedAmmoCount;

			result = true;

			m_FMODSystem->playSound(m_SoundLaser, nullptr, false, &m_FMODChannelEffects);

			break;
		}
	}

	m_CurrentReloadIntervalCounter = 0;

	return result;
}

void CNeonInvader::SpawnItem()
{
	if (!m_GameStarted) return;
	if (m_GameOver) return;

	if (m_CurrentStageItemSpawningCount >= m_CurrentStageMaxItemSpawningCount) return;

	for (auto& item : m_vItems)
	{
		if (item.Dead)
		{
			int item_id{ rand() % KItemTypeCount };

			item.PtrEntity->SetRectangleUVRange(XMFLOAT2(100.0f * item_id, 0), XMFLOAT2(100, 100));
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

void CNeonInvader::AnimateScene()
{
	AnimateEffects();
	AnimateScores();
	AnimateCameraShaking();
}

void CNeonInvader::SetCameraToOrigin()
{
	SCBCameraShake no_shake{};

	CConstantBuffer* cb_camera_shake{ m_PtrDirectX->GetCBCameraShakePtr() };
	cb_camera_shake->Update(&no_shake);
}

void CNeonInvader::AnimateEffects()
{
	for (auto& effect : m_vEffects)
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

void CNeonInvader::AnimateScores()
{
	for (auto& score : m_vScores)
	{
		if (score.PtrEntity->Visible && score.PtrEntity->m_vAnimations.size())
		{
			score.PtrEntity->Animate();
			if (score.PtrEntity->m_vAnimations[score.PtrEntity->m_AnimationIndex]->IsOver)
			{
				score.PtrEntity->Visible = false;
				score.Dead = true;
			}
		}
	}
}

void CNeonInvader::AnimateCameraShaking()
{
	if (m_CBCameraShakeData.CameraOffsetX == 0 && m_CBCameraShakeData.CameraOffsetY == 0) return;

	m_CBCameraShakeData.CameraOffsetX *= -0.98f;
	m_CBCameraShakeData.CameraOffsetY *= -0.98f;

	if (fabs(m_CBCameraShakeData.CameraOffsetX) <= 0.001f) m_CBCameraShakeData.CameraOffsetX = 0;
	if (fabs(m_CBCameraShakeData.CameraOffsetY) <= 0.001f) m_CBCameraShakeData.CameraOffsetY = 0;

	CConstantBuffer* cb_camera_shake{ m_PtrDirectX->GetCBCameraShakePtr() };
	cb_camera_shake->Update(&m_CBCameraShakeData);
}

void CNeonInvader::ShakeCamera(float XIntensity, float YIntensity)
{
	m_CBCameraShakeData.CameraOffsetX = XIntensity;
	m_CBCameraShakeData.CameraOffsetY = YIntensity;
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

		for (auto& enemy : m_vEnemies)
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

		m_FMODSystem->update();
	}
}

void CNeonInvader::ClearDeadShots()
{
	for (auto& i : m_vMainShipShots)
	{
		if (i.Dead) continue;

		if (i.PtrEntity->WorldPosition.x < -m_WindowSize.x / 2 - KScreenSpawnBoundary ||
			i.PtrEntity->WorldPosition.x > +m_WindowSize.x / 2 + KScreenSpawnBoundary ||
			i.PtrEntity->WorldPosition.y < -m_WindowSize.y / 2 - KScreenSpawnBoundary ||
			i.PtrEntity->WorldPosition.y > +m_WindowSize.y / 2 + KScreenSpawnBoundary)
		{
			i.PtrEntity->Visible = false;
			i.Dead = true;

			--m_CurrentUsedAmmoCount;
		}
	}
}

void CNeonInvader::ClearDeadItems()
{
	for (auto& i : m_vItems)
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
		for (auto& item : m_vItems)
		{
			if (item.PtrEntity->m_Collided)
			{
				m_Score += KScoreGetItem;
				SpawnScore(KScoreGetItem, item.PtrEntity->WorldPosition);

				item.Dead = true;
				item.PtrEntity->Visible = false;

				switch (item.eItemType)
				{
				case EItemType::Ammo:
					if (m_CurrentMaxAmmoCount < KMaxAmmoLimit)
					{
						++m_CurrentMaxAmmoCount;
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
						m_CurrentReloadInterval -= 40;
						m_CurrentReloadInterval = max(m_CurrentReloadInterval, KMinReloadInterval);
					}
					break;
				default:
					break;
				}

				m_FMODSystem->playSound(m_SoundItem, nullptr, false, &m_FMODChannelEffects);

				m_PtrMainSprite->m_Collided = false;
			}
		}

		for (auto& main_sprite_shot : m_vMainShipShots)
		{
			if (main_sprite_shot.PtrEntity->m_Collided)
			{
				m_Score += KScoreHitEnemy;
				SpawnScore(KScoreHitEnemy, main_sprite_shot.PtrEntity->WorldPosition);

				--m_CurrentUsedAmmoCount;
				main_sprite_shot.Dead = true;
				main_sprite_shot.PtrEntity->Visible = false;

				m_FMODSystem->playSound(m_SoundExplosionSmall, nullptr, false, &m_FMODChannelEffects);
				SpawnEffect(main_sprite_shot.PtrEntity->WorldPosition, 0.4f);
			}
		}

		for (auto& enemy_shot : m_vEnemyShots)
		{
			if (enemy_shot.PtrEntity->m_Collided)
			{
				enemy_shot.Dead = true;
				enemy_shot.PtrEntity->Visible = false;

				m_FMODSystem->playSound(m_SoundExplosionSmall, nullptr, false, &m_FMODChannelEffects);
				SpawnEffect(enemy_shot.PtrEntity->WorldPosition, 0.2f);
			}
		}

		if (m_PtrMainSprite->m_Collided)
		{
			--m_CurrentLife;

			if (m_CurrentLife <= 0)
			{
				m_FMODSystem->playSound(m_SoundExplosionBig, nullptr, false, &m_FMODChannelEffects);

				SpawnEffect(m_PtrMainSprite->WorldPosition);

				m_GameOver = true;
			}

			m_CollisionIntervalCounter = 0;
		}

		for (auto& enemy : m_vEnemies)
		{
			if (enemy.PtrEntity->m_Collided)
			{
				--enemy.Life;

				if (enemy.Life <= 0)
				{
					m_Score += KScoreKillEnemy;
					SpawnScore(KScoreKillEnemy, XMFLOAT2(enemy.PtrEntity->WorldPosition.x + 20.0f, enemy.PtrEntity->WorldPosition.y + 30.0f));

					enemy.Dead = true;
					enemy.PtrEntity->Visible = false;
					--m_CurrentEnemyCount;

					m_FMODSystem->playSound(m_SoundExplosionBig, nullptr, false, &m_FMODChannelEffects);

					SpawnEffect(enemy.PtrEntity->WorldPosition);
				}
				else
				{
					if (m_PtrMainSprite->m_Collided)
					{
						m_FMODSystem->playSound(m_SoundExplosionSmall, nullptr, false, &m_FMODChannelEffects);

						SpawnEffect(enemy.PtrEntity->WorldPosition, 0.4f);
					}
				}
			}
		}
	}
}