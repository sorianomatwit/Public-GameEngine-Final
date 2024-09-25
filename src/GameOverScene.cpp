#include "GameOverScene.hpp"

void GameOverScene::init() {
	backgroundColor = SKYBLUE;
	// Implementation of init

	auto& font = GameEngine::getAssets().getFont("PlayMegaGames");
	Vec2 center = Vec2(GameEngine::mainCamera.target.x, GameEngine::mainCamera.target.y );
	Text titleText = {
		EndingText,
		font,
		GRAY,
		30.0f,
		center,
		Vec2(0, GameEngine::windowHeight * -0.35f)
	};
	Text startText = {
		"Play Again?",
		font,
		GRAY,
		30.0f,
		center,
		Vec2( 0, 0)
	};
	Text quitText = {
		"QUIT",
		font,
		GRAY,
		30.0f,
		center,
		Vec2(0, GameEngine::windowHeight * 0.25f)
	};
	auto t = EntityManager::createEntity()
		.addComponent<CGui>(CGui(titleText));

	auto s = EntityManager::createEntity()
		.addComponent<CGui>(CGui(startText));
	auto q = EntityManager::createEntity()
		.addComponent<CGui>(CGui(quitText));
	EntityManager::createEntity()
		.addComponent(CMenuOption(s, q))
		.addComponent<CInput>(CInput(MOVE_DOWN, MOVE_UP, SELECT));
}

void GameOverScene::drawDebug() {


}