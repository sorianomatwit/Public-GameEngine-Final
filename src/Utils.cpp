#pragma once
#include "Utils.hpp"
#include <cmath>
#include <utility>

uint32_t Lehmer32(uint32_t num) {
	uint32_t nLehmer = num + 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

float Lehmer32Float(uint32_t seed) {
	uint32_t nLehmer = seed + 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;

	seed = m2; // Update the seed

	// Convert to a float in the range [0.0, 1.0)
	return m2 / (float)UINT32_MAX;
}

Texture2D createCircleTexture(float radius, Color c) {
	RenderTexture2D shape = LoadRenderTexture(radius * 2, radius * 2);
	BeginTextureMode(shape);
	ClearBackground(BLANK);
	DrawCircle(radius, radius, radius, c);
	EndTextureMode();
	return shape.texture;
}

Texture2D createRectTexture(float width, float height, Color c) {
	RenderTexture2D shape = LoadRenderTexture(width, height);
	BeginTextureMode(shape);
	ClearBackground(BLANK);
	DrawRectangle(0, 0, width, height, WHITE);
	DrawRectangle(2.5f, 2.5f, width - 5, height - 5, c);
	EndTextureMode();
	return shape.texture;
}

Vec2 getOverlap(Rect a, Rect b) {
	Vec2 diff = Vec2(a.pos.x, a.pos.y) - Vec2(b.pos.x, b.pos.y);
	Vec2 delta(fabsf(diff.x), fabsf(diff.y));
	Vec2 area = Vec2(a.size.x, a.size.y) + Vec2(b.size.x, b.size.y);
	Vec2 result = area = delta;

	return result;
}

bool RectContains(Rect outer, Rect inner) {
	return inner.pos.x >= outer.pos.x &&
		inner.pos.x + inner.size.x <= outer.pos.x + outer.size.x &&
		inner.pos.y >= outer.pos.y &&
		inner.pos.y + inner.size.y <= outer.pos.y + outer.size.y;
}

bool RectOverlaps(Rect a, Rect b) {
	return (a.pos.x < b.pos.x + b.size.x &&
		a.pos.x + a.size.x > b.pos.x &&
		a.pos.y < b.pos.y + b.size.y &&
		a.pos.y + a.size.y > b.pos.y);
}

bool CircleOverlaps(Circ a, Circ b) {
	Vec2 d = a.pos - b.pos;
	float distanceSquared = d.x * d.x + d.y * d.y;
	float radiusSum = a.radius + b.radius;
	float radiusSumSquared = radiusSum * radiusSum;

	return distanceSquared <= radiusSumSquared;
}

float getCircOverlap(Circ a, Circ b) {
	float distance = (a.pos - b.pos).length(); 
	return 0.5f * (distance - a.radius - b.radius);
}
Vec2 getDirectionCirc(Circ a, Circ b) {
	return (a.pos - b.pos).normalize();
}
bool RectOverlapsCirc(Rect outer, Circ inner) {
	float closestX = std::max(outer.pos.x, std::min(inner.pos.x, outer.pos.x + outer.size.x));
	float closestY = std::max(outer.pos.y, std::min(inner.pos.y, outer.pos.y + outer.size.x));

	float distanceX = closestX - inner.pos.x;
	float distanceY = closestY - inner.pos.y;

	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

	return distanceSquared <= (inner.radius * inner.radius);
}

bool RectContainsCirc(Rect outer, Circ inner) {
	bool output = (inner.pos.x - inner.radius >= outer.pos.x) &&
		(inner.pos.x + inner.radius <= outer.pos.x + outer.size.x) &&
		(inner.pos.y - inner.radius >= outer.pos.y) &&
		(inner.pos.y + inner.radius <= outer.pos.y + outer.size.y);
	return output;
}

float getRectCircOverlap(Rect rect, Circ circ) {
	float closestX = std::max(rect.pos.x, std::min(circ.pos.x, rect.pos.x + rect.size.x));
	float closestY = std::max(rect.pos.y, std::min(circ.pos.y, rect.pos.y + rect.size.y));

	// Calculate the distance between the circle's center and this closest point
	float distance = circ.pos.dist(Vec2(closestX, closestY));

	// Calculate overlap
	float overlap = circ.radius - distance;
	return overlap > 0 ? overlap : 0;
}

Vec2 calculateCircOverlapBoundary(const Circ& circle, float left, float right, float top, float bottom) {
	Vec2 overlap;

	// Calculate X axis overlap
	float leftOverlap = left - (circle.pos.x - circle.radius);
	float rightOverlap = (circle.pos.x + circle.radius) - right;
	overlap.x = std::max(leftOverlap, 0.0f) - std::max(rightOverlap, 0.0f);

	// Calculate Y axis overlap
	float topOverlap = top - (circle.pos.y - circle.radius);
	float bottomOverlap = (circle.pos.y + circle.radius) - bottom;
	overlap.y = std::max(topOverlap, 0.0f) - std::max(bottomOverlap, 0.0f);

	return overlap;
}