#include "GameEngine.hpp"
#include <include/raylib.h>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <imgui/imgui_stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <memory>
#include <cstdio>

//SCENES
#include "StartMenuScene.hpp"
#include "GameScene.hpp"
#include "LoadingScene.hpp"
#include "OpeningScene.hpp"
#include "GameOverScene.hpp"

void GameEngine::addAllScenes() {
	sceneMap[START] = std::make_shared<StartMenuScene>();
	sceneMap[OPEN] = std::make_shared<OpeningScene>();
	sceneMap[GAME] = std::make_shared<GameScene>();
	sceneMap[LOADING] = std::make_shared<LoadingScene>();
	sceneMap[GAMEOVER] = std::make_shared<GameOverScene>();
}

Assets& GameEngine::getAssets() {
	return assets;
}

void GameEngine::updateKeys() {
	std::vector<int> keysThisFrame;
	int key = 0;

	GameEngine::keysPressedThisFrame.clear();
	GameEngine::keysReleasedThisFrame.clear();
	do {
		key = GetKeyPressed();
		if (key != 0) keysThisFrame.push_back(key);
	} while (key != 0);

	for (int theKey : keysThisFrame) {
		GameEngine::keysPressed.insert(theKey);
		GameEngine::keysPressedThisFrame.insert(theKey);
	}

	std::vector<int> toErase;
	for (int theKey : GameEngine::keysPressed) {
		if (IsKeyReleased(theKey)) {
			toErase.push_back(theKey);
			GameEngine::keysReleasedThisFrame.insert(theKey);
		}
	}
	for (int erase : toErase) {
		GameEngine::keysPressed.erase(erase);
	}

}
static std::mutex sceneMutex;

void GameEngine::update() {

	float deltatime = GetFrameTime();
	
	GameEngine::updateKeys();
	auto currentScene = sceneMap[currScene];
	auto startSys = std::chrono::system_clock::now();
	currentScene->inputSystem.Run(deltatime);
	currentScene->particlecCleanUpSystem.Run(deltatime);
	currentScene->audioCleanUpSystem.Run(deltatime);
	currentScene->update(deltatime);
	currentScene->audioSystem.Run(deltatime);
	auto endSys = std::chrono::system_clock::now();
	auto startRend = std::chrono::system_clock::now();

	BeginDrawing();
	BeginMode2D(GameEngine::mainCamera);
	ClearBackground(sceneMap[currScene]->backgroundColor);
	currentScene->renderSystem.Run(deltatime);
	currentScene->particleSystem.Run(deltatime);
	currentScene->guiSystem.Run(deltatime);
	GameEngine::debugWindow();
	EndMode2D();
	auto endRend = std::chrono::system_clock::now();
	rlImGuiBegin();
	ImGui::SetNextWindowSize(ImVec2(400, 400));
	currentScene->drawDebug();
	rlImGuiEnd();
	EndDrawing();
	renderDeltaTime = std::chrono::duration<float, std::milli>(endRend - startRend).count();
	systemsDeltaTime = std::chrono::duration<float, std::milli>(endSys - startSys).count();
	if (currScene == LOADING && waitingScene.wait_for(std::chrono::duration<int, std::milli>(0)) == std::future_status::ready) {
		std::static_pointer_cast<LoadingScene>(sceneMap[LOADING])->clear();
		waitingScene.get();
		EntityManager::activateAllEntities();
		currScene = queuedScene;
	}
	//std::lock_guard<std::mutex> lock(sceneMutex);
	currentFrame++;


}

void GameEngine::changeScene(SCENE sceneName) {
	EntityManager::destroyAll();
	queuedScene = sceneName;
	currScene = LOADING;
	sceneMap[LOADING]->init();
	GameEngine::waitingScene = std::async(std::launch::async, &SceneBase::init, sceneMap[sceneName]);

}
void GameEngine::start(std::string config) {
	//read config

	std::ifstream file(config);
	if (!file.is_open()) {
		std::cout << "ERROR: config file not found: " + config << "\n";
		return;
	}
	std::string str;
	std::string assetPath;
	std::string windowName;
	while (file.good()) { //use system to do this instead
		file >> str;
		if (str == "WINDOW") {
			file >> windowName >> windowWidth >> windowHeight;
		}
		if (str == "ASSETS") {
			file >> assetPath;
		}
	}
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
	InitWindow(windowWidth, windowHeight, windowName.c_str());
	InitAudioDevice();
	worldWidth = windowWidth * worldScale;
	worldHeight = windowHeight * worldScale;

	mainCamera = Camera2D({ windowWidth * 0.5f,windowHeight * 0.5f }, { windowWidth * 0.5f,windowHeight * 0.5f }, 0, 1);
	rlImGuiSetup(true);
	ImGui::GetStyle().ScaleAllSizes(2);
	SetTargetFPS(60);


	assets.load(assetPath);
	addAllScenes();
	changeScene(GAME);
}


void GameEngine::run() {
	while (running && !WindowShouldClose()) {
		update();
	}
	quit();
}

void GameEngine::quit() {
	CloseAudioDevice();
	UnloadShader(GameEngine::shader);
	rlImGuiShutdown();
	CloseWindow();
	running = false;
}

std::set<int> GameEngine::getKeysPressedThisFrame() {
	return GameEngine::keysPressedThisFrame;
}
std::set<int> GameEngine::getKeysReleasedThisFrame() {
	return GameEngine::keysReleasedThisFrame;
}

void GameEngine::debugWindow() {
	if (GameEngine::debug.showColliders && ComponentSet<CRigidBody>::count > 0) {
		for (auto id : ComponentSet<CRigidBody>::entities)
		{
			auto rbData = Entity(id).getComponent<CRigidBody>();
			for (auto& collider : rbData.colliders) {
				if (collider.show) {
					auto pos = rbData.position + collider.offset;
					DrawCircle(pos.x, pos.y, collider.radius, ColorFromNormalized({ 0.0f,1.0f, 0.0f, 0.5f }));
				}
			}
		}
	}
}

bool animationInView(CAnimation& component) {
	auto& camera = GameEngine::mainCamera;

	auto halfScreenWidth = GetScreenWidth() * 0.5f;
	auto halfScreenHeight = GetScreenHeight() * 0.5f;

	float left = camera.target.x - (halfScreenWidth);
	float right = camera.target.x + (halfScreenWidth);
	float top = camera.target.y - (halfScreenHeight);
	float bottom = camera.target.y + (halfScreenHeight);
	float halfwidth = component.animation.getTexture().width * 0.5f;
	float halfheight = component.animation.getTexture().height * 0.5f;

	return (component.position.x + halfwidth > left &&
		component.position.x - halfwidth < right &&
		component.position.y + halfheight > top &&
		component.position.y - halfheight < bottom);
}