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

class CNeonInvader final
{
public:
	CNeonInvader(const XMFLOAT2& WindowSize) : m_WindowSize{ WindowSize } {}
	~CNeonInvader() {}

	void InitGame(int Life);
	void SetGameData(SStageSetData& StageSetData, CEntity* EntityMainSprite, 
		vector<SEnemy>& vEnemies, vector<SShot>& vMainSpriteShots, vector<SShot>& vEnemyShots, vector<SEffect>& vEffects, vector<SItem>& vItems) noexcept;
	void SetStage(int StageID);

	void SpawnMainSpriteShot();
	void SpawnItem();

	void AnimateEffects();
	void ReorientEnemies();
	void ReorientItems();

	void ExecuteGame();

public:
	int GetStage() const noexcept { return m_CurrentStage; }
	int GetMaxStage() const noexcept { return m_MaxStage; }
	int GetLife() const noexcept { return m_CurrentLife; }
	int GetShotCount() const noexcept { return m_CurrentShotCount; }
	int GetMaxShotCount() const noexcept { return m_CurrentMaxShotCount; }
	int GetEnemyCount() const noexcept { return m_CurrentEnemyCount; }
	int GetMaxEnemyCount() const noexcept { return m_CurrentMaxEnemyCount; }
	bool IsGameRunning() const noexcept { return m_GameStarted; }
	bool IsGameOver() const noexcept { return m_GameOver; }
	bool IsCompleted() const noexcept { return m_GameCompleted; }

private:
	int PositionEntityInsideScreen(CEntity* PtrEntity, int FromDirection = -1);
	void PositionEnemyInsideScreen(SEnemy& Enemy);
	void SpawnEnemy(EEnemyType Type, int Life, int Ammo, float SpeedFactor);

	void SpawnEnemyShot(SEnemy& Enemy, float ShotSpeed = 400.0f);

	void SpawnEffect(const XMFLOAT2& Position, float Scalar = 1.0f);

	void Reload();

	void ProcessCollision();
	void RepositionEnemiesOutOfScreen();
	void OrientEntityTowardsMainEntity(CEntity* PtrEntity, float SpeedFactor, bool GraduallyOrient = false, float RotationSpeedFactor = 1.0f);
	void ClearDeadShots();
	void ClearDeadItems();

public:
	static constexpr size_t KMaxMainSpriteShotLimit{ 20 };
	static constexpr size_t KMaxEnemyShotLimit{ 200 };
	static constexpr size_t KMaxEnemyLimit{ 30 };
	static constexpr size_t KMaxEffectLimit{ 40 };
	static constexpr size_t KMaxItemLimit{ 10 };	

private:
	static constexpr float KScreenSpawnBoundary{ 30.0f };
	static constexpr float KEnemyNormalSpeedFactor{ 1.2f };
	static constexpr float KEnemyBigSpeedFactor{ 1.5f };
	static constexpr float KItemSpeedFactor{ 180.0f };
	static constexpr float KDefaultBulletSpeed{ 400.0f };
	static constexpr float KMaxBulletSpeed{ 800.0f };
	static constexpr int KCollisionInterval{ 200 };
	static constexpr int KEnemyShotIntervalDeviance{ 300 };
	static constexpr int KItemTypeCount{ 4 };
	static constexpr int KMaxAmmoLimit{ 10 };
	static constexpr int KMaxLifeLimit{ 5 };
	static constexpr int KDefaultReloadInterval{ 300 };
	static constexpr int KMinReloadInterval{ 50 };
	static constexpr int KDefaultMaxShotCount{ 3 };

	XMFLOAT2			m_WindowSize{};

	bool				m_GameStarted{ false };
	bool				m_GameOver{ false };
	bool				m_GameCompleted{ false };
	int					m_CollisionIntervalCounter{ KCollisionInterval };
	int					m_CurrentLife{};
	int					m_CurrentShotCount{};
	int					m_CurrentMaxShotCount{ KDefaultMaxShotCount };
	int					m_CurrentStageItemSpawningCount{};
	int					m_CurrentStageMaxItemSpawningCount{};

	int					m_CurrentEnemyCount{};
	int					m_CurrentMaxEnemyCount{};

	int					m_CurrentReloadInterval{ KDefaultReloadInterval };
	int					m_CurrentReloadIntervalCounter{};

	int					m_CurrentStage{};
	int					m_MaxStage{};

	float				m_CurrentBulletSpeed{ KDefaultBulletSpeed };

	SStageSetData*		m_PtrStageSet{};
	CEntity*			m_PtrMainSprite{};
	vector<SEnemy>*		m_PtrVEnemies{};
	vector<SShot>*		m_PtrVMainSpriteShots{};
	vector<SShot>*		m_PtrVEnemyShots{};
	vector<SEffect>*	m_PtrVEffecs{};
	vector<SItem>*		m_PtrVItems{};
};