#include "EngineSystems.hpp"

class GameEngine;

void ParticleSystem::Execute(std::vector<uint16_t> entites, float deltatime) {

	for (auto id : entites)
	{
		auto entity = Entity(id);
		if (!entity.isActive()) continue;

		auto& emitter = entity.getComponent<CParticleEmitter>();
		emitter.isFinished = emitter.particles[0].isDead;
		for (auto& p : emitter.particles) {
			//Update
			emitter.isFinished = emitter.isFinished && p.isDead;
			if (p.isDead) continue;
			p.position += p.velocity;
			p.lifespan -= deltatime;
			p.isDead = p.lifespan <= 0;
			//Draw
			if (!emitter.shape.hasEnded()) {
				emitter.shape.update();
			}
			else if (emitter.loopAnimation) {
				emitter.shape.currentFrame = 0;
			}

			float scaledWidth = emitter.shape.getScaledSize().x;
			float scaledHeight = emitter.shape.getScaledSize().y;

			float x = p.position.x;
			float y = p.position.y;

			Rectangle src = emitter.shape.getFrameRect();
			Rectangle dest = { x, y, scaledWidth, scaledHeight };
			Vector2 origin = {
				 0.5f * emitter.shape.size.x * emitter.shape.scale.x
				, 0.5f * emitter.shape.size.y * emitter.shape.scale.y };


			DrawTexturePro(emitter.shape.getTexture(), src, dest, origin, 0, ColorAlpha(emitter.tint, (p.lifespan / p.initialLifespan) * !p.isDead));

		}
	}
}

void ParticleCleanUpSystem::Execute(std::vector<uint16_t> entites, float deltatime) {

	for (int i = entites.size() - 1; i >= 0; i--)
	{
		auto pEntity = Entity(entites[i]);
		if (pEntity.getComponent<CParticleEmitter>().isFinished) {
			pEntity.removeComponent<CParticleEmitter>();
		}
	}
}