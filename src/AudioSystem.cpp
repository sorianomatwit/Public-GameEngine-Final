#pragma once
#include "EngineSystems.hpp"

void AudioPlaySystem::Execute(std::vector<uint16_t> entities, float deltatime) {
	for (auto id : entities) {
		auto entity = Entity(id);
		if (!entity.isActive()) continue;
		auto& audioData = entity.getComponent<CAudio>();

		if (audioData.type == MUSIC) {
			auto& music = audioData.music;
			if (!IsMusicStreamPlaying(music)) {
				PlayMusicStream(music);
				audioData.hasStarted = true;
			}
			UpdateMusicStream(music);
		}
		else {
			auto& sound = audioData.sound;

			if (!audioData.hasStarted && !IsSoundPlaying(sound)) {
				PlaySound(sound);
				audioData.hasStarted = true;
			}
		}
	}
}


void AudioCleanUpSystem::Execute(std::vector<uint16_t> entites, float deltatime) {

	for (int i = entites.size() - 1; i >= 0; i--)
	{
		auto entity = Entity(entites[i]);
		auto& audioData = entity.getComponent<CAudio>();
		if (audioData.type == MUSIC) {
			auto& music = audioData.music;
			if (audioData.hasStarted && !IsMusicStreamPlaying(music) && !audioData.isLooping)
				entity.destroy();
		}
		else if (audioData.type == SOUND) {
			auto& sound = audioData.sound;
			if (audioData.hasStarted && !IsSoundPlaying(sound))
				entity.destroy();
		}
	}
}