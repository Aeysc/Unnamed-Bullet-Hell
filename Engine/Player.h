#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "TileMap.h"
#include "Collider.h"
#include "Bullet.h"
#include "Mouse.h"
#include "Timer.h"
#include <memory>
#include "Anim.h"
#include "VisualEffect.h"

// Works like top down now, but can be modified
//  to use platformer controls.
class Player
{
public:
	Player( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bullets,
		std::vector<VisualEffect>& visualEffects );

	void Update( const Keyboard& kbd,const Mouse& ms,
		float dt );
	void Draw( Graphics& gfx ) const;

	void MoveTo( const Vec2& updatedPos );
	void SetJustShot( bool val );
	void SetInvulStatus( bool isInvul );
	void SetSubColor( Color c );
	void MultiplyMoveSpeedFactor( float amount );

	Vec2& GetPos();
	const Vec2& GetVel() const;
	Vec2 GetCenter() const;
	Rect GetRect() const;
	bool JustShot() const;
	bool IsInvul() const;
private:
	void Jump();
	void Land();
private:
	Vec2 pos;
	static constexpr Vei2 size = { 32,32 };
	static constexpr float speed = 171.4f;
	Collider coll;
	std::vector<std::unique_ptr<Bullet>>& myBullets;
	std::vector<VisualEffect>& visualEffects;
	Timer shotTimer = { 0.23f };
	const TileMap& map;
	static constexpr float bulletSpeed = 324.2f;
	Vec2 moveDir = { 0.0f,0.0f };
	bool jumping = false;
	Timer jumpTimer = 0.61f;
	Timer jumpReset = 1.0f;
	const Surface surfSheet = "Images/Player1Anim.bmp";
	Anim walk;
	bool justShot = false;
	bool invul = false;
	Color subColor = Colors::Magenta;
	float moveSpeedFactor = 1.0f;
	Timer footstepTimer = 0.12f;
};