#pragma once

#include "SystemBase.hpp"
#include "Animation.hpp"
#include <raylib.h>
#include <type_traits>
#include <variant>
#include <random>


//DEFINE INPUT COMPONENT HELPER
#pragma region INPUT
enum ActionType {
	PRESS = 0,
	PRESSED = 1,
	RELEASED = 2
};

enum JoystickButton {
	GAMEPAD_LEFT_AXIS_POS_X = 0,
	GAMEPAD_LEFT_AXIS_NEG_X = 1,
	GAMEPAD_LEFT_AXIS_POS_Y = 2,
	GAMEPAD_LEFT_AXIS_NEG_Y = 3,
	GAMEPAD_RIGHT_AXIS_POS_X = 4,
	GAMEPAD_RIGHT_AXIS_NEG_X = 5,
	GAMEPAD_RIGHT_AXIS_POS_Y = 6,
	GAMEPAD_RIGHT_AXIS_NEG_Y = 7
};

using InputVariant = std::variant<KeyboardKey, MouseButton, GamepadButton, JoystickButton>;
typedef std::vector<InputVariant> InputAction;

struct MouseData {
	Vec2 position;
	Vec2 delta;
};

struct JoyStickData {
	Vec2 rightAxis;
	Vec2 rightAxisDelta;
	Vec2 leftAxis;
	Vec2 leftAxisDelta;
};

template<typename T>
concept isInput = std::is_same_v<T, InputAction>;

/**
 * Input Component
 *
 * Can store bool for any input
 */
class CInput : public Component<CInput> {
private:
	friend class InputSystem;
	std::map<int, bool> keyboardInputs[3];
	std::map<int, bool> mouseInputs[3];
	std::map<int, bool> gamepadInputs[3];
	std::map<int, bool> joyStickInputs[3];
	bool isKeyRegistered(int key);
public:
	MouseData mouse;
	JoyStickData joystick;
	InputVariant lastInputType;
	int keyCount = 0;
	CInput();
	template <typename ...Args> requires (isInput<Args> && ...)
		CInput(Args ...keys) {
		keyboardInputs[PRESSED] = {};
		keyboardInputs[PRESS] = {};
		keyboardInputs[RELEASED] = {};

		mouseInputs[PRESSED] = {};
		mouseInputs[PRESS] = {};
		mouseInputs[RELEASED] = {};

		gamepadInputs[PRESSED] = {};
		gamepadInputs[PRESS] = {};
		gamepadInputs[RELEASED] = {};

		(registerKeyInput(keys), ...);
	}

	bool isActive = true;


	void registerKeyInput(InputAction action);
	bool isInputPressed(InputAction action);
	bool isInputPress(InputAction action);
	bool isInputReleased(InputAction action);

	//Gamepad input
};

#pragma endregion

struct Collider {
	float radius;
	Vec2 offset = Vec2(0, 0);
	bool show = false;
};

//enum BodyType {
//	STATIC,
//	DYNAMIC
//};
/**
 * Rigidbody Component
 *
 * Defines the position, velocty, scale, and angle of an entity
 *
 * All entities should have a transform
 */
class CRigidBody : public Component<CRigidBody> {
public:
	Vec2 position;
	Vec2 prevPosition;
	Vec2 velocity;
	Vec2 acceleration;
	Vec2 scale;
	std::vector<Collider> colliders;
	float angle;

	CRigidBody();
	CRigidBody(const Vec2& position, const Vec2& velocity, float angle, std::vector<Collider> col);
	std::string stringData();

	void applyForce(Vec2 force);
};


/**
 * Animation Component
 *
 * Defines the animation properties
 */
class CAnimation : public Component<CAnimation> {
public:
	int zIndex = 0;
	Animation animation; /*Actual animation object for this component */
	bool repeat = false;         /*true/false if this animation repeats after the last animation frame */
	Vector2 position;
	Color tint = WHITE;
	float angle;
	CAnimation();
	CAnimation(const Animation& animation, bool r);
	CAnimation(const Animation& animation, bool r, int zIndex);
	CAnimation(const Animation& animation, bool r, Color c);
	CAnimation(const Animation& animation, bool r, Vector2 pos, float a);
	CAnimation(const Animation& animation, bool r, Vector2 pos, float a, Color c);
	CAnimation(const Animation& animation, bool r, Vector2 pos, float a, int zIndex);
};



class TLoadTracker : public Component<TLoadTracker> {};

struct Text {
	std::string text;
	Font font;
	Color color;
	float fontSize;
	Vec2 origin = Vec2(0, 0);
	Vec2 offset = Vec2(0, 0);

	Vec2 getSize() {
		Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1);
		return Vec2(textSize.x, textSize.y);
	}
};
struct Display {
	Animation animation;
	bool repeat;
	Color tint;
	Vec2 origin = Vec2(0, 0);
	bool isThreeSlice = false;
	float edgeSize = 0;
	float middleWidth = 0;
	Vec2 offset = Vec2(0, 0);
	float angle = 0;
};


using GuiElement = std::variant<Text, Display>;

template<typename T>
concept isGui = std::is_same_v<T, Text> || std::is_same_v<T, Display>;

class CGui : public Component<CGui> {
public:
	std::vector<GuiElement> guiItems;
	CGui() = default;

	template <typename ...Args> requires (isGui<Args> && ...)
		CGui(Args ...items) : guiItems{ items... } {}
};

struct Range {
	float min;
	float max;
};
struct Particle {
	bool isDead = false;
	float lifespan;
	float initialLifespan;
	Vec2 position;
	Vec2 velocity;
};


class CParticleEmitter : public Component<CParticleEmitter> {

public:
	std::mt19937 gen;

	uint16_t amount = 0;
	std::vector<Particle> particles;
	Vec2 origin;
	std::uniform_real_distribution<float> lifespanDis;
	std::uniform_real_distribution<float> angleDis;
	std::uniform_real_distribution<float> speedDis;
	Animation shape;
	bool loopAnimation = true;
	bool isFinished = false;
	Color tint;
	CParticleEmitter() = default;
	CParticleEmitter(Animation shape, uint16_t amount, Vec2 origin, Range velRange, Range anglerange, Range lifeRange, Color c);
	Particle makeParticle();
	void generateAllParticle(Range velocityRange, Range angleRange, Range lifeRange);
};



enum AudioType {
	SOUND,
	MUSIC
};
class CAudio : public Component<CAudio> {
public:
	AudioType type;
	bool isLooping;
	Sound sound; 
	Music music;
	bool isPlaying = false;
	bool hasStarted = false;


	CAudio() = default;
	CAudio(const Sound& sound, bool isLooping) :  sound(sound) , isLooping(isLooping) { 
		type = SOUND;

	
	};

	CAudio(const Music& music, bool isLooping) : music(music), isLooping(isLooping) {
		type = MUSIC;


	};
};