#pragma once

#include "..\Helper\CTexturePool.h"
#include "..\Helper\CASCIIRenderer.h"
#include "CEntityPool.h"
#include "CStageLoader.h"

enum class EEnemyType
{
	Small,
	Normal,
	Big,
};

enum class EItemType
{
	Ammo,
	Shield,
	BulletSpeed,
	ReloadSpeed,
};

struct SEffect
{
	CEntity* PtrEntity{};
	bool Dead{ true };
};

struct SScore
{
	CEntity* PtrEntity{};
	bool Dead{ true };
};

struct SShot
{
	CEntity* PtrEntity{};
	bool Dead{ true };
};

struct SEnemy
{
	CEntity* PtrEntity{};
	float SpeedFactor{};
	bool Dead{ true };
	int Life{};
	int ShotInterval{};
	int ShotIntervalCounter{};
};

struct SItem
{
	CEntity* PtrEntity{};
	bool Dead{ true };
	EItemType eItemType{};
};

class CNeonInvader final : public CEntityPool
{
public:
	CNeonInvader(const XMFLOAT2& WindowSize, CDirectX* PtrDirectX) : CEntityPool(PtrDirectX) { m_WindowSize = WindowSize; }
	~CNeonInvader() { ReleaseAudio(); }

	void CreateGameObjects(CTexture* SpriteSheet, CTexture* Effect, CTexture* ItemSet, CTexture* UISheet);
	void CreateAudioObjects(const string& AssetDir);
	void ReleaseAudio();
	void InitGame();
	void SetGameData(SStageSetData& StageSetData) noexcept;
	void SetStage(int StageID);

	bool SpawnMainSpriteShot();
	void SpawnItem();

	void AnimateScene();
	void SetCameraToOrigin();
	
	void ReorientEnemies();
	void ReorientItems();

	void ExecuteGame();

public:
	CEntity* GetMainSpriteEntity() { return m_PtrMainSprite; }

public:
	int GetStage() const noexcept { return m_CurrentStage; }
	int GetMaxStage() const noexcept { return m_MaxStage; }
	int GetLife() const noexcept { return m_CurrentLife; }
	int GetShotCount() const noexcept { return m_CurrentUsedAmmoCount; }
	int GetMaxShotCount() const noexcept { return m_CurrentMaxAmmoCount; }
	int GetEnemyCount() const noexcept { return m_CurrentEnemyCount; }
	int GetMaxEnemyCount() const noexcept { return m_CurrentMaxEnemyCount; }
	int GetScore() const noexcept { return m_Score; }
	bool IsGameRunning() const noexcept { return m_GameStarted; }
	bool IsGameOver() const noexcept { return m_GameOver; }
	bool IsCompleted() const noexcept { return m_GameCompleted; }

private:
	void DetectCoarseCollision() override;

private:
	int PositionEntityInsideScreen(CEntity* PtrEntity, int FromDirection = -1);
	void PositionEnemyInsideScreen(SEnemy& Enemy);
	void SpawnEnemy(EEnemyType Type, int Life, int Ammo, float SpeedFactor);

	void SpawnEnemyShot(SEnemy& Enemy, float ShotSpeed = 400.0f);

	void SpawnEffect(const XMFLOAT2& Position, float Scalar = 1.0f);

	void SpawnScore(int Score, const XMFLOAT2& Position);

	void Reload();

	void ProcessCollision();
	void RepositionEnemiesOutOfScreen();
	void OrientEntityTowardsMainEntity(CEntity* PtrEntity, float SpeedFactor, bool GraduallyOrient = false, float RotationSpeedFactor = 1.0f);
	
	void AnimateEffects();
	void AnimateScores();
	void AnimateCameraShaking();
	void ShakeCamera(float XIntensity, float YIntensity);

	void ClearDeadShots();
	void ClearDeadItems();

public:
	static constexpr size_t KMaxMainSpriteShotLimit{ 20 };
	static constexpr size_t KMaxEnemyShotLimit{ 200 };
	static constexpr size_t KMaxEnemyLimit{ 30 };
	static constexpr size_t KMaxEffectLimit{ 40 };
	static constexpr size_t KMaxItemLimit{ 10 };
	static constexpr int KMaxLifeLimit{ 5 };

private:
	static constexpr float KScreenSpawnBoundary{ 30.0f };
	static constexpr int KCollisionInterval{ 200 };
	static constexpr float KCameraShakeXIntensity{ 15.0f };
	static constexpr float KCameraShakeYIntensity{ 5.0f };

private:
	static constexpr int KItemTypeCount{ 4 };
	static constexpr float KEnemyNormalSpeedFactor{ 1.2f };
	static constexpr float KEnemyBigSpeedFactor{ 1.5f };
	static constexpr float KItemSpeedFactor{ 120.0f };
	static constexpr float KDefaultBulletSpeed{ 200.0f };
	static constexpr float KMaxBulletSpeed{ 600.0f };
	static constexpr int KEnemyShotIntervalDeviance{ 300 };
	static constexpr int KDefaultReloadInterval{ 400 };
	static constexpr int KMinReloadInterval{ 50 };
	static constexpr int KDefaultAmmoCount{ 4 };
	static constexpr int KMaxAmmoLimit{ 15 };

private:
	static constexpr int KScoreHitEnemy{ 5 };
	static constexpr int KScoreGetItem{ 10 };
	static constexpr int KScoreKillEnemy{ 20 };

	XMFLOAT2			m_WindowSize{};

	bool				m_GameStarted{ false };
	bool				m_GameOver{ false };
	bool				m_GameCompleted{ false };
	int					m_CollisionIntervalCounter{ KCollisionInterval };
	int					m_CurrentLife{};
	int					m_CurrentUsedAmmoCount{};
	int					m_CurrentMaxAmmoCount{ KDefaultAmmoCount };
	int					m_CurrentStageItemSpawningCount{};
	int					m_CurrentStageMaxItemSpawningCount{};

	int					m_CurrentEnemyCount{};
	int					m_CurrentMaxEnemyCount{};

	int					m_CurrentReloadInterval{ KDefaultReloadInterval };
	int					m_CurrentReloadIntervalCounter{};

	int					m_CurrentStage{};
	int					m_MaxStage{};

	float				m_CurrentBulletSpeed{ KDefaultBulletSpeed };

	int					m_Score{};

	SStageSetData*		m_PtrStageSet{};

	CEntity*			m_PtrMainSprite{};
	vector<SShot>		m_vMainShipShots{};
	vector<SShot>		m_vEnemyShots{};
	vector<SEnemy>		m_vEnemies{};
	vector<SItem>		m_vItems{};
	vector<SEffect>		m_vEffects{};
	vector<SScore>		m_vScores{};

	FMOD::System*		m_FMODSystem{};
	FMOD::Sound*		m_SoundBG{};
	FMOD::Sound*		m_SoundLaser{};
	FMOD::Sound*		m_SoundLaserEnemy{};
	FMOD::Sound*		m_SoundExplosionBig{};
	FMOD::Sound*		m_SoundExplosionSmall{};
	FMOD::Sound*		m_SoundItem{};

	FMOD::Channel*		m_FMODChannelBG{};
	FMOD::Channel*		m_FMODChannelEffects{};

	SCBCameraShake		m_CBCameraShakeData{};
};