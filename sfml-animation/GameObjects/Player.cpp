#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Framework.h"


void Player::Init()
{
	std::string textureId = "graphics/sprite_sheet.png";

	// Idle
	{
		AnimationClip clip;
		clip.id = "Idle";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;

		sf::IntRect coord(0, 0, 120, 120);
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({ textureId, coord });
			coord.left += coord.width;
		}

		animation.AddClip(clip);
	}

	// Move
	{
		AnimationClip clip;
		clip.id = "Move";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;

		sf::IntRect coord(0, 120, 120, 120);
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({ textureId, coord });
			coord.left += coord.width;
		}
		clip.frames.push_back({ textureId, sf::IntRect(0, 240, 120, 120) });
		animation.AddClip(clip);
	}

	// Jump
	{
		AnimationClip clip;
		clip.id = "Jump";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;

		sf::IntRect coord(0, 360, 120, 120);
		for (int i = 0; i < 7; ++i)
		{
			clip.frames.push_back({ textureId, coord });
			coord.left += coord.width;
		}

		//clip.frames[6].action = []() {
		//	std::cout << "On Complete Jump Clip" << std::endl;
		//};

		animation.AddClip(clip);
	}

	animation.SetTarget(&sprite);

	SetOrigin(Origins::BC);
}

void Player::Reset()
{
	animation.Play("Idle");
	SetOrigin(origin);
	SetPosition({ 0, 0 });
	SetFlipX(false);
}

void Player::Update(float dt)
{
	animation.Update(dt);
	float h = INPUT_MGR.GetAxis(Axis::Horizontal);

	// 플립
	if (h != 0.f)
	{
		bool flip = h < 0.f;
		if (GetFlipX() != flip)
		{
			SetFlipX(flip);
		}
	}

	// 점프
	if (isGround && INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		velocity.y += JumpForce;
		animation.Play("Jump");
		isGround = false;
	}

	// 이동
	velocity.x = h * speed;
	velocity.y += gravity * dt;
	position += velocity * dt;

	// 바닥 충돌 처리
	if (position.y > 0.f)
	{
		isGround = true;
		position.y = 0.f;
		velocity.y = 0.f;
	}

	SetPosition(position);

	// 에니메이션
	if (animation.GetCurrentClipId() == "Idle")
	{
		if (isGround && h != 0.f)
		{
			animation.Play("Move");
		}
	}
	else if (animation.GetCurrentClipId() == "Move")
	{
		if (isGround && h == 0.f)
		{
			animation.Play("Idle");
		}
	}
	else if (animation.GetCurrentClipId() == "Jump")
	{
		if (isGround)
		{
			animation.Play((h == 0.f) ? "Idle" : "Move");
		}
	}
}

bool Player::GetFlipX() const
{
	return filpX;
}

void Player::SetFlipX(bool filp)
{
	filpX = filp;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !filpX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}