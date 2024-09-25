#pragma once
#include "SceneBase.hpp"

class GameOverScene : public SceneBase {
private:
	friend class GameOverSystem;
	inline static InputAction MOVE_UP = { KEY_W,KEY_UP,GAMEPAD_LEFT_AXIS_NEG_Y };
	inline static InputAction MOVE_DOWN = { KEY_S,KEY_DOWN,GAMEPAD_LEFT_AXIS_POS_Y };
	inline static InputAction SELECT = { KEY_SPACE, GAMEPAD_BUTTON_RIGHT_FACE_DOWN };
public:
	inline static std::string EndingText = "YOU LOSE";
	GameOverScene() : SceneBase
	(
		std::make_shared<GameOverSystem>()

	) {};

	void init() override;
	void drawDebug() override;
};
