#pragma once

#include "../Helper/CDirectX.h"
#include "../Helper/CObject2D.h"

static constexpr size_t KAnimationIndexInvalid{ std::string::npos };

struct SAnimationFrame
{
	SAnimationFrame() {}
	SAnimationFrame(const XMFLOAT2& _UVOffset, const XMFLOAT2& _UVSize) : UVOffset{ _UVOffset }, UVSize{ _UVSize } {}

	XMFLOAT2 UVOffset{};
	XMFLOAT2 UVSize{};
};

struct SAnimation
{
	SAnimation(const string& _Name, size_t _TickPerFrame) : Name{ _Name }, TickPerFrame{ _TickPerFrame } {}

	string Name{};
	size_t TickPerFrame{};
	size_t CurrentFrameIndex{};
	vector<SAnimationFrame> vFrames{};
};

class CEntity final : public CObject2D
{
	friend class CEntityPool;

public:
	CEntity(CDirectX* DirectX) : CObject2D(DirectX) {}
	~CEntity() {}

	SAnimation* AddAnimation(const string& AnimationName, size_t TickPerFrame = 30);
	void SetAnimation(size_t AnimationIndex);
	void SetAnimation(const string& AnimationName);
	void SetAnimationFlipping(ERenderFlipOption Flip);
	void Animate();

public:
	void MoveForward(float DistanceFactor = 1.0f);
	void MoveBackward(float DistanceFactor = 1.0f);
	void MoveLeft(float DistanceFactor = 1.0f);
	void MoveRight(float DistanceFactor = 1.0f);
	void SetLinearVelocity(const XMVECTOR& Velocity);

public:
	XMFLOAT2	LinearVelocity{};
	bool		Visible{ true };

private:
	void UpdateAnimationFrame();

private:
	vector<unique_ptr<SAnimation>>	m_vAnimations{};
	size_t							m_AnimationIndex{ KAnimationIndexInvalid };
	size_t							m_AnimationTick{};
	ERenderFlipOption				m_AnimationFlip{ ERenderFlipOption::None };
	unordered_map<string, size_t>	m_AnimationIndexMap{};
};