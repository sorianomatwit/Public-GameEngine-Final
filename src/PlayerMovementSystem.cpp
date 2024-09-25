
#include "GameSystems.hpp"
#include "GameEngine.hpp"
#include "GameScene.hpp"

void PlayerMovementSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	for (auto id : entities) {
		auto entity = Entity(id);
		auto& playerData = entity.getComponent<CPlayer>();
		if (playerData.stunned) continue;

		auto& rb = entity.getComponent<CRigidBody>();
		auto& car = entity.getComponent<CCarController>();

		if (entity.hasComponent<CCollisionContainer>()) {
			for (auto& c : entity.getComponent<CCollisionContainer>().enterCollisions) {
				if (c.otherEntity.hasComponent<CTile>()) {
					auto& cTile = c.otherEntity.getComponent<CTile>();
					GameScene::onCurrentTile = cTile.tile.info;
					auto cannotPass =
						cTile.tile.info.type == ROAD_BLOCK ||
						cTile.tile.info.type == House_0 ||
						cTile.tile.info.type == House_1 ||
						cTile.tile.info.type == DOG_HOUSE;

					if (cannotPass) {
						rb.position -= (c.direction * c.overlap);
						rb.acceleration *= car.impactReduction;
					}
				}
			}
		}

		if (playerData.powerUps[TripleShot].isActive) {
			playerData.canon.getComponent<CAnimation>().animation = (playerData.currentAmmo == MAIL) ?
				GameEngine::getAssets().getAnimation("Triple_M_cannon") : GameEngine::getAssets().getAnimation("Triple_T_cannon");
		}
		else {
			playerData.canon.getComponent<CAnimation>().animation = (playerData.currentAmmo == MAIL) ?
				GameEngine::getAssets().getAnimation("Mail_cannon") : GameEngine::getAssets().getAnimation("Treat_cannon");
		}


		rb.position += rb.velocity;
		rb.velocity += rb.acceleration;
		rb.acceleration *= 0;
		rb.velocity += rb.velocity * -car.dragC;

		for (auto& collider : rb.colliders)
		{
			Circ player{
				rb.position,
				collider.radius
			};
			Vec2 overlap = calculateCircOverlapBoundary(player, 0, GameEngine::worldWidth, 0, GameEngine::worldHeight);
			rb.position -= overlap * -1;
		}
		auto& canonrb = playerData.canon.getComponent<CRigidBody>();
		canonrb.position = rb.position;
		canonrb.angle = rb.angle;

		if (playerData.powerUps[Shield].isActive) {
			playerData.shield.getComponent<CRigidBody>().position = rb.position;
		}
		else if (playerData.shield.isActive()) {
			playerData.shield.destroy();
		}

	}
}
