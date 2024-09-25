
#include "Animation.hpp"

Animation::Animation() = default;

Animation::Animation(const Texture2D& tex, Vec2 scaledSize) : Animation(tex, scaledSize, 1, 0) {}

Animation::Animation(const Texture2D& tex, Vec2 scaledSize, int frameCount, int speed) : texture(tex), frameCount(frameCount), speed(speed) {
	this->scale = scaledSize;
	this->scaledSize = scaledSize;
	this->scaledSize.x *= tex.width / frameCount;
	this->scaledSize.y *= tex.height;
	size = { (float)(tex.width / frameCount),(float)tex.height };
	frameRect = { currentFrame * size.x,0,(float)size.x,(float)size.y };
}

Animation::Animation(const Animation& anim, Vec2 scaledSize) : texture(anim.texture), frameCount(anim.frameCount), speed(anim.speed) {
	this->scale = scaledSize;
	this->scaledSize = scaledSize;
	this->scaledSize.x *= texture.width / frameCount;
	this->scaledSize.y *= texture.height;
	size = { (float)(texture.width / frameCount),(float)texture.height };
	frameRect = { currentFrame * size.x,0,(float)size.x,(float)size.y };
}

/**
 * Updates the raylib rectangle that is used as the source texture for the animation based on anim speed and frame count
 */
void Animation::update() {
	if (speed > 0) {
		currentFrame++;
		if (currentFrame >= frameCount * speed) {
			currentFrame = 0;  // Reset to the beginning of the animation
		}
		int frame = currentFrame / speed;
		frameRect = { frame * size.x, 0, (float)size.x, (float)size.y };
	}
}

/**
 * Gets the entire animation texture
 *
 * @return Raylib texture2D that contains all animation frames
 */
const Texture2D& Animation::getTexture() const {
	return texture;
}


/**
 * Checks if the animation has reached its frame count
 *
 * @return boolean if the animation as reached the end of its frame count
 */
bool Animation::hasEnded() const {
	if (speed > 0 && (currentFrame / speed) % frameCount == frameCount - 1) {
		return true;
	}
	else {
		return false;
	}
}
/**
 * Checks if the animation has reached its frame count
 *
 * @return boolean if the animation as reached the end of its frame count
 */
const Vec2& Animation::getSize() const {
	return size;
}

/**
 * Gets the (x,y) scaled size of the animation
 *
 * @return Vec2 that contains the x an y scaled size of the animation
 */
const Vec2& Animation::getScaledSize() const {
	return scaledSize;
}


/**
 * Gets the rectangle that corresponds to the current animation frame
 *
 * @return Raylib rectangle that frames the current animation frame in the animation texture
 */
const Rectangle& Animation::getFrameRect() const {
	return frameRect;
}