#include "EngineComponents.hpp";
#include "Utils.hpp"

#pragma region Input
CInput::CInput() {
	keyboardInputs[PRESSED] = {};
	keyboardInputs[PRESS] = {};
	keyboardInputs[RELEASED] = {};
};

void CInput::registerKeyInput(InputAction action) {
	for (const InputVariant& input : action) {
		std::visit([this](auto&& key) {
			using T = std::decay_t<decltype(key)>;
			if constexpr (std::is_same_v<T, KeyboardKey>) {
				keyboardInputs[PRESS][key] = false;
				keyboardInputs[PRESSED][key] = false;
				keyboardInputs[RELEASED][key] = false;
			}
			else if constexpr (std::is_same_v<T, MouseButton>) {
				mouseInputs[PRESS][key] = false;
				mouseInputs[PRESSED][key] = false;
				mouseInputs[RELEASED][key] = false;

			}
			else if constexpr (std::is_same_v<T, GamepadButton>) {
				gamepadInputs[PRESS][key] = false;
				gamepadInputs[PRESSED][key] = false;
				gamepadInputs[RELEASED][key] = false;
			}
			else if constexpr (std::is_same_v<T, JoystickButton>) {
				joyStickInputs[PRESS][key] = false;
				joyStickInputs[PRESSED][key] = false;
				joyStickInputs[RELEASED][key] = false;
			}
			}, input);
	}
	keyCount++;
}
bool CInput::isKeyRegistered(int key) {
	return keyboardInputs[PRESS].find(key) != keyboardInputs[PRESS].end();
}
bool CInput::isInputPressed(InputAction action) {
	bool isPressed = false;
	for (const InputVariant& input : action) {
		std::visit([this, &isPressed](auto&& key) {
			using T = std::decay_t<decltype(key)>;
			if constexpr (std::is_same_v<T, KeyboardKey>) {
				isPressed = isPressed || keyboardInputs[PRESSED][key];
			}
			else if constexpr (std::is_same_v<T, MouseButton>) {
				isPressed = isPressed || mouseInputs[PRESSED][key];

			}
			else if constexpr (std::is_same_v<T, GamepadButton>) {
				isPressed = isPressed || gamepadInputs[PRESSED][key];
			}
			else if constexpr (std::is_same_v<T, JoystickButton>) {
				isPressed = isPressed || joyStickInputs[PRESSED][key];
			}
			}, input);
	}

	return isPressed;
}
bool CInput::isInputPress(InputAction action) {
	bool isPressed = false;
	for (const InputVariant& input : action) {
		std::visit([this, &isPressed](auto&& key) {
			using T = std::decay_t<decltype(key)>;
			if constexpr (std::is_same_v<T, KeyboardKey>) {
				isPressed = isPressed || keyboardInputs[PRESS][key];
			}
			else if constexpr (std::is_same_v<T, MouseButton>) {
				isPressed = isPressed || mouseInputs[PRESS][key];
			}
			else if constexpr (std::is_same_v<T, GamepadButton>) {
				isPressed = isPressed || gamepadInputs[PRESS][key];
			}
			else if constexpr (std::is_same_v<T, JoystickButton>) {
				isPressed = isPressed || joyStickInputs[PRESS][key];
			}
			if (isPressed) {
				lastInputType = key;
			}
			}, input);
	}

	return isPressed;
}
bool CInput::isInputReleased(InputAction action) {
	bool isPressed = false;
	for (const InputVariant& input : action) {
		std::visit([this, &isPressed](auto&& key) {
			using T = std::decay_t<decltype(key)>;
			if constexpr (std::is_same_v<T, KeyboardKey>) {
				isPressed = isPressed || keyboardInputs[RELEASED][key];
			}
			else if constexpr (std::is_same_v<T, MouseButton>) {
				isPressed = isPressed || mouseInputs[RELEASED][key];
			}
			else if constexpr (std::is_same_v<T, GamepadButton>) {
				isPressed = isPressed || gamepadInputs[RELEASED][key];
			}
			else if constexpr (std::is_same_v<T, JoystickButton>) {
				isPressed = isPressed || joyStickInputs[RELEASED][key];
			}
			}, input);
	}
	return isPressed;
}
#pragma endregion

CRigidBody::CRigidBody() {
	position = Vec2(0.0f, 0.0f);
	velocity = Vec2(0.0f, 0.0f);
	scale = Vec2(1.0f, 1.0f);
	angle = 0;
}
CRigidBody::CRigidBody(const Vec2& position, const Vec2& velocity, float angle, std::vector<Collider> cols) :position(position), velocity(velocity), angle(angle), colliders(cols) {
	scale = Vec2{ 1.0f,1.0f };
}

void CRigidBody::applyForce(Vec2 force) {
	acceleration += force;
}

std::string CRigidBody::stringData() {
	return std::format("pos: {}, {} \nvel: {}, {}", position.x, position.y, velocity.x, velocity.y);
}

CAnimation::CAnimation() = default;
CAnimation::CAnimation(const Animation& animation, bool r, int z) : animation(animation), repeat(r), zIndex(z), position(Vector2(0.0f, 0.0f)), angle(0.0f) {};
CAnimation::CAnimation(const Animation& animation, bool r, Color c) : animation(animation), repeat(r), zIndex(0), position(Vector2(0.0f, 0.0f)), angle(0.0f), tint(c) {};
CAnimation::CAnimation(const Animation& animation, bool r) : animation(animation), repeat(r), position(Vector2(0.0f, 0.0f)), angle(0.0f) {};
CAnimation::CAnimation(const Animation& animation, bool r, Vector2 pos, float a) : animation(animation), repeat(r), position(pos), angle(a) {};
CAnimation::CAnimation(const Animation& animation, bool r, Vector2 pos, float a, Color c) : animation(animation), repeat(r), position(pos), angle(a), tint(c) {};
CAnimation::CAnimation(const Animation& animation, bool r, Vector2 pos, float a, int z) : animation(animation), repeat(r), position(pos), angle(a), zIndex(z) {};


CParticleEmitter::CParticleEmitter(Animation shape, uint16_t amt, Vec2 origin, Range velocityRange, Range angleRange, Range lifeRange, Color c) : shape(shape), amount(amt), origin(origin), tint(c) {
	generateAllParticle(velocityRange, angleRange, lifeRange);
}

void CParticleEmitter::generateAllParticle(Range velocityRange, Range angleRange, Range lifeRange) {
	std::random_device rd;
	gen = std::mt19937(rd());
	lifespanDis = std::uniform_real_distribution<float>(lifeRange.min, lifeRange.max);
	speedDis = std::uniform_real_distribution<float>(velocityRange.min, velocityRange.max);
	angleDis = std::uniform_real_distribution<float>(angleRange.min, angleRange.max);
	for (uint16_t i = 0; i < amount; i++)
	{
		particles.push_back(makeParticle());
	}
}
Particle CParticleEmitter::makeParticle() {
	Particle p;
	p.lifespan = lifespanDis(gen);
	p.initialLifespan = p.lifespan;
	float speed = speedDis(gen);
	float angle = angleDis(gen);

	//set Velocity	
	Vec2 direction = Vec2::DOWN.rotateRad(angle * (PI / 180.0f));
	p.velocity = direction * speed;
	p.position = origin;
	return p;
}
