#pragma once

#include <fstream>
#include <windows.h>
#include <cassert>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <unordered_map>
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include "../DirectXTK/pch.h"
#include "../FMOD/fmod.hpp"

#pragma comment(lib, "fmodL_vc.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using std::ifstream;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::wstring;
using std::to_string;
using std::to_wstring;
using std::stoi;
using std::stof;
using std::chrono::steady_clock;
using std::unordered_map;
using std::pair;
using std::make_pair;

enum class ESampler
{
	Point,
	Linear
};

enum class EShaderType
{
	Invalid,

	VertexShader,
	PixelShader
};

static XMFLOAT2 operator*(const XMFLOAT2& a, float s)
{
	return XMFLOAT2(a.x * s, a.y * s);
}

static XMFLOAT2 operator/(const XMFLOAT2& a, float s)
{
	return XMFLOAT2(a.x / s, a.y / s);
}

static XMFLOAT2 operator+(const XMFLOAT2& a, const XMFLOAT2& b)
{
	return XMFLOAT2(a.x + b.x, a.y + b.y);
}

static XMFLOAT2 operator-(const XMFLOAT2& a, const XMFLOAT2& b)
{
	return XMFLOAT2(a.x - b.x, a.y - b.y);
}

static float XMFLOAT2GetLength(const XMFLOAT2& a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

static XMFLOAT2 XMFLOAT2Normalize(const XMFLOAT2& a)
{
	float length{ XMFLOAT2GetLength(a) };
	return (a / length);
}