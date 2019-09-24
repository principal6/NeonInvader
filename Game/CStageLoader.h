#pragma once

#include "..\Helper\CommonHeader.h"

struct SStageData
{
	int StageID{};
	float EnemySpeedFactor{};
	int EnemyCountSmall{};
	int EnemyCountNormal{};
	int EnemyCountBig{};
	int TotalEnemyCount{};
	int EnemyShotInterval{};
};

struct SStageSetData
{
	string Name{};
	vector<SStageData> vStages{};
};

class CStageSetLoader
{
public:
	CStageSetLoader() {}
	~CStageSetLoader() {}

	void LoadStageSetFromFile(const string& StageSetFileName);

	SStageSetData& GetStageSetData() noexcept { return m_StageSet; }

private:
	SStageSetData m_StageSet{};
};