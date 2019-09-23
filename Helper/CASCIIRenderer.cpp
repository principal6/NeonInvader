#include "CDirectX.h"
#include "CASCIIRenderer.h"

void CASCIIRenderer::Create(int MaxTextLength, const string& AssetDir, const string& CharsetInfoFileName, float SpacingFactor)
{
	m_MaxTextLength = MaxTextLength;

	ParseCharsetInfo(AssetDir + CharsetInfoFileName);

	m_ASCIITexture = make_unique<CTexture>(m_DirectX->GetDevicePtr(), m_DirectX->GetDeviceContextPtr());
	m_ASCIITexture->CreateTexture(AssetDir + m_CharacterSetInfo.TextureFileName);
	SetTexture(m_ASCIITexture.get());

	float width{ m_CharacterSetInfo.CharacterSize.x };
	float height{ m_CharacterSetInfo.CharacterSize.y };

	vector<SVertex2D> vertices{};
	for (int i = 0; i < m_MaxTextLength; ++i)
	{
		vertices.emplace_back(XMFLOAT2(width * (SpacingFactor * i), 0.0f), XMFLOAT2(0.0f, 0.0f));
		vertices.emplace_back(XMFLOAT2(width * (SpacingFactor * i) + width, 0.0f), XMFLOAT2(0.0f, 0.0f));
		vertices.emplace_back(XMFLOAT2(width * (SpacingFactor * i), -height), XMFLOAT2(0.0f, 0.0f));
		vertices.emplace_back(XMFLOAT2(width * (SpacingFactor * i) + width, -height), XMFLOAT2(0.0f, 0.0f));
	}

	vector<SIndex> indices{};
	for (int i = 0; i < m_MaxTextLength; ++i)
	{
		indices.emplace_back(i * 4 + 0, i * 4 + 1, i * 4 + 2);
		indices.emplace_back(i * 4 + 1, i * 4 + 3, i * 4 + 2);
	}
	
	CObject2D::Create(vertices, indices, true);
}

void CASCIIRenderer::ParseCharsetInfo(const string& CharsetInfoFileName)
{
	ifstream ifs{ CharsetInfoFileName };
	assert(ifs.is_open());
	
	int read_count{};
	int character_index{};
	while (!ifs.eof())
	{
		char line[300]{};
		ifs.getline(line, 300);

		string sline{ line };

		if (sline[0] == '#') continue;

		switch (read_count)
		{
		case 0:
			m_CharacterSetInfo.TextureFileName = sline;
			break;
		case 1:
			m_CharacterSetInfo.NumCols = stoi(sline);
			break;
		case 2:
			m_CharacterSetInfo.NumRows = stoi(sline);
			break;
		case 3:
			m_CharacterSetInfo.CharacterSize.x = stof(sline);
			break;
		case 4:
			m_CharacterSetInfo.CharacterSize.y = stof(sline);
			break;
		default:
			for (int i = 0; i < m_CharacterSetInfo.NumCols; ++i)
			{
				int id{ stoi(sline.substr((size_t)3 * i, 3)) };
				m_CharacterSetInfo.ASCIIMap[id] = character_index;

				++character_index;
			}
			break;
		}
		++read_count;
	}
}

void CASCIIRenderer::RenderText(const string& Text, const XMFLOAT2 Position, float ScalingFactor)
{
	assert(Text.length() < m_MaxTextLength);

	Scalar.x = ScalingFactor;
	Scalar.y = ScalingFactor;
	WorldPosition = Position;

	for (auto& v : m_Vertices)
	{
		v.TexCoord.x = 0.0f;
		v.TexCoord.y = 0.0f;
	}

	for (size_t i = 0; i < Text.length(); ++i)
	{
		auto& c{ Text[i] };
		int character_index{};
		if (m_CharacterSetInfo.ASCIIMap.find(c) == m_CharacterSetInfo.ASCIIMap.end())
		{
			character_index = 0;
		}
		else
		{
			character_index = m_CharacterSetInfo.ASCIIMap[c];
		}
		
		int x{ character_index % m_CharacterSetInfo.NumCols };
		int y{ character_index / m_CharacterSetInfo.NumCols };

		float u0{ (m_CharacterSetInfo.CharacterSize.x * x) / m_TextureSize.x };
		float v0{ (m_CharacterSetInfo.CharacterSize.y * y) / m_TextureSize.y };
		float u1{ u0 + m_CharacterSetInfo.CharacterSize.x / m_TextureSize.x };
		float v1{ v0 + m_CharacterSetInfo.CharacterSize.y / m_TextureSize.y };
		
		m_Vertices[i * 4 + 0].TexCoord.x = u0;
		m_Vertices[i * 4 + 0].TexCoord.y = v0;

		m_Vertices[i * 4 + 1].TexCoord.x = u1;
		m_Vertices[i * 4 + 1].TexCoord.y = v0;

		m_Vertices[i * 4 + 2].TexCoord.x = u0;
		m_Vertices[i * 4 + 2].TexCoord.y = v1;

		m_Vertices[i * 4 + 3].TexCoord.x = u1;
		m_Vertices[i * 4 + 3].TexCoord.y = v1;
	}

	CObject2D::UpdateVertexBuffer();

	CObject2D::Draw();
}