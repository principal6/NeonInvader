#pragma once

#include "..\Helper\CTexturePool.h"
#include "..\Helper\CASCIIRenderer.h"
#include "CEntityPool.h"
#include "CLevelLoader.h"

enum class EEnemyType
{
	Small,
	Normal,
	Big,
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
	void SetGameData(CEntity* EntityMainSprite, vector<SEnemy>& vEnemies, vector<SShot>& vShots) noexcept;
	void SetLevel(SLevelData* PtrLevelData);
	void SpawnShot(float ShotSpeed);

	void ExecuteGame();

public:
	int GetLife() const noexcept { return m_CurrentLife; }
	int GetShotCount() const noexcept { return m_CurrentShotCount; }
	int GetMaxShotCount() const noexcept { return m_CurrentMaxShotCount; }
	int GetEnemyCount() const noexcept { return m_CurrentEnemyCount; }
	int GetMaxEnemyCount() const noexcept { return m_CurrentMaxEnemyCount; }
	bool IsGameOver() const noexcept { return m_GameOver; }

private:
	void SpawnEnemy(EEnemyType Type, int Life, float SpeedFactor);
	void PositionEnemyInsideScreen(SEnemy& Enemy);

	void ProcessCollision();
	void RepositionEnemiesOutOfScreen();
	void ClearDeadShots();

public:
	static constexpr size_t KMaxShotLimit{ 20 };
	static constexpr size_t KMaxEnemyLimit{ 30 };

private:
	static constexpr float KEnemySpawnBoundary{ 30.0f };
	static constexpr float KEnemyNormalSpeedFactor{ 1.2f };
	static constexpr float KEnemyBigSpeedFactor{ 1.5f };
	static constexpr int KCollisionInterval{ 600 };

	XMFLOAT2 m_WindowSize{};

	bool m_GameStarted{ false };
	bool m_GameOver{ false };
	int m_CollisionIntervalCounter{ KCollisionInterval };
	int m_CurrentLife{};
	int m_CurrentShotCount{};
	int m_CurrentMaxShotCount{ 3 };

	int m_CurrentEnemyCount{};
	int m_CurrentMaxEnemyCount{};

	CEntity* m_PtrMainSprite{};
	vector<SEnemy>* m_PtrVEnemies{};
	vector<SShot>* m_PtrVShots{};
};