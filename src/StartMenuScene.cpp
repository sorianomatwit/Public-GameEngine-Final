#include "StartMenuScene.hpp"

void StartMenuScene::init() {
	backgroundColor = SKYBLUE;
	// Implementation of init
	//EntityManager::createEntity()
	//	.addComponent<CParticleEmitter>(
	//		CParticleEmitter
	//		(
	//			GameEngine::getAssets().getAnimation("Circle").getTexture(),
	//			100,
	//			Vec2(GameEngine::mainCamera.target.x, GameEngine::mainCamera.target.y),
	//			{ 1,5 },
	//			{ 0,360 },
	//			{ .5f,5 }
	//		)
	//	);
	auto& font = GameEngine::getAssets().getFont("PlayMegaGames");
	Vec2 topLeft = Vec2(GameEngine::mainCamera.target.x - GameEngine::windowWidth * 0.5f, GameEngine::mainCamera.target.y - GameEngine::windowHeight * 0.5f);
	Text startText = {
		"START",
		font,
		GRAY,
		30.0f,
		topLeft,
		Vec2(30.0f + startText.getSize().x * 0.5f, GameEngine::windowHeight * 0.25f)
	};
	Text creditsText = {
		"CREDITS",
		font,
		GRAY,
		30.0f,
		topLeft,
		Vec2(30.0f + creditsText.getSize().x * 0.5f, GameEngine::windowHeight * 0.35f)
	};
	Text quitText= {
		"QUIT",
		font,
		GRAY,
		30.0f,
		topLeft,
		Vec2(30.0f + quitText.getSize().x * 0.5f, GameEngine::windowHeight * 0.45f)
	};
	auto s = EntityManager::createEntity()
		.addComponent<CGui>(CGui(startText));
	auto c = EntityManager::createEntity()
		.addComponent<CGui>(CGui(creditsText));
	auto q = EntityManager::createEntity()
		.addComponent<CGui>(CGui(quitText));
	EntityManager::createEntity()
		.addComponent(CMenuOption(s, c, q))
		.addComponent<CInput>(CInput(MOVE_DOWN, MOVE_UP, SELECT));
}

void StartMenuScene::drawDebug() {
	ImGui::Begin("Start Scene", NULL, ImGuiWindowFlags_NoResize);

	ImGui::End();


}

