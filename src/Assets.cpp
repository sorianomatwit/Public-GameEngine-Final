#include "Assets.hpp"
#include <sstream>
#include <iostream>

Assets::Assets() = default;

Assets::~Assets() {
	//unload fonts
	for (auto& [name, font] : fontMap) {
		UnloadFont(font);
	}
	//unload Texture2Ds
	for (auto& [name, tex] : animationMap) {
		UnloadTexture(tex.getTexture());
	}

	for (auto& [_,sound] : soundMap )
	{
		UnloadSound(sound);
	}
	for (auto& [_, music] : musicMap)
	{
		UnloadMusicStream(music);
	}
}

/**
 * Loads the game textures, fonts, and animation definitions from the asset definition file
 *
 * @param path string that contains the path to the asset definition file
 */
void Assets::load(const std::string path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << path << std::endl;
		return;
	}

	std::string line;
	bool headerSkipped = false;
	while (std::getline(file, line)) {
		if (!headerSkipped) {
			headerSkipped = true;
			continue; // Skip header row
		}

		std::istringstream iss(line);
		std::string type, name, path;
		int frameCount, speed;
		float scaleFactor;

		// Read CSV fields
		if (!(iss >> type >> name >> path)) {
			std::cerr << "Error reading line: " << line << std::endl;
			continue; // Skip invalid lines
		}
		// Determine object type and call respective function
		switch (toascii(type[0]))
		{
		case TEXTURE:
			iss >> frameCount >> speed >> scaleFactor;
			addAnimation(name, path, frameCount, speed, scaleFactor);
			break;
		case FONT:
			addFont(name, path);
			break;
		case SOUND:
			addSound(name, path);
			break;
		case MUSIC:
			addMusic(name, path);
			break;
		default:
			std::cerr << "Unknown object type: " << type << std::endl;
			break;
		}
	}
	file.close();
}

/**
 * Gets the raylib font asset based on name
 *
 * @param name Font name
 */
const Font& Assets::getFont(const std::string& name) const {
	return fontMap.at(name);
}

/**
 * Gets the animation asset based on name
 *
 * @param name Animation name
 */
const Animation& Assets::getAnimation(const std::string& name) const {
	return animationMap.at(name);
}

const Sound& Assets::getSound(const std::string& name) const {
	return soundMap.at(name);
}

const Music& Assets::getMusic(const std::string& name) const {
	return musicMap.at(name);
}


/**
 * Loads the font using raylib library and adds to the font map data structure
 *
 * @param name Font name
 * @param path Font path relative to the exe
 */
void Assets::addFont(const std::string& name, const std::string& path) {
	Font font = LoadFont(path.c_str());
	fontMap[name] = font;
}

void Assets::addSound(const std::string& name, const std::string& path) {
	Sound sound = LoadSound(path.c_str());
	soundMap[name] = sound;
}

void Assets::addMusic(const std::string& name, const std::string& path) {
	Music music = LoadMusicStream(path.c_str());
	musicMap[name] = music;
}

/**
 * Creates the animation object from the input information
 *
 * @param name Animation name
 * @param textureName Texture name for use with this animation
 * @param frameCount Number of frame in this animation
 * @param speed How fast to change each frame when running
 * @param scaleFactor int that determins the up-scale of the texture when animating
 */
void Assets::addAnimation(const std::string& name, const std::string& path, int frameCount, int speed, float scaleFactor) {
	Texture2D tex = LoadTexture(path.c_str());
	tex.width *= scaleFactor;
	tex.height *= scaleFactor;
	SetTextureFilter(tex, TEXTURE_FILTER_POINT);
	Animation anim = Animation(tex, { (float)scaleFactor,(float)scaleFactor }, frameCount, speed);
	animationMap[name] = anim;
}
