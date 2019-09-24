#include "CLevelLoader.h"

void CLevelLoader::LoadLevelSetFromFile(const string& LevelsFileName)
{
	static constexpr int KRowCountPerLevel{ 5 };
	ifstream ifs{ LevelsFileName };
	assert(ifs.is_open());

	int read_count{};
	while (!ifs.eof())
	{
		char line[300]{};
		ifs.getline(line, 300);
		string sline{ line };

		if (sline[0] == '#') continue;
		if (!sline.length()) continue;

		if (read_count == 0)
		{
			m_LevelSet.vLevels.reserve(stoi(sline));
		}
		else
		{
			if ((read_count - 1) % KRowCountPerLevel == 0)
			{
				m_LevelSet.vLevels.emplace_back();
				m_LevelSet.vLevels.back().LevelID = stoi(sline);
			}
			else if ((read_count - 1) % KRowCountPerLevel == 1)
			{
				m_LevelSet.vLevels.back().EnemySpeedFactor = stof(sline);
			}
			else if ((read_count - 1) % KRowCountPerLevel == 2)
			{
				m_LevelSet.vLevels.back().EnemyCountSmall = stoi(sline);
			}
			else if ((read_count - 1) % KRowCountPerLevel == 3)
			{
				m_LevelSet.vLevels.back().EnemyCountNormal = stoi(sline);
			}
			else if ((read_count - 1) % KRowCountPerLevel == 4)
			{
				m_LevelSet.vLevels.back().EnemyCountBig = stoi(sline);

				m_LevelSet.vLevels.back().TotalEnemyCount = 
					m_LevelSet.vLevels.back().EnemyCountBig + 
					m_LevelSet.vLevels.back().EnemyCountNormal +
					m_LevelSet.vLevels.back().EnemyCountSmall;
			}
		}
		
		++read_count;
	}
}

SLevelData* CLevelLoader::GetLevelData(int LevelID)
{
	for (auto& level : m_LevelSet.vLevels)
	{
		if (level.LevelID == LevelID)
		{
			return &level;
		}
	}

	return nullptr;
}