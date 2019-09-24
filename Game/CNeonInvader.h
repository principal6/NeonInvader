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

	void SetLevel(SLevelData* PtrLevelData, vector<SEnemy>& vEnemies, const CEntity* EntityMainShip);
	void RepositionEnemiesOutOfScreen(vector<SEnemy>& vEnemies, const CEntity* EntityMainShip);
	
	void SpawnShot(vector<SShot>& vShots, float ShotSpeed, const CEntity* EntityMainShip);
	void ClearDeadShots(vector<SShot>& vShots);

	int GetShotCount() { return m_CurrentShotCount; }
	int GetMaxShotCount() { return m_CurrentMaxShotCount; }
	int GetEnemyCount() { return m_CurrentEnemyCount; }
	int GetMaxEnemyCount() { return m_CurrentMaxEnemyCount; }

private:
	void SpawnEnemy(vector<SEnemy>& vEnemies, EEnemyType Type, int Life, float SpeedFactor, const CEntity* EntityMainShip);
	void PositionEnemyInsideScreen(SEnemy& Enemy, const CEntity* EntityMainShip);

public:
	static constexpr size_t KMaxShotLimit{ 20 };
	static constexpr size_t KMaxEnemyLimit{ 30 };

private:
	static constexpr float KEnemySpawnBoundary{ 30.0f };
	static constexpr float KEnemyNormalSpeedFactor{ 1.2f };
	static constexpr float KEnemyBigSpeedFactor{ 1.5f };

	XMFLOAT2 m_WindowSize{};
	int m_CurrentShotCount{};
	int m_CurrentMaxShotCount{ 3 };

	int m_CurrentEnemyCount{};
	int m_CurrentMaxEnemyCount{};
};