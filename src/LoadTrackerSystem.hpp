#pragma once
#include "SystemBase.hpp"
#include "EngineComponents.hpp"

class LoadTrackerSystem : public System<TLoadTracker> {
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

