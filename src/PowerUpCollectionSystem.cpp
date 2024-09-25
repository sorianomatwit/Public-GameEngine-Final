#include "GameSystems.hpp"
#include "GameEngine.hpp"

class GameEngine;

void PowerUpCollectionSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	for (int i = entities.size() - 1;i >= 0;i--)
	{
		auto id = entities[i];
		auto entity = Entity(id);
		auto powerUpData = entity.getComponent<CPowerUp>();
		//Check collision with player and then set powerup vector to active
		if (entity.hasComponent<CCollisionContainer>()) {
			for (auto& c : entity.getComponent<CCollisionContainer>().enterCollisions) {
				if (c.otherEntity.hasComponent<CPlayer>()) {
					auto& playerData = c.otherEntity.getComponent<CPlayer>();
					if (powerUpData.powerUp.type == Shield) {
						//create shield
						playerData.shield
							.addComponent<CRigidBody>(CRigidBody(Vec2(0, 0), Vec2(0, 0), 0, { {48,Vec2(0,0)} }))
							.addComponent<CAnimation>(
								CAnimation(
									Animation(GameEngine::getAssets().getAnimation("Shield"), Vec2(3.0f, 3.0f)),
									false,
									ColorFromNormalized({ 1.0f,1.0f ,1.0f ,0.5f })
								)
							);
						playerData.shield.setActive(true);

					}

					if (powerUpData.powerUp.type == ExtraHealth) {
						auto& healthdata = c.otherEntity.getComponent<CHealth>();
						if (healthdata.health + 1 <= healthdata.maxHealth) {
							healthdata.health ++;
						}
					}
					playerData.powerUps[powerUpData.powerUp.type] = powerUpData.powerUp;
					entity.destroy();
				}

				if (c.otherEntity.hasComponent<CHouse>() || (c.otherEntity.hasComponent<CTile>() && c.otherEntity.getComponent<CTile>().tile.info.type == ROAD_BLOCK)) {
					entity.getComponent<CRigidBody>().position -= (c.direction * c.overlap);
				}
			}
		}
	}
}

void PowerUpMovementSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	for (auto id: entities)
	{
		auto entity = Entity(id);
		auto& rbData = entity.getComponent<CRigidBody>();
		
		rbData.velocity += rbData.acceleration;
		rbData.position += rbData.velocity;
		rbData.acceleration *= 0;

		rbData.velocity *= -0.15;
	}
}

