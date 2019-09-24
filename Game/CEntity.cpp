#include "CEntity.h"

void CEntity::SetCollisionBox(const XMFLOAT2& HalfSize)
{
	m_FineCollisionBoxHalfSize = HalfSize;

	m_vFineCollisionBoxPoints.clear();
	m_vFineCollisionBoxPoints.emplace_back(XMFLOAT2(-HalfSize.x, +HalfSize.y));
	m_vFineCollisionBoxPoints.emplace_back(XMFLOAT2(+HalfSize.x, +HalfSize.y));
	m_vFineCollisionBoxPoints.emplace_back(XMFLOAT2(+HalfSize.x, -HalfSize.y));
	m_vFineCollisionBoxPoints.emplace_back(XMFLOAT2(-HalfSize.x, -HalfSize.y));

	m_FineCollisionRadius = min(HalfSize.x, HalfSize.y);
	m_CoarseCollisionRadius = XMFLOAT2GetLength(HalfSize);

	ShouldCollide = true;
}

SAnimation* CEntity::AddAnimation(const string& AnimationName, size_t TickPerFrame)
{
	m_AnimationIndexMap[AnimationName] = m_vAnimations.size();

	m_vAnimations.emplace_back(make_unique<SAnimation>(AnimationName, TickPerFrame));

	return m_vAnimations.back().get();
}

void CEntity::SetAnimation(size_t AnimationIndex, bool ForcedSet)
{
	if (m_AnimationIndex != AnimationIndex || ForcedSet)
	{
		m_AnimationIndex = AnimationIndex;
		m_vAnimations[m_AnimationIndex]->CurrentFrameIndex = 0;
		m_vAnimations[m_AnimationIndex]->IsOver = false;
		m_AnimationTick = 0;

		UpdateAnimationFrame();
	}
}

void CEntity::SetAnimation(const string& AnimationName, bool ForcedSet)
{
	SetAnimation(m_AnimationIndexMap[AnimationName], ForcedSet);
}

void CEntity::SetAnimationFlipping(ERenderFlipOption Flip)
{
	m_AnimationFlip = Flip;

	UpdateAnimationFrame();
}

void CEntity::Animate()
{
	++m_AnimationTick;

	auto& current_animation{ m_vAnimations[m_AnimationIndex] };
	if (m_AnimationTick >= current_animation->TickPerFrame)
	{
		if (!current_animation->IsOver)
		{
			++current_animation->CurrentFrameIndex;

			if (current_animation->CurrentFrameIndex >= current_animation->vFrames.size())
			{
				if (current_animation->ShouldRepeat)
				{
					current_animation->CurrentFrameIndex = 0;
				}
				else
				{
					current_animation->CurrentFrameIndex = current_animation->vFrames.size() - 1;
					current_animation->IsOver = true;
				}
			}

			UpdateAnimationFrame();
		}

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