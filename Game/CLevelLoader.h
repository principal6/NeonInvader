#pragma once

#include "..\Helper\CommonHeader.h"

struct SLevelData
{
	int LevelID{};
	float EnemySpeedFactor{};
	int EnemyCountSmall{};
	int EnemyCountNormal{};
	int EnemyCountBig{};
	int TotalEnemyCount{};
};

struct SLevelSetData
{
	string Name{};
	vector<SLevelData> vLevels{};
};

class CLevelLoader
{
public:
	CLevelLoader() {}
	~CLevelLoader() {}

	void LoadLevelSetFromFile(const string& LevelsFileName);

	SLevelData* GetLevelData(int LevelID);

private:
	SLevelSetData m_LevelSet{};
};