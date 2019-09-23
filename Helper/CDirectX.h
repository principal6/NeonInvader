#pragma once

#include "CommonHeader.h"
#include "CShader.h"
#include "CObject2D.h"
#include "CConstantBuffer.h"

class CDirectX
{
	friend class CEntityPool;

public:
	CDirectX(HINSTANCE hInstance, const XMFLOAT2& WindowSize) : m_hInstance{ hInstance }, m_WindowSize{ WindowSize } {};
	~CDirectX() {};

	void Create(LPCTSTR Title, WNDPROC WndProc, int nShowCmd, const string& FontFileName);

	CShader* AddShader(EShaderType ShaderType, LPCWSTR ShaderFileName, LPCSTR EntryPoint,
		const D3D11_INPUT_ELEMENT_DESC* VSInputElements = nullptr, UINT VSInputElementCount = 0);
	CShader* GetShader(size_t Index);

	void UseSampler(ESampler Sampler);

	void BeginRendering(const float (&ClearColorArray)[4]);
	void RenderText(const wstring& Text, const XMFLOAT2& Position, const FXMVECTOR& Color);
	void EndRendering();

private:
	void CreateWin32Window(LPCTSTR Title, WNDPROC WndProc, int nShowCmd);
	void CreateDirect3DObjects(const string& FontFileName);

private:
	void CreateDevice();
	void CreateAndSetRenderTargetView();
	void CreateBaseSamplers();
	void CreateBaseConstantBuffers();
	void CreateImageBlendState();
	void SetViewport();

private:
	HWND							m_hWnd{};
	HINSTANCE						m_hInstance{};
	XMFLOAT2						m_WindowSize{};

	ComPtr<IDXGISwapChain>			m_SwapChain{};
	ComPtr<ID3D11Device>			m_Device{};
	ComPtr<ID3D11DeviceContext>		m_DeviceContext{};
	ComPtr<ID3D11RenderTargetView>	m_RenderTargetView{};

	vector<unique_ptr<CShader>>		m_vShaders{};

	ComPtr<ID3D11SamplerState>		m_SamplerPoint{};
	ComPtr<ID3D11SamplerState>		m_SamplerLinear{};

	ComPtr<ID3D11BlendState>		m_BlendImageTransparency{};

	XMMATRIX						m_MatrixProjection{};
	unique_ptr<CConstantBuffer>		m_CBProjection{};
	unique_ptr<CConstantBuffer>		m_CBWorld{};

	unique_ptr<SpriteBatch>			m_SpriteBatch{};
	unique_ptr<SpriteFont>			m_SpriteFont{};
};