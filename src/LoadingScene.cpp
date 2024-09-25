#include "LoadingScene.hpp"

void LoadingScene::init() {
	auto pos = GameEngine::mainCamera.target;
	auto campos = Vec2(pos.x, pos.y);
	auto& font = GameEngine::getAssets().getFont("PlayMegaGames");
	Text text = {
		"Loading...",
		font,
		WHITE,
		30.0f,
		campos
	};
	Display ProgressBarBg = {
		Animation(GameEngine::getAssets().getAnimation("Gray_bar"), Vec2(2,2)),
		false,
		WHITE,
		campos,
		true,
		10.0f,
		0
	};
	Display ProgressBar = {
		Animation(GameEngine::getAssets().getAnimation("White_bar"), Vec2(2,2)),
		false,
		WHITE,
		campos,
		true,
		10.0f,
		0
	};
	auto e = EntityManager::createEntity()
		.addComponent<CGui>(CGui(text, ProgressBarBg, ProgressBar))
		.addComponent<TLoadTracker>(TLoadTracker());
	sceneEntities.push_back(e);
	EntityManager::activateAllEntities();
}

void LoadingScene::drawDebug() {

}

