#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Utils.h"

void Player::Init()
{
	textureId = "graphics/sprite_sheet.png";
	// Idle
	{
		AnimationClip clip;
		clip.id = "Idle";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;

		sf::IntRect coord(0, 0, 120, 120);
		int count = 0;
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
		int count = 0;
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({ textureId, coord });
			coord.left += coord.width;
		}
		clip.frames.push_back({ textureId, sf::IntRect(0, 240, 120, 120)});

		animation.AddClip(clip);
	}
	// Jump
	{
		AnimationClip clip;
		clip.id = "Jump";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;

		sf::IntRect coord(0, 360, 120, 120);
		int count = 0;
		for (int i = 0; i < 7; ++i)
		{
			clip.frames.push_back({ textureId, coord });
			coord.left += coord.width;
		}

		clip.frames[6].action = []() {
			std::cout << "On Complete Jump Clip" << std::endl;
		};

		animation.AddClip(clip);
	}

	animation.SetTarget(&sprite);

	SetOrigin(Origins::BC);

}

void Player::Reset()
{
	animation.Play("Idle");
	SetPosition(0, 0);
	SetOrigin(origin);
}

void Player::Update(float dt)
{
	animation.Update(dt);

	//float h = INPUT_MGR.GetAxis(Axis::Horizontal);

	//// 플립
	//if (h != 0.f)
	//{
	//	bool flip = h < 0.f;
	//	if (GetFlipX() != flip)
	//	{
	//		SetFlipX(flip);
	//	}
	//}

	//// 점프
	//if (isGround && INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	//{
	//	velocity.y += JumpForce;
	//	animation.Play("Jump");
	//	isGround = false;
	//}

	//// 이동
	//velocity.x = h * speed;
	//velocity.y += gravity  dt;
	//position += velocity * dt;

	//if (position.y > 0.f)
	//{
	//	isGround = true;
	//	position.y = 0.f;
	//	velocity.y = 0.f;
	//}

	// 애니메이션



	dir.x = INPUT_MGR.GetAxisRaw(Axis::Horizontal);


	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		velocity = jumpForce;
		animation.Play("Jump");
		isJump = true;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::A) && !isJump)
	{
		animation.Play("Move");
		sprite.setScale(-1.0f, 1.f);
		isMove = true;
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::D) && !isJump)
	{
		animation.Play("Move");
		sprite.setScale(1.0f, 1.f);
		isMove = true;
	}
	if (!isJump && !isMove)
	{
		animation.Play("Idle");
	}



	position += dir * dt * speed;
	velocity -= gravity * dt * (jumpSpeed+4.f);
	position.y -= velocity * dt * jumpSpeed;



	position.y += gravity * dt;
	if (position.y >= 210.f)
	{
		isJump = false;
		velocity = 0;
		position.y = 210.f;
	}
	SetPosition(position);

	SpriteGo::Update(dt);
}