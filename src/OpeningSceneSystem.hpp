#pragma once
#include "SystemBase.hpp"
#include "EngineComponents.hpp"

class OpeningSceneSystem : public System<CAnimation> {
protected:
	float wait10Sec = 10.0f;
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

