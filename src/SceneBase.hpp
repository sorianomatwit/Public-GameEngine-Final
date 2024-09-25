#pragma once
#include "EngineSystems.hpp"
#include "GameSystems.hpp"
#include "GameEngine.hpp"
#include <raylib.h>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <imgui/imgui_stdlib.h>

class GameEngine;

template<typename T, typename U>
concept isSharedPtrodBase = std::is_base_of_v<U, typename T::element_type>&& std::is_same_v<T, std::shared_ptr<typename T::element_type>>;

class SceneBase {
protected:
    std::vector<std::shared_ptr<SystemBase>> Features;
    /* System and executed in the order of added to the scene */
    template <typename ...Args> requires (isSharedPtrodBase<Args, SystemBase> && ...)
        SceneBase(Args ...systems) : Features{ systems... } {};

public:
	Color backgroundColor = BLACK;
	InputSystem inputSystem;
	RenderSystem renderSystem;
	DrawGuiSystem guiSystem;

	ParticleSystem particleSystem;
	ParticleCleanUpSystem particlecCleanUpSystem;

	AudioPlaySystem audioSystem;
	AudioCleanUpSystem audioCleanUpSystem;
	/*Register Input keys and create entites */
	virtual void init() = 0;
	void update(float deltatime) {
		for (int i = 0; i < Features.size();i++)
		{
			Features[i]->Run(deltatime);
		}
	}
	virtual void drawDebug() = 0;
	void restartScene() {
		EntityManager::destroyAll();
		init();
	}

};
