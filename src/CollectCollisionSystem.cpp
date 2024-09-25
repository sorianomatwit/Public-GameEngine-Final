#include "GameSystems.hpp"
#include "Utils.hpp"
#include "GameEngine.hpp"


void CollectCollisionSystem::Execute(std::vector<uint16_t> entites, float deltatime) {
	Rect camArea = {
			Vec2(GameEngine::mainCamera.target.x - GameEngine::windowWidth * 0.5f, GameEngine::mainCamera.target.y - GameEngine::windowHeight * 0.5f),
			Vec2(GameEngine::windowWidth, GameEngine::windowHeight),
	};
	float threshold = 30;
	collidableEntityCount = 0;
	std::vector<uint16_t> collidableEntities;
	for (auto id : entites)
	{
		auto entity = Entity(id);
		auto& rb = entity.getComponent<CRigidBody>();
		for (auto& collider : rb.colliders) {
			Circ circ = { rb.position + collider.offset, collider.radius };
			collider.show = false;
			if (!RectOverlapsCirc(camArea, circ) && getRectCircOverlap(camArea, circ) <= threshold) continue;
			collidableEntityCount++;
			collider.show = true;
			collidableEntities.push_back(id);
			break;
		}
	}
	for (auto id : collidableEntities)
	{
		auto entity = Entity(id);
		auto& rb = entity.getComponent<CRigidBody>();
		for (auto& collider : rb.colliders) {
			Circ circ = { rb.position + collider.offset, collider.radius };
			for (auto otherId : collidableEntities)
			{
				if (id == otherId) continue;
				auto otherEntity = Entity(otherId);
				auto& otherRb = otherEntity.getComponent<CRigidBody>();
				for (auto& otherCollider : otherRb.colliders) {

					Circ otherCirc = { otherRb.position + otherCollider.offset, otherCollider.radius };

					bool currentlyOverlap = CircleOverlaps(circ, otherCirc);
					bool previouslyOverlap = CircleOverlaps(circ, otherCirc);
					if (currentlyOverlap) {
						if (!entity.hasComponent<CCollisionContainer>()) {
							entity.addComponent<CCollisionContainer>(CCollisionContainer());
						}
						Collision c = { Entity(otherId) , rb.velocity, getDirectionCirc(circ,otherCirc),  getCircOverlap(circ, otherCirc) };
						entity.getComponent<CCollisionContainer>().enterCollisions.push_back(c);
					}
					if (previouslyOverlap && !currentlyOverlap) {
						if (!entity.hasComponent<CCollisionContainer>()) {
							entity.addComponent<CCollisionContainer>(CCollisionContainer());
						}
						Collision c = { Entity(otherId) , rb.velocity , getDirectionCirc(circ,otherCirc),  getCircOverlap(circ, otherCirc) };
						entity.getComponent<CCollisionContainer>().exitCollisions.push_back(c);
					}
				}
			}
		}
	}
}
