#pragma once
#include <include/raylib.h>
#include <string>
#include "Vec2.hpp"

/**
 *
 * Contains all size, state, and asset information related to a single animation
 *
 * Animations are created for each texture in the game. Single textures are considered to have one frame of animation that repeats.
 * All entities that appear in the game should have an animation component with an associated animation object.
 *
 */
class Animation {
public:
    int frameCount = 1;       /* Number of frames in this animation */
    int currentFrame = 0;     /* Current animation frame */
    int speed = 1;            /* Frame delay between animation frames (small is faster) */
    Texture2D texture;      /* Raylib texture that contains the full animation sheet */
    Vec2 scaledSize = { 1,1 };  /* Scaled size (pixels) of the animation frame */
    Vec2 scale = { 1,1 };
    Rectangle frameRect;    /* Raylib rectangle */
    Vec2 size = { 1,1 };        /* Size (pixels) of the animation frame */
    Animation();
    Animation(const Texture2D& tex, Vec2 scaledSize);
    Animation(const Texture2D& tex, Vec2 scaledSize, int frameCount, int speed);
    Animation(const Animation& anim, Vec2 scaledSize);


    void update();
    bool hasEnded() const;
    const std::string& getName() const;
    const Vec2& getScaledSize() const;
    const Vec2& getSize() const;
    const Texture2D& getTexture() const;
    const Rectangle& getFrameRect() const;
    
};
