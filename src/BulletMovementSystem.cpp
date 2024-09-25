#pragma once
#include "GameSystems.hpp"
#include "GameEngine.hpp"
#include "GameScene.hpp"
#include "GameOverScene.hpp"

void BulletMovementSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	for (int i = entities.size() - 1; i >= 0; i--) {
		auto bullet = Entity(entities[i]);
		auto& rbData = bullet.getComponent<CRigidBody>();
		auto& bulletData = bullet.getComponent<TBullet>();

		rbData.position += rbData.velocity;

		Rect world = {
			Vec2(0,0),
			Vec2(GameEngine::worldWidth,GameEngine::worldHeight)
		};
		Circ shot{
			rbData.position,
			rbData.colliders[0].radius
		};
		if (!RectContainsCirc(world, shot)) {
			bullet.destroy();
			continue;
		}

		if (bulletData.type == TREAT && bullet.hasComponent<CCollisionContainer>()) {
			auto& collisions = bullet.getComponent<CCollisionContainer>().enterCollisions;
			for (auto& collision : collisions) {
				auto& otherEntity = collision.otherEntity;
				if (otherEntity.hasComponent<TDog>()) {
					auto& dogHealth = otherEntity.getComponent<CHealth>();
						otherEntity.getComponent<TDog>().isEating = true;
					//Spawn Heart particles
					EntityManager::createEntity().addComponent<CParticleEmitter>(
						CParticleEmitter(
							Animation(GameEngine::getAssets().getAnimation("Heart"), Vec2(0.35, 0.35)),
							25,
							rbData.position,
							{ std::max(0.0f,collision.impactVelocity.length() - 3),collision.impactVelocity.length() },
							{ 0,360},
							{ 1.0f, 5 },
							WHITE
						)).setActive(true);

					auto& animation = otherEntity.getComponent<CAnimation>().animation;
					if (otherEntity.hasComponent<CChaser>()) {
						animation = GameEngine::getAssets().getAnimation("Chaser_eating");
					}
					else
						animation = GameEngine::getAssets().getAnimation("Barker_eating");
					bullet.destroy();
					break;
				}
			}
		}

		if (bulletData.type == MAIL && bullet.hasComponent<CCollisionContainer>()) {
			auto& collisions = bullet.getComponent<CCollisionContainer>().enterCollisions;
			for (auto& collision : collisions) {
				auto& otherEntity = collision.otherEntity;
				if (otherEntity.hasComponent<CHouse>()) {
					auto& cTile = otherEntity.getComponent<CTile>();
					auto& houseData = otherEntity.getComponent<CHouse>();
					if (cTile.tile.info.type == House_0 || cTile.tile.info.type == House_1) {
						if (!houseData.house->isComplete) {
							//HOUSE IS COMPLETED
							houseData.house->mailAmount--;
							if (houseData.house->mailAmount <= 0) {
								houseData.house->isComplete = true;
								auto& animation = otherEntity.getComponent<CAnimation>();
								std::mt19937 gen(std::time(nullptr));
								// Define a uniform distribution between 0 and 360
								std::uniform_real_distribution<> distrib(0.0f, 360.0f);
								// Generate a random number
								float random_number = distrib(gen);
								Vec2 randomDir = Vec2::UP.rotateRad(random_number * (PI / 180)).normalize();

								GameScene::createPowerUp(houseData.house->position);

								auto player = Entity(ComponentSet<CPlayer>::entities[0]);
								auto& playerData = player.getComponent<CPlayer>();
								playerData.housesCompleted++;
								if (playerData.housesCompleted == playerData.totalHouse) {
									GameOverScene::EndingText = "YOU WIN!!";
									GameEngine::changeScene(GAMEOVER);
								}
							}
						}
					bullet.destroy();
					}
				}
			}
		}
	}
}
