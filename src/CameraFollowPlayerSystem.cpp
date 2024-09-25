#pragma once
#include "GameSystems.hpp"
#include "GameEngine.hpp"
class GameEngine;

void CameraFollowPlayerSystem::Execute(std::vector<uint16_t> entites, float deltatime) {
	for (auto id : entites)
	{
		auto entity = Entity(id);
		auto& rb = entity.getComponent<CRigidBody>();
		bool isWithinLeftBoundary = rb.position.x >= GameEngine::windowWidth * 0.5f;
		bool isWithinRightBoundary = rb.position.x <= GameEngine::worldWidth - GameEngine::windowWidth * 0.5f;
		bool isWithinTopBoundary = rb.position.y >= GameEngine::windowHeight * 0.5f;
		bool isWithinBottomBoundary = rb.position.y <= GameEngine::worldHeight - GameEngine::windowHeight * 0.5f;

		if ((isWithinLeftBoundary && isWithinRightBoundary) || goPassWorldBoundary)
			GameEngine::mainCamera.target.x = rb.position.x;
		if ((isWithinTopBoundary && isWithinBottomBoundary) || goPassWorldBoundary)
			GameEngine::mainCamera.target.y = rb.position.y;
	}
}