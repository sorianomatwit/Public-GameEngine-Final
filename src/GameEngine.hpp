#pragma once
#include <set>
#include "SceneBase.hpp"
#include "Assets.hpp"
#include <future>
#include <chrono>

class SceneBase;

bool animationInView(CAnimation& component);

enum SCENE {
	START = 0,
	OPEN = 1,
	GAME = 2,
	LOADING = 3,
	GAMEOVER = 4
};

struct DebugVars {
	bool showColliders;
};
class GameEngine {
private:
	friend class RenderSystem;
	inline static bool running = true;
	//TODO when finished turn this into an array of the amount of scenes
	inline static SCENE currScene;
	inline static SCENE queuedScene;
	inline static Shader shader;
	inline static Matrix mvp;
	inline static int zIndexLoc;
	inline static std::set<int> keysPressed;
	inline static std::set<int> keysPressedThisFrame;
	inline static std::set<int> keysReleasedThisFrame;
	inline static Assets assets;
	inline static std::future<void> waitingScene;
public:
	inline static std::map<SCENE, std::shared_ptr<SceneBase>> sceneMap;
	inline static int currentFrame = 0;
	inline static int worldWidth;
	inline static int worldHeight;
	inline static int worldScale = 4;
	inline static DebugVars debug;

	inline static int windowHeight;
	inline static int windowWidth;
	inline static float renderDeltaTime = 0;
	inline static float systemsDeltaTime = 0;
	inline static Camera2D mainCamera;

	static std::set<int> getKeysPressedThisFrame();
	static std::set<int> getKeysReleasedThisFrame();

	static Assets& getAssets();

	static void updateKeys();
	static void update();
	static void addAllScenes();
	static void changeScene(SCENE sceneName);
	static void start(std::string config);
	static void run();
	static void quit();
	//For drawing debug views only that will render in the game DO NOT PUT IMGUI stuff in here
	static void debugWindow();

};