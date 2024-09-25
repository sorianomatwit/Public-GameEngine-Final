#include "GameSystems.hpp"
#include "GameEngine.hpp"
#include "GameScene.hpp"

void ChaseSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	auto player = Entity(ComponentSet<CPlayer>::entities[0]);
	auto& playerRb = player.getComponent<CRigidBody>();
	Vec2 playerPosition = playerRb.position;

	for (int i = entities.size() - 1; i >= 0; i--) {

		auto entity = Entity(entities[i]);
		auto& rb = entity.getComponent<CRigidBody>();
		Vec2 direction = (playerPosition - rb.position).normalize();
		auto& dog = entity.getComponent<TDog>();
		if (dog.isEating) {
			dog.eatingTimer -= deltaTime;
			if (dog.eatingTimer <= 0) {
				entity.destroy();
			}
		}
		else {

			if (entity.hasComponent<CChaser>()) {
				auto& chaser = entity.getComponent<CChaser>();
				auto& animation = entity.getComponent<CAnimation>();

				if (chaser.disableMovement) {
					chaser.disableMovementTimer -= deltaTime;
					if (chaser.disableMovementTimer <= 0.0f) {
						chaser.disableMovementTimer = 0.0f;
						chaser.disableMovement = false;
					}
					continue;
				}

				Vec2 currentDirection = direction;
				float angle = currentDirection.angleBetween(Vec2::UP) * (180.0f / PI);
				if (currentDirection.x > 0) {
					angle *= -1;
				}

				rb.angle = angle;
				rb.velocity = currentDirection * chaser.speed * deltaTime;
				rb.position += rb.velocity;

				animation.animation.update();

				if (entity.hasComponent<CCollisionContainer>()) {
					auto& collisions = entity.getComponent<CCollisionContainer>().enterCollisions;

					for (auto& collision : collisions) {
						auto& otherEntity = collision.otherEntity;

						if (otherEntity.hasComponent<CPlayer>()) {
							auto& playerData = player.getComponent<CPlayer>();
							if (!playerData.powerUps[Shield].isActive) {
								player.getComponent<CHealth>().health -= 1;
								if (player.getComponent<CHealth>().health <= 0) {
									playerRb.position = Vec2(GameEngine::worldWidth * 0.5f, GameEngine::worldHeight * 0.5f);
									player.getComponent<CHealth>().health = player.getComponent<CHealth>().maxHealth;
								}

								Vec2 knockbackDirection = collision.direction;
								float knockbackMultiplier = 1050.0f;
								Vec2 playerAcceleration = collision.impactVelocity * knockbackMultiplier;
								Vec2 dogAcceleration = playerAcceleration * 0.7f;

								playerRb.velocity -= knockbackDirection * playerAcceleration.length() * deltaTime;
								rb.velocity += knockbackDirection * dogAcceleration.length() * deltaTime;
							}
							else {
								playerData.powerUps[Shield].isActive = false;
								playerData.powerUps[Shield].duration = 0;
								EntityManager::createEntity()
									.addComponent<CParticleEmitter>(CParticleEmitter(
										Animation(GameEngine::getAssets().getAnimation("Circle"), Vec2(1, 1)),
										50,
										playerPosition,
										{ 2.0f,6.0f },
										{ 0,360 },
										{ 0.1,1.5 },
										BLUE
									)).setActive(true);
							}
							chaser.disableMovementTimer = 2.0f;
							chaser.disableMovement = true;
						}
					}
				}
			}
			else if (entity.hasComponent<CBarker>()) {
				auto& barker = entity.getComponent<CBarker>();
				auto& animation = entity.getComponent<CAnimation>();

				Vec2 currentDirection = direction;
				float angle = currentDirection.angleBetween(Vec2::UP) * (180.0f / PI);
				if (currentDirection.x > 0) {
					angle *= -1;
				}

				rb.angle = angle;
				rb.velocity = currentDirection * barker.speed * deltaTime;
				rb.position += rb.velocity;


				animation.animation.update();
			}
		}
	}
}
