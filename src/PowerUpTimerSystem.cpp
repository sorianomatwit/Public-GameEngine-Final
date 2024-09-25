#include "GameSystems.hpp"

void PowerUpTimerSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	for (auto id : entities)
	{
		auto entity = Entity(id);
		//Check collision with player and then set powerup vector to active
		auto& playerData = entity.getComponent<CPlayer>();

		for (auto& powerUp : playerData.powerUps)
		{
			if (powerUp.isActive) {
				powerUp.duration -= deltaTime;
			}
			powerUp.isActive = powerUp.duration > 0;

		}
	}
}