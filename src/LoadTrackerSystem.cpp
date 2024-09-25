#include "LoadTrackerSystem.hpp"
#include "LoadingScene.hpp"

void LoadTrackerSystem::Execute(std::vector<uint16_t> entites, float deltatime) {

	for (auto id : entites)
	{
		if (!Entity(id).hasComponent<CGui>()) continue;
		auto& guiData = Entity(id).getComponent<CGui>();
		for (auto& item : guiData.guiItems) {
			if (auto* textGui = std::get_if<Text>(&item)) {

				textGui->text = std::format("Loading {}/{}", LoadingScene::loadingCount, LoadingScene::loadingTotal);
				textGui->offset.y = -textGui->getSize().y*2;
			}
		}
		if (guiData.guiItems.size() > 0) {
			auto& displayDataBg = std::get<Display>(guiData.guiItems[1]);
			auto& displayData = std::get<Display>(guiData.guiItems[2]);
			displayDataBg.middleWidth = GameEngine::windowWidth * 0.5f - displayDataBg.edgeSize * 2;
			displayDataBg.offset.x = -0.25f *(displayDataBg.middleWidth + displayDataBg.edgeSize * 2);
			displayData.offset = displayDataBg.offset;
			displayData.middleWidth = displayDataBg.middleWidth * (float)LoadingScene::loadingCount / (float)LoadingScene::loadingTotal;
		}
	}
}
