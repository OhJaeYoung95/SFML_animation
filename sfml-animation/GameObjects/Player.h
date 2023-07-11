#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player : public SpriteGo
{
protected:
	AnimationController animation;
	sf::Vector2f dir = { 0, 0 };
	float velocity = 0.f;
	float jumpForce = 380.f;
	float gravity = 98.f;
	float jumpSpeed = 2.f;
	float speed = 100.f;

	bool isMove = true;
	bool isJump = false;
	bool isGround = false;

	bool flipX;
public:
	Player(const std::string& textureId = "", const std::string& n = "") 
		: SpriteGo(textureId, n) {};
	virtual ~Player() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;

	void SetIsGround(bool isGround) { this->isGround = isGround; }
	bool GetFlipX() { return flipX; }
	bool SetFlipX(bool flip);
};

