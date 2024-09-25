
#include "EngineSystems.hpp"
#include "GameEngine.hpp"
#include <algorithm>
class GameEngine;

bool CompareByZIndex(Entity& a, Entity& b) {
	return a.getComponent<CAnimation>().zIndex < b.getComponent<CAnimation>().zIndex;
}


void RenderSystem::Execute(std::vector<uint16_t> entites, float deltatime) {
	int renderCount = 0;
	std::vector<Entity> renderList;
	for (int i = 0; i < entites.size(); i++) {
		auto entity = Entity(entites[i]);

		if (!entity.isActive()) continue;
		auto& renderData = entity.getComponent<CAnimation>();

		if (!animationInView(renderData)) continue;
		renderList.push_back(entity);

	}
	std::sort(renderList.begin(), renderList.end(), CompareByZIndex);
	for (auto entity : renderList)
	{
		auto& renderData = entity.getComponent<CAnimation>();
		const Texture2D& tex = renderData.animation.getTexture();

		if (!renderData.animation.hasEnded()) {
			renderData.animation.update();
		}
		else if (renderData.repeat) {
			renderData.animation.currentFrame = 0;
		}

		float scaledWidth = renderData.animation.getScaledSize().x;
		float scaledHeight = renderData.animation.getScaledSize().y;

		float x = renderData.position.x;
		float y = renderData.position.y;

		Rectangle src = renderData.animation.getFrameRect();
		Rectangle dest = { x,y,scaledWidth,scaledHeight };
		Vector2 origin = { scaledWidth / 2,scaledHeight / 2 };

		DrawTexturePro(tex, src, dest, origin, renderData.angle, renderData.tint);
	}
}