#include "GameSystems.hpp"
#include "GameEngine.hpp"

void StunSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
    for (auto id : entities) {
        auto entity = Entity(id);
        auto& barker = entity.getComponent<CBarker>();
        auto& rb = entity.getComponent<CRigidBody>();

        barker.barkTimer += deltaTime;

        if (barker.barkTimer >= barker.barkInterval) {
            barker.barkTimer = 0.0f;

           
            auto player = Entity(ComponentSet<CPlayer>::entities[0]);
            auto& playerRb = player.getComponent<CRigidBody>();

            float distance = rb.position.dist(playerRb.position);

            
            if (distance <= 200.0f) {
                auto& playerData = player.getComponent<CPlayer>();
                playerData.stunned = true;
                playerData.stunTimer = 2.0f;

                Vec2 runAwayDirection = (rb.position - playerRb.position).normalize();
                rb.velocity = runAwayDirection * (barker.speed * 5) * deltaTime;
            }
        }

        //Check if barker has moved 200 units away from the player to despawn
        if (rb.velocity.length() > 0) {
            float runDistance = rb.position.dist(Entity(ComponentSet<CPlayer>::entities[0]).getComponent<CRigidBody>().position);
            if (runDistance >= 400.0f) {
                entity.destroy();
            }
            else {
                rb.position += rb.velocity;
            }
        }
    }
    auto player = Entity(ComponentSet<CPlayer>::entities[0]);
    auto& playerData = player.getComponent<CPlayer>();
    if (playerData.stunned) {
        playerData.stunTimer -= deltaTime;
        if (playerData.stunTimer <= 0) {
            playerData.stunned = false;
        }
    }
}
