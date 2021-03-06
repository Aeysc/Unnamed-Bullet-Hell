#pragma once

#include "EnemyBase.h"
#include "Bullet.h"

class WormBugBoss
	:
	public EnemyBase
{
private:
	enum class State
	{
		Wander,
		DigDown,
		Tunnel,
		DigUp,
		Explode
	};
public:
	WormBugBoss( const Vec2& pos,const TileMap& map,
		std::vector<std::unique_ptr<Bullet>>& bullets,
		std::vector<std::unique_ptr<EnemyBase>>& enemies );

	void Update( const EnemyUpdateInfo& info,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( int damage,const Vec2& loc ) override;

	bool IsBoss() const override;
private:
	static constexpr Vei2 size = { 128,128 };
	static constexpr int health = 350;
	std::vector<std::unique_ptr<Bullet>>* pBulletVec;
	std::vector<std::unique_ptr<EnemyBase>>* pEnemyVec;
	CSurfPtr surfSheet = SurfCodex::Fetch( "Images/WormBugBoss.bmp" );
	Anim wandering;
	Anim diggingDown;
	Anim diggingUp;
	Anim exploding;
	State action = State::Wander;
	static constexpr float moveRange = 400.0f;
	static constexpr float moveSpeed = 210.0f;
	static constexpr float bulletSpeed = 300.0f;
	Timer wanderTimer = 10.0f;
	static constexpr float tunnelSpeed = 500.0f;
	static constexpr int bugSpawnChance = 25;
};