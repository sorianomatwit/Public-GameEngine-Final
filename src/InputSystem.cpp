#pragma once
#include "EngineSystems.hpp"
#include "GameEngine.hpp"

class GameEngine;

bool setAxis(int key, int GAMEPAD, float rightX, float rightY, float leftX, float leftY) {


	switch (key)
	{
	case GAMEPAD_LEFT_AXIS_POS_X: return leftX > 0;
	case GAMEPAD_LEFT_AXIS_NEG_X: return leftX < 0;
	case GAMEPAD_LEFT_AXIS_POS_Y: return leftY > 0;
	case GAMEPAD_LEFT_AXIS_NEG_Y: return leftY < 0;
	case GAMEPAD_RIGHT_AXIS_POS_X: return rightX > 0;
	case GAMEPAD_RIGHT_AXIS_NEG_X: return rightX < 0;
	case GAMEPAD_RIGHT_AXIS_POS_Y: return rightY > 0;
	case GAMEPAD_RIGHT_AXIS_NEG_Y: return rightY < 0;
	default:
		break;
	}
}


void InputSystem::Execute(std::vector<uint16_t> entites, float deltatime) {

	for (auto id : entites)
	{
		auto entity = Entity(id);
		if (!entity.isActive()) continue;

		auto& inputData = entity.getComponent<CInput>();
		if (!inputData.isActive) continue;
		auto& keysPress = inputData.keyboardInputs[PRESS];
		auto& keysPressed = inputData.keyboardInputs[PRESSED];
		auto& keysReleased = inputData.keyboardInputs[RELEASED];

		//get Mouse inputs
		Vec2 camPos(GameEngine::mainCamera.target.x, GameEngine::mainCamera.target.y);
		auto mouseVector2 = GetMousePosition();
		inputData.mouse.position = (camPos - Vec2(GameEngine::windowWidth, GameEngine::windowHeight) * 0.5f) + Vec2(mouseVector2.x, mouseVector2.y);
		mouseVector2 = GetMouseDelta();
		inputData.mouse.delta = Vec2(mouseVector2.x, mouseVector2.y);

		for (auto& [key, value] : inputData.mouseInputs[PRESSED]) value = IsMouseButtonPressed(key);
		for (auto& [key, value] : inputData.mouseInputs[PRESS]) value = IsMouseButtonDown(key);
		for (auto& [key, value] : inputData.mouseInputs[RELEASED]) value = IsMouseButtonReleased(key);

		//get GamePad input and axis
		if (IsGamepadAvailable(0)) {
			int GAMEPAD = 0;
			for (auto& [key, value] : inputData.gamepadInputs[PRESSED])	value = IsGamepadButtonPressed(GAMEPAD, key);
			for (auto& [key, value] : inputData.gamepadInputs[PRESS]) value = IsGamepadButtonDown(GAMEPAD, key);
			for (auto& [key, value] : inputData.gamepadInputs[RELEASED]) value = IsGamepadButtonReleased(GAMEPAD, key);

			//get axis

			float rightX = GetGamepadAxisMovement(GAMEPAD, GAMEPAD_AXIS_RIGHT_X);
			float rightY = GetGamepadAxisMovement(GAMEPAD, GAMEPAD_AXIS_RIGHT_Y);
			float leftX = GetGamepadAxisMovement(GAMEPAD, GAMEPAD_AXIS_LEFT_X);
			float leftY = GetGamepadAxisMovement(GAMEPAD, GAMEPAD_AXIS_LEFT_Y);
			Vec2 lefta = Vec2(leftX, leftY);
			Vec2 righta = Vec2(rightX, rightY);
			inputData.joystick.leftAxisDelta = lefta - inputData.joystick.leftAxis;
			inputData.joystick.rightAxisDelta = righta - inputData.joystick.rightAxis;
			inputData.joystick.leftAxis = lefta;
			inputData.joystick.rightAxis = righta;
			for (auto& [key, value] : inputData.joyStickInputs[PRESS]) {
				
				inputData.joyStickInputs[PRESSED][key] = !value;
				value = setAxis(key, GAMEPAD, rightX, rightY, leftX, leftY);
				inputData.joyStickInputs[PRESSED][key] = inputData.joyStickInputs[PRESSED][key] && value;
			}
			for (auto& [key, value] : inputData.joyStickInputs[RELEASED]) {
				value = !setAxis(key, GAMEPAD, rightX, rightY, leftX, leftY);
			}

		}

		for (auto& [registeredKey, value] : keysPress)
		{
			keysPressed[registeredKey] = false;
			for (int key : GameEngine::getKeysPressedThisFrame()) {
				//CONTAINS ALL KEYS PRESSED IN THIS FRAME
				if (!inputData.isKeyRegistered(key) || registeredKey != key) continue;
				//registered key was pressed
				keysPressed[registeredKey] = !value && !keysPressed[registeredKey];
				value = true;
			}
		}
		for (auto& [registeredKey, value] : keysReleased) {
			value = false;
			for (int key : GameEngine::getKeysReleasedThisFrame()) {
				//CONTAINS ALL KEYS RELEASED IN THIS FRAME
				if (!inputData.isKeyRegistered(key) || registeredKey != key) continue;
				//registered key was released
				value = true;
				keysPress[registeredKey] = false;
			}
		}

		////DEBUG INPUTS
		//for (auto [key, value] : keysPressed) {
		//	if (value) std::cout << "I WAS PRESSED\n";
		//	if (keysPress[key]) std::cout << "IM BEING PRESS\n";
		//	if (keysReleased[key]) std::cout << "IM BEING RELEASED\n";
		//}


	}
}