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
	int Life{ 10 };
};

class CNeonInvader final
{
public:
	CNeonInvader(const XMFLOAT2& WindowSize) : m_WindowSize{ WindowSize } {}
	~CNeonInvader() {}

	void InitGame(int Life);
	void SetGameData(SStageSetData& StageSetData, CEntity* EntityMainSprite, 
		vector<SEnemy>& vEnemies, vector<SShot>& vShots, vector<SEffect>& vEffects) noexcept;
	void SetStage(int StageID);
	void SpawnShot(float ShotSpeed);

	void AnimateEffects();

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
	void PositionEnemyInsideScreen(SEnemy& Enemy);
	void SpawnEnemy(EEnemyType Type, int Life, float SpeedFactor);

	void SpawnEffect(const XMFLOAT2& Position);

	void ProcessCollision();
	void RepositionEnemiesOutOfScreen();
	void ClearDeadShots();

public:
	static constexpr size_t KMaxShotLimit{ 20 };
	static constexpr size_t KMaxEnemyLimit{ 30 };
	static constexpr size_t KMaxEffectLimit{ 40 };

private:
	static constexpr float KEnemySpawnBoundary{ 30.0f };
	static constexpr float KEnemyNormalSpeedFactor{ 1.2f };
	static constexpr float KEnemyBigSpeedFactor{ 1.5f };
	static constexpr int KCollisionInterval{ 600 };

	XMFLOAT2			m_WindowSize{};

	bool				m_GameStarted{ false };
	bool				m_GameOver{ false };
	bool				m_GameCompleted{ false };
	int					m_CollisionIntervalCounter{ KCollisionInterval };
	int					m_CurrentLife{};
	int					m_CurrentShotCount{};
	int					m_CurrentMaxShotCount{ 3 };

	int					m_CurrentEnemyCount{};
	int					m_CurrentMaxEnemyCount{};

	int					m_CurrentStage{};
	int					m_MaxStage{};

	SStageSetData*		m_PtrStageSet{};
	CEntity*			m_PtrMainSprite{};
	vector<SEnemy>*		m_PtrVEnemies{};
	vector<SShot>*		m_PtrVShots{};
	vector<SEffect>*	m_PtrVEffecs{};
};