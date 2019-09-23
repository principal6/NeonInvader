#pragma once

#include "CommonHeader.h"
#include "../Helper/CTexture.h"

class CDirectX;

enum ERenderFlipOption
{
	None,
	Horizontal,
	Vertical,
	HorizontalAndVertical
};

struct SVertex2D
{
	SVertex2D() {};
	SVertex2D(const XMFLOAT2& _Position) : Position{ _Position } {};
	SVertex2D(const XMFLOAT2& _Position, const XMFLOAT4& _Color) : Position{ _Position }, Color{ _Color } {};
	SVertex2D(const XMFLOAT2& _Position, const XMFLOAT2& _TexCoord) : Position{ _Position }, TexCoord{ _TexCoord } {};

	XMFLOAT2 Position{};
	XMFLOAT4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT2 TexCoord{};
};

struct SIndex
{
	SIndex() {};
	SIndex(DWORD _0, DWORD _1, DWORD _2) : I0{ _0 }, I1{ _1 }, I2{ _2 } {};

	DWORD I0{}, I1{}, I2{};
};

class CObject2D
{
public:
	CObject2D(CDirectX* DirectX) : m_DirectX{ DirectX } { assert(m_DirectX); }
	~CObject2D() {}

	virtual void Create(const vector<SVertex2D>& vVertices, const vector<SIndex>& vIndices, bool IsDynamic = true);
	virtual void CreateRectangle(const XMFLOAT2& RectangleSize, bool IsDynamic = true);

	virtual void SetRectangleUVRange(const XMFLOAT2& OffsetInTexture, const XMFLOAT2& SizeInTexture, ERenderFlipOption Flip = ERenderFlipOption::None);
	virtual void SetRectangleUVRangeAndSize(const XMFLOAT2& OffsetInTexture, const XMFLOAT2& SizeInTexture, ERenderFlipOption Flip = ERenderFlipOption::None);

	virtual void SetTexture(CTexture* SharedTexture);

	virtual void Draw();

protected:
	virtual void UpdateVertexBuffer();

public:
	ESampler				Sampler{ ESampler::Point };
	XMFLOAT2				WorldPosition{};
	float					RotationAngle{};
	XMFLOAT2				Scalar{ 1.0f, 1.0f };

protected:
	CDirectX*				m_DirectX{};

	ComPtr<ID3D11Buffer>	m_VertexBuffer{};
	ComPtr<ID3D11Buffer>	m_IndexBuffer{};

	UINT					m_VertexBufferStride{ sizeof(SVertex2D) };
	UINT					m_VertexBufferOffset{};

	vector<SVertex2D>		m_Vertices{};
	vector<SIndex>			m_Indices{};

	CTexture*				m_PtrSharedTexture{};

	XMMATRIX				m_MatrixWorld{ XMMatrixIdentity() };
};
