#pragma once
#include <map>
#include <string>
#include <fstream>
#include "Animation.hpp"

/**
 * Contains the ascii code for the first letter of the asset type
 */


 /**
  *
  * Contains maps (string->Asset Type) for Font, Texture, and Animation objects. Maps are filled when a scene loads.
  *
  * Primarily used to retrieve an asset by string name.
  *
  */
class Assets {
public:
	enum ASSETYPES { TEXTURE = 'T', FONT = 'F', SOUND = 'S', MUSIC = 'M' };
	std::map<std::string, Font> fontMap;           /* Map from string font name to raylib Font struct */
	std::map<std::string, Animation> animationMap; /* Map from string animation name to Animation object */
	std::map<std::string, Music> musicMap;
	std::map<std::string, Sound> soundMap;
	Assets();
	~Assets();
	void load(const std::string path);
	const Font& getFont(const std::string& name) const;
	const Animation& getAnimation(const std::string& name) const;
	const Music& getMusic(const std::string& name) const;
	const Sound& getSound(const std::string& name) const;

	void addFont(const std::string& name, const std::string& path);
	void addSound(const std::string& name, const std::string& path);
	void addMusic(const std::string& name, const std::string& path);
	void addAnimation(const std::string& name, const std::string& texture, int frameCount, int speed, float scaleFactor);
};