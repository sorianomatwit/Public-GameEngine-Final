#include "OpeningSceneSystem.hpp"
#include "GameEngine.hpp"

class GameEngine;

void OpeningSceneSystem::Execute(std::vector<uint16_t> entites, float deltatime) {
	for (auto id : entites)
	{
		auto renderData = Entity(id).getComponent<CAnimation>();

		GameEngine::sceneMap[OPEN]->backgroundColor = (renderData.animation.currentFrame == 5) ? ColorFromNormalized(
			{
				59.0f / 255.0f,
				250.0f / 255.0f,
				134.0f / 255.0f,
				1.0f,
			}
		) : BLACK;

		if (renderData.animation.hasEnded()) {
			wait10Sec--;
		}

		if (wait10Sec <= 0) {
			GameEngine::changeScene(GAME);
		}
	}
}
