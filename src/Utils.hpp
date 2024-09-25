#pragma once
#include <stdint.h>
#include <raylib.h>
#include "Vec2.hpp"
#include "math.h"

struct Rect {
	Vec2 pos;
	Vec2 size;
};
struct Circ {
	Vec2 pos;
	float radius;
};

uint32_t Lehmer32(uint32_t num);

float Lehmer32Float(uint32_t seed);

Texture2D createCircleTexture(float radius, Color c);

Texture2D createRectTexture(float width, float height, Color c);

Vec2 getOverlap(Rect a, Rect b);

bool RectContains(Rect outer, Rect inner);

bool RectOverlaps(Rect a, Rect b);

bool CircleOverlaps(Circ a, Circ b);

bool RectContainsCirc(Rect outer, Circ inner);

bool RectOverlapsCirc(Rect outer, Circ inner);

float getCircOverlap(Circ a, Circ b);

Vec2 getDirectionCirc(Circ a, Circ b);

float getRectCircOverlap(Rect rect, Circ circ);

Vec2 calculateCircOverlapBoundary(const Circ& circle, float left, float right, float top, float bottom);