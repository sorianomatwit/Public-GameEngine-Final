#include "GameSystems.hpp"

void CleanCollisionSystem::Execute(std::vector<uint16_t> entites, float deltatime) {
	for (int i = entites.size() - 1; i >= 0; i--)
	{
		Entity(entites[i]).removeComponent<CCollisionContainer>();
	}
}