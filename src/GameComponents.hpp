#pragma once
#include "SystemBase.hpp"
#include <raylib.h>
#include "EngineComponents.hpp"
#include "Utils.hpp"
#include <functional>

#pragma region TAGS

enum Difficulty {
	EASY = 0, //target score: 1000
	MED = 1, //1500
	HARD = 2, //2000

};


struct House {
	int mailAmount;
	int totalMail;
	bool isComplete;
	Vec2 position;
	std::vector<Entity> tiles;
};

class CHouse : public Component<CHouse> {
public:
	std::shared_ptr<House> house;
	bool hasSpawnPowerUp = false;
	CHouse(std::shared_ptr<House> house) : house(house) {};
	CHouse() = default;
};

enum DogType {
	CHASER = 0,
	BARKER = 1
};
class CDogHouse : public Component<CDogHouse> {
public:
	bool spawnDog = false;
	float detectionRadius = 200.0f;
	float spawnInterval;
	float timeSinceLastSpawn;
	DogType dog = CHASER;

	CDogHouse() = default;
	CDogHouse(float interval, DogType dog);
};

enum BulletType {
	MAIL = 0,
	TREAT = 1
};


class CChaser : public Component<CChaser> {
public:
	float speed = 65.0f;
	float disableMovementTimer;
	bool disableMovement = false;
	CChaser() = default;
};

class CHealth : public Component<CHealth> {
public:
	int health;
	int maxHealth;

	CHealth() = default;
	CHealth(int h, int maxH) : health(h), maxHealth(maxH) {}
};


class CBarker : public Component<CBarker> {
public:
	float barkInterval = 2.0f;
	float barkTimer = 0.0f;
	float speed = 90.0f;

	CBarker() = default;
	CBarker(float interval) : barkInterval(interval), barkTimer(0.0f) {}
};

class TBullet : public Component<TBullet> {
public:
	BulletType type = MAIL;
	TBullet() = default;
	TBullet(BulletType t) : Component<TBullet>(), type(t) {};
};



class TDog : public Component<TDog> {
	public:
		float eatingTimer = 10.0f;
		bool isEating = false;
};
class TCar : public Component<TCar> {};

#pragma endregion


#pragma region TILE
enum TILETYPE {
	GRASS = 8,
	ROAD = 9,
	CURVE_ROAD = 10,
	INTERSECTION = 15,
	DOG_HOUSE = 11,
	ROAD_BLOCK = 12,
	FENCE = -1,
	FENCE_CORNER = -2,
	House_0 = 0,
	House_1 = 1,
	House_2 = 2,
	House_3 = 3
};

enum DIRECTION {
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3
};

typedef std::vector<TILETYPE> TypeEdges;

struct TileInfo {
public:
	DIRECTION direction;
	TILETYPE type;
	TypeEdges edges;
	int weight;
};

class TileNode {
public:
	static inline DIRECTION getOpposite(DIRECTION d) {
		if (d == UP) return DOWN;
		if (d == DOWN) return UP;
		if (d == LEFT) return RIGHT;
		if (d == RIGHT) return LEFT;
	}
	bool isSet = false;
	TileInfo info;
	std::vector<TileInfo> availableTiles;
	std::array<std::shared_ptr<TileNode>, 4> adjacentTiles;
	TileNode();
	void addRotations(TileInfo base);
	TileInfo decide(TileInfo info);
	TileInfo decide();
	bool decide(Vec2 cell, int width, int height, std::vector<TileNode>& allTiles);
	void repopulate();

	bool clean(TileInfo tile, DIRECTION dir);


};

class CTile : public Component<CTile> {
public:
	TileNode tile;
	CTile();
	CTile(TileNode t);
};
#pragma endregion


struct Collision {
	Entity otherEntity;
	Vec2 impactVelocity;
	Vec2 direction;
	float overlap;
};

class CCollisionContainer : public Component<CCollisionContainer> {
public:
	std::vector<Collision> enterCollisions;
	std::vector<Collision> exitCollisions;
};

class CCarController : public Component<CCarController> {
public:

	CCarController() = default;
	CCarController(float carAcc, float tspeed, float basedrag) : carAcceleration(carAcc), turnSpeed(tspeed), baseDragC(basedrag), dragC(baseDragC) {};
	float steerAngle = 0.0f;
	float carAcceleration;
	float turnSpeed;
	float baseDragC;
	float dragC;
	float impactReduction = 0.25f;
};

enum PowerUpType {
	TripleShot = 0,
	SpeedBoost = 1,
	Shield = 2,
	ExtraHealth = 3,
	NONE
};

struct PowerUp {
	bool isActive;
	PowerUpType type;
	float duration;
	float probability;
};

class CPowerUp : public Component<CPowerUp> {
private:
	friend class PlayerUpdateGuiSystem;
	std::vector<PowerUp> allPowerUps = {
		{ true, TripleShot,10.0f,30 },
		{ true, SpeedBoost,10.0f,30 },
		{ true, Shield,10.0f,20 },
		{ true, ExtraHealth,10.0f,10 },
		{true, NONE, 0.0,0}
	};
public:
	PowerUp powerUp;
	CPowerUp();
};

class CPlayer : public Component<CPlayer> {
public:
	//Power Up stuff
	std::vector<PowerUp> powerUps = {
		{ false, TripleShot,0,0 },
		{ false, SpeedBoost,0,0 },
		{ false, Shield,0,0 },
		{ false, ExtraHealth,0,0 },
	};

	Entity shield = Entity(UINT16_MAX);
	Entity canon = Entity(UINT16_MAX);

	//Bullet stuff
	BulletType currentAmmo = MAIL;
	float shootCoolDownAmount[2] = { 1.0f, 1.0f };
	float shootCoolDownTimer[2] = { 0.0f, 0.0f };
	float bulletSpeed = 10;

	//Stun dog stuff
	float stunTimer = 0.0f;
	bool stunned = false;
	int lives = 3;

	float totalHouse = 0;
	float housesCompleted = 0;

	//Gui Stuff
	enum PlayerGui {
		AmmoIconBG = 0,
		AmmoIcon = 1,
		AmmoIconOverlay = 2,
		TripleIconBG = 3,
		TripleIcon = 4,
		TripleIconOverlay = 5,
		SpeedIconBG = 6,
		SpeedIcon = 7,
		SpeedIconOverlay = 8,
		ShieldUpIconBG = 9,
		ShieldUpIcon = 10,
		ShieldIconOverlay = 11,
		HousesLeftBG = 12,
		HousesLeftProgress = 13,
		ClosestHouse = 14,
		HpBarGray = 15,
		HpBarWhite = 16,
	};
	CPlayer() : Component<CPlayer>() {
		canon = EntityManager::createEntity();
		shield = EntityManager::createEntity();
	}
};

class CMenuOption : public Component<CMenuOption> {
public:
	std::vector<Entity> menuOptions;
	CMenuOption() = default;
	template <typename ...Args>
	CMenuOption(Args ...options) : menuOptions{ options... } {};
};

