#include "GameSystems.hpp"
#include "GameEngine.hpp"
#include "GameScene.hpp"

void SpawnDogSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {

    Entity playerEntity = Entity(ComponentSet<CPlayer>::entities[0]);
    auto& playerRb = playerEntity.getComponent<CRigidBody>();
    Vec2 playerPosition = playerRb.position;

    for (auto id : entities) {
        auto entity = Entity(id);
        auto& dogHouse = entity.getComponent<CDogHouse>();
        auto& rb = entity.getComponent<CRigidBody>();

        float distanceToPlayer = (rb.position - playerPosition).length();

        if (distanceToPlayer <= dogHouse.detectionRadius) {
            dogHouse.spawnDog = true;
        } else {
            dogHouse.spawnDog = false;
        }

        if (dogHouse.spawnDog) {
            dogHouse.timeSinceLastSpawn += deltaTime;

            if (dogHouse.timeSinceLastSpawn >= dogHouse.spawnInterval) {
                dogHouse.timeSinceLastSpawn = 0.0f;

                DogType dogType = (rand() % 4 == 0) ? BARKER : CHASER;
                GameScene::createDog(rb.position, dogType);
            }
        }
    }
}
