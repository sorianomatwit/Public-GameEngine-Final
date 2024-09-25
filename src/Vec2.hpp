
#pragma once

/**
 * Contains two float values that can be used as a vector
 */
class Vec2 {
public:
	static Vec2 UP;
	static Vec2 DOWN;
	static Vec2 LEFT;
	static Vec2 RIGHT;
	float x; /* x value in vector */
	float y; /* y value in vector */

	Vec2();
	Vec2(float x, float y);

	float length() const;
	float dist(const Vec2& v) const;
	Vec2 normalize() const;
	
	float angleBetween(Vec2 v) const;

	Vec2 rotateRad(float angle) const;

	bool operator ==(const Vec2& rhs) const;
	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator * (float value) const;
	Vec2 operator -(const Vec2& rhs) const;
	float operator *(const Vec2& rhs) const;
	void operator +=(const Vec2& rhs);
	void operator -=(const Vec2& rhs);
	void operator *=(const Vec2& rhs);
	void operator *=(float value);
	void operator -=(float value);
	Vec2 operator -(float value) const;
	Vec2 operator +(float value) const;
	void operator +=(float value);

};