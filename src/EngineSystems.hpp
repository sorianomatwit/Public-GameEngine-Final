#pragma once
#include "EngineComponents.hpp"


/**
* This set the input for all entites with the CInput component
**/
class InputSystem : public System<CInput> {
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};


class RenderSystem : public System<CAnimation> {
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

class DrawGuiSystem : public System<CGui> {
protected: 
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

class ParticleSystem : public System<CParticleEmitter> {
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

class ParticleCleanUpSystem : public System<CParticleEmitter> {
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

class AudioPlaySystem : public System<CAudio> {
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};

class AudioCleanUpSystem : public System<CAudio> {
protected:
	void Execute(std::vector<uint16_t> entites, float deltatime) override;
};