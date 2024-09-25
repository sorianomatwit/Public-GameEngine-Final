#include "OpeningScene.hpp"


void OpeningScene::init() {
	backgroundColor = BLACK;
	auto position = GameEngine::mainCamera.target;
	EntityManager::createEntity()
		.addComponent<CAnimation>(CAnimation(GameEngine::getAssets().getAnimation("Intro_logo"), false, position, 0))
		.setActive(true);
}

void OpeningScene::drawDebug() {
	if (ImGui::BeginTabBar("scene##left_tabs_bar")) {
		if (ImGui::BeginTabItem("Game Engine")) {
			auto text = std::format("FPS {}", GetFPS());
			ImGui::Text(text.c_str());
			ImGui::SameLine();
			text = std::format("Render delta {}", GameEngine::renderDeltaTime);
			ImGui::Text(text.c_str());

			ImGui::SameLine();
			text = std::format("System delta {}", GameEngine::systemsDeltaTime);
			ImGui::Text(text.c_str());


			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

