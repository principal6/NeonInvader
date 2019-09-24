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
	SAnimation(const string& _Name, size_t _TickPerFrame, bool _ShouldRepeat = true) : 
		Name{ _Name }, TickPerFrame{ _TickPerFrame }, ShouldRepeat{ _ShouldRepeat } {}

	string Name{};
	size_t TickPerFrame{};
	size_t CurrentFrameIndex{};
	vector<SAnimationFrame> vFrames{};
	bool ShouldRepeat{};
	bool IsOver{ false };
};

class CEntity final : public CObject2D
{
	friend class CEntityPool;
	friend class CNeonInvader;

public:
	CEntity(CDirectX* DirectX) : CObject2D(DirectX) {}
	~CEntity() {}

	void SetCollisionBox(const XMFLOAT2& HalfSize);

	SAnimation* AddAnimation(const string& AnimationName, size_t TickPerFrame = 30);
	void SetAnimation(size_t AnimationIndex, bool ForcedSet = false);
	void SetAnimation(const string& AnimationName, bool ForcedSet = false);
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
	bool		ShouldCollide{ false };

private:
	void UpdateAnimationFrame();

private:
	vector<unique_ptr<SAnimation>>	m_vAnimations{};
	size_t							m_AnimationIndex{ KAnimationIndexInvalid };
	size_t							m_AnimationTick{};
	ERenderFlipOption				m_AnimationFlip{ ERenderFlipOption::None };
	unordered_map<string, size_t>	m_AnimationIndexMap{};

	XMFLOAT2						m_FineCollisionBoxHalfSize{};
	vector<XMFLOAT2>				m_vFineCollisionBoxPoints{};
	float							m_CoarseCollisionRadius{};
	float							m_FineCollisionRadius{};
	bool							m_Collided{ false };
};