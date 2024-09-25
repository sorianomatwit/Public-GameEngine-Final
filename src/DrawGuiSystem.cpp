#include "EngineSystems.hpp"

void DrawThreeSlice(Texture2D texture, Rectangle destRect, int edgeSize, int middleWidth, Vector2 origin, float angle, Color tint) {
	int w = texture.width;
	int h = texture.height;

	// Source rectangles
	Rectangle srcLeft = { 0, 0, (float)edgeSize, (float)h };                    // Left edge
	Rectangle srcMiddle = { (float)edgeSize, 0, (float)(w - 2 * edgeSize), (float)h }; // Middle part
	Rectangle srcRight = { (float)(w - edgeSize), 0, (float)edgeSize, (float)h };     // Right edge

	// Calculate destination middle width
	int newMiddleWidth = middleWidth > 0 ? middleWidth : (int)(destRect.width - 2 * edgeSize);

	// Destination rectangles
	Rectangle destLeft = { destRect.x, destRect.y, (float)edgeSize, destRect.height }; // Left edge
	Rectangle destMiddle = { destRect.x + edgeSize, destRect.y, (float)newMiddleWidth, destRect.height }; // Middle part
	Rectangle destRight = { destRect.x + edgeSize + newMiddleWidth, destRect.y, (float)edgeSize, destRect.height }; // Right edge

	// Draw the three parts
	DrawTexturePro(texture, srcLeft, destLeft, origin, angle, tint);
	DrawTexturePro(texture, srcMiddle, destMiddle, origin, angle, tint);
	DrawTexturePro(texture, srcRight, destRight, origin, angle, tint);
}

void DrawGuiSystem::Execute(std::vector<uint16_t> entites, float deltatime) {
	for (auto id : entites)
	{
		auto entity = Entity(id);
		if (!entity.isActive()) continue;
		auto& guiData = entity.getComponent<CGui>();

		for (auto& gui : guiData.guiItems)
		{
			if (Text* textGui = std::get_if<Text>(&gui)) {
				auto& textData = *textGui;
				auto text = textData.text.c_str();

				Vector2 textSize = MeasureTextEx(textData.font, text, textData.fontSize, 1);
				float xOffset = textSize.x * -0.5f;
				float yOffset = textSize.y * -0.5f;
				Vector2 pos = Vector2(textData.origin.x + textData.offset.x + xOffset, textData.origin.y + textData.offset.y + yOffset);

				DrawTextEx(textData.font, text, pos, textData.fontSize, 1, textData.color);
			}
			else if (Display* displayGui = std::get_if<Display>(&gui)) {
				// is Display
				auto& displayData = *displayGui;
				const Texture2D& tex = displayData.animation.getTexture();
				
				if (displayData.isThreeSlice) {
					float scaledWidth = displayData.animation.getScaledSize().x;
					float scaledHeight = displayData.animation.getScaledSize().y;

					float x = displayData.origin.x + displayData.offset.x;
					float y = displayData.origin.y + displayData.offset.y;
					Rectangle dest = { x, y, displayData.middleWidth + displayData.edgeSize*2, scaledHeight };

					Vector2 origin = {
						 0.5f * displayData.animation.size.x * displayData.animation.scale.x
						, 0.5f * displayData.animation.size.y * displayData.animation.scale.y };
					DrawThreeSlice(tex, dest, displayData.edgeSize, displayData.middleWidth, origin, displayData.angle, displayData.tint);
				}
				else {
					if (!displayData.animation.hasEnded()) {
						displayData.animation.update();
					}
					else if (displayData.repeat) {
						displayData.animation.currentFrame = 0;
					}

					float scaledWidth = displayData.animation.getScaledSize().x;
					float scaledHeight = displayData.animation.getScaledSize().y;

					float x = displayData.origin.x + displayData.offset.x;
					float y = displayData.origin.y + displayData.offset.y;

					Rectangle src = displayData.animation.getFrameRect();
					Rectangle dest = { x, y, scaledWidth, scaledHeight };
					Vector2 origin = {
						 0.5f * displayData.animation.size.x * displayData.animation.scale.x
						, 0.5f * displayData.animation.size.y * displayData.animation.scale.y };


					DrawTexturePro(tex, src, dest, origin, displayData.angle, displayData.tint);
				}
			}
		}
	}
}

