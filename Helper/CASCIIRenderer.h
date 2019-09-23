#pragma once

#include "CObject2D.h"

struct SCharacterSetInfo
{
	string TextureFileName{};
	int NumCols{};
	int NumRows{};
	XMFLOAT2 CharacterSize{};
	unordered_map<int, int> ASCIIMap{};
};

class CASCIIRenderer final : protected CObject2D
{
public:
	CASCIIRenderer(CDirectX* DirectX) : CObject2D(DirectX) {}
	~CASCIIRenderer() {}

	void Create(int MaxTextLength, const string& AssetDir, const string& CharsetInfoFileName, float SpacingFactor = 1.0f);

	void RenderText(const string& Text, const XMFLOAT2 Position, float ScalingFactor = 1.0f);

private:
	void ParseCharsetInfo(const string& CharsetInfoFileName);

private:
	static constexpr int KDefaultMaxTextLength{ 300 };

	int						m_MaxTextLength{ KDefaultMaxTextLength };
	SCharacterSetInfo		m_CharacterSetInfo{};

	unique_ptr<CTexture>	m_ASCIITexture{};
};