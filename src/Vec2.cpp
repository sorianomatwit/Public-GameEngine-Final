#include "Vec2.hpp"
#include <math.h>


Vec2 Vec2::UP = Vec2(0, 1);
Vec2 Vec2::DOWN = Vec2(0, -1);
Vec2 Vec2::LEFT = Vec2(-1, 0);
Vec2 Vec2::RIGHT = Vec2(1, 0);

Vec2::Vec2() {
	this->x = 0.0f;
	this->y = 0.0f;
}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

/**
 * Computes the length of the vector
 *
 * @return float that contains the length of the vector
 */
float Vec2::length() const {
	return sqrtf(x * x + y * y);
}

/**
 * Computes the distance between this vector and v
 *
 * @param v vector to compute distance to
 */
float Vec2::dist(const Vec2& v) const {
	return (v - *this).length();
}

/**
 * Normalizes the vector to length 1
 *
 * @return Vec2 with length 1
 */
Vec2 Vec2::normalize() const {
	float l = length();
	return Vec2(x / l, y / l);
}

Vec2 Vec2::rotateRad(float angle) const {
	auto cosTheta = cosf(angle);
	auto sinTheta = sinf(angle);

	return Vec2(x * cosTheta - y * sinTheta, x * sinTheta + y * cosTheta);
}

float Vec2::angleBetween(Vec2 v) const {

	float dot = *this * v;
	float mags = this->length() * v.length();
	float cosTheta = dot / mags;
	return acosf(cosTheta);
}


//Operator overloads

bool Vec2::operator ==(const Vec2& rhs) const {
	return (rhs.x == x && rhs.y == y);
}
Vec2 Vec2::operator + (const Vec2& rhs) const {
	return Vec2(x + rhs.x, y + rhs.y);
}
Vec2 Vec2::operator * (float value) const {
	return Vec2(x * value, y * value);
}
void Vec2::operator +=(const Vec2& rhs) {
	x += rhs.x;
	y += rhs.y;
}
Vec2 Vec2::operator -(const Vec2& rhs) const {
	return Vec2(x - rhs.x, y - rhs.y);
}
void Vec2::operator -=(const Vec2& rhs) {
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator -=(float value) {
	x -= value;
	y -= value;
}

Vec2 Vec2::operator -(float value) const {
	return Vec2(x - value, y - value);

}

Vec2 Vec2::operator +(float value) const {
	return Vec2(x + value, y + value);
}

void Vec2::operator +=(float value) {
	x += value;
	y += value;
}
float Vec2::operator *(const Vec2& rhs) const {
	return x * rhs.x + y * rhs.y;
}
void Vec2::operator *=(const Vec2& rhs) {
	x = x * rhs.x;
	y = y * rhs.y;
}

void Vec2::operator *=(float value) {
	x = x * value;
	y = y * value;
}
