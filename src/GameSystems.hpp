
#pragma once
#include "EngineComponents.hpp"
#include "GameComponents.hpp"


class PlayerMovementSystem : public System<CPlayer> {
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};


class PlayerDrivingSystem : public System<CPlayer> {
public:
	inline static bool enableGrassDrag = true;
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

class PlayerShootingSystem : public System<CPlayer> {
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

class StunSystem : public System<CBarker> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};


class CameraFollowPlayerSystem : public System<CPlayer> {
public:
	inline static bool goPassWorldBoundary = false;
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

class ChaseSystem : public System<CChaser> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};


class CollectCollisionSystem : public System<CRigidBody> {
public:
	inline static int collidableEntityCount = 0;
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

class CleanCollisionSystem : public System<CCollisionContainer> {
	void Execute(std::vector<uint16_t> entites, float deltatime) override;

};

class SpawnMailSystem : public System<CHouse> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class SpawnDogSystem : public System<CDogHouse> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class MenuActionSystem : public System<CMenuOption> {
protected:
	int menuIndex = 0;
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class BulletMovementSystem : public System<TBullet> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class SyncBodiesSystem : public System<CRigidBody> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class PowerUpCollectionSystem : public System<CPowerUp> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class PowerUpTimerSystem : public System<CPlayer> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class PlayerUpdateGuiSystem : public System<CPlayer> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class UpdateHouseGuiSystem : public System<CHouse> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class PowerUpMovementSystem : public System<CPowerUp> {
protected:
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};

class GameOverSystem : public System<CMenuOption> {
protected:
	int menuIndex = 0;
	void Execute(std::vector<uint16_t> entities, float deltaTime) override;
};