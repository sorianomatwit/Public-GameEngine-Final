#include "GameSystems.hpp"
#include "GameEngine.hpp"
class GameEngine;

void SyncBodiesSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	for (auto id : entities)
	{
		auto entity = Entity(id);
		auto& renderData = entity.getComponent<CAnimation>();
		auto& rbData = entity.getComponent<CRigidBody>();

		renderData.angle = rbData.angle;
		renderData.position = Vector2(rbData.position.x, rbData.position.y);
	}
}
