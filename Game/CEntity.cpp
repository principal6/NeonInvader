#include "CEntity.h"

SAnimation* CEntity::AddAnimation(const string& AnimationName, size_t TickPerFrame)
{
	m_AnimationIndexMap[AnimationName] = m_vAnimations.size();

	m_vAnimations.emplace_back(make_unique<SAnimation>(AnimationName, TickPerFrame));

	return m_vAnimations.back().get();
}

void CEntity::SetAnimation(size_t AnimationIndex)
{
	if (m_AnimationIndex != AnimationIndex)
	{
		m_AnimationIndex = AnimationIndex;
		m_vAnimations[m_AnimationIndex]->CurrentFrameIndex = 0;
		m_AnimationTick = 0;

		UpdateAnimationFrame();
	}
}

void CEntity::SetAnimation(const string& AnimationName)
{
	SetAnimation(m_AnimationIndexMap[AnimationName]);
}

void CEntity::SetAnimationFlipping(ERenderFlipOption Flip)
{
	m_AnimationFlip = Flip;

	UpdateAnimationFrame();
}

void CEntity::Animate()
{
	++m_AnimationTick;

	if (m_AnimationTick >= m_vAnimations[m_AnimationIndex]->TickPerFrame)
	{
		++m_vAnimations[m_AnimationIndex]->CurrentFrameIndex;
		if (m_vAnimations[m_AnimationIndex]->CurrentFrameIndex >= m_vAnimations[m_AnimationIndex]->vFrames.size())
		{
			m_vAnimations[m_AnimationIndex]->CurrentFrameIndex = 0;
		}

		UpdateAnimationFrame();

		m_AnimationTick = 0;
	}
}

void CEntity::UpdateAnimationFrame()
{
	auto& current_frame_index{ m_vAnimations[m_AnimationIndex]->CurrentFrameIndex };
	auto& current_frame{ m_vAnimations[m_AnimationIndex]->vFrames[current_frame_index] };
	SetRectangleUVRange(current_frame.UVOffset, current_frame.UVSize, m_AnimationFlip);
}

void CEntity::MoveForward(float DistanceFactor)
{
	XMVECTOR vec_dir{ 0.0f, +1.0f, 0.0f, 0.0f };
	vec_dir *= DistanceFactor;

	XMMATRIX mat_rot{ XMMatrixRotationZ(RotationAngle) };
	vec_dir = XMVector2TransformNormal(vec_dir, mat_rot);

	WorldPosition.x += XMVectorGetX(vec_dir);
	WorldPosition.y += XMVectorGetY(vec_dir);
}

void CEntity::MoveBackward(float DistanceFactor)
{
	XMVECTOR vec_dir{ 0.0f, +1.0f, 0.0f, 0.0f };
	vec_dir *= DistanceFactor;

	XMMATRIX mat_rot{ XMMatrixRotationZ(RotationAngle) };
	vec_dir = XMVector2TransformNormal(vec_dir, mat_rot);

	WorldPosition.x -= XMVectorGetX(vec_dir);
	WorldPosition.y -= XMVectorGetY(vec_dir);
}

void CEntity::MoveLeft(float DistanceFactor)
{
	XMVECTOR vec_dir{ -1.0f, 0.0f, 0.0f, 0.0f };
	vec_dir *= DistanceFactor;

	XMMATRIX mat_rot{ XMMatrixRotationZ(RotationAngle) };
	vec_dir = XMVector2TransformNormal(vec_dir, mat_rot);

	WorldPosition.x += XMVectorGetX(vec_dir);
	WorldPosition.y += XMVectorGetY(vec_dir);
}

void CEntity::MoveRight(float DistanceFactor)
{
	XMVECTOR vec_dir{ +1.0f, 0.0f, 0.0f, 0.0f };
	vec_dir *= DistanceFactor;

	XMMATRIX mat_rot{ XMMatrixRotationZ(RotationAngle) };
	vec_dir = XMVector2TransformNormal(vec_dir, mat_rot);

	WorldPosition.x += XMVectorGetX(vec_dir);
	WorldPosition.y += XMVectorGetY(vec_dir);
}

void CEntity::SetLinearVelocity(const XMVECTOR& Velocity)
{
	LinearVelocity.x = XMVectorGetX(Velocity);
	LinearVelocity.y = XMVectorGetY(Velocity);
}