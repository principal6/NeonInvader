#include "CStageLoader.h"

void CStageSetLoader::LoadStageSetFromFile(const string& StageSetFileName)
{
	static constexpr int KRowCountPerStage{ 5 };
	ifstream ifs{ StageSetFileName };
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
			m_StageSet.vStages.reserve(stoi(sline));
		}
		else
		{
			if ((read_count - 1) % KRowCountPerStage == 0)
			{
				m_StageSet.vStages.emplace_back();
				m_StageSet.vStages.back().StageID = stoi(sline);
			}
			else if ((read_count - 1) % KRowCountPerStage == 1)
			{
				m_StageSet.vStages.back().EnemySpeedFactor = stof(sline);
			}
			else if ((read_count - 1) % KRowCountPerStage == 2)
			{
				m_StageSet.vStages.back().EnemyCountSmall = stoi(sline);
			}
			else if ((read_count - 1) % KRowCountPerStage == 3)
			{
				m_StageSet.vStages.back().EnemyCountNormal = stoi(sline);
			}
			else if ((read_count - 1) % KRowCountPerStage == 4)
			{
				m_StageSet.vStages.back().EnemyCountBig = stoi(sline);

				m_StageSet.vStages.back().TotalEnemyCount = 
					m_StageSet.vStages.back().EnemyCountBig + 
					m_StageSet.vStages.back().EnemyCountNormal +
					m_StageSet.vStages.back().EnemyCountSmall;
			}
		}
		
		++read_count;
	}
}