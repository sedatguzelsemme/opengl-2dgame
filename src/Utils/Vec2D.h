#pragma once

#include <iostream>

class Vec2D
{
public:
	Vec2D():Vec2D(0,0) {};
	Vec2D(float x, float y) :mX(x), mY(y) {}

	inline void SetX(float x) { mX = x; }
	inline void SetY(float y) { mY = y; }
	inline float GetX() { return mX; }
	inline float GetY() { return mY; }


	friend std::ostream& operator<<(std::ostream& consoleOut, const Vec2D& vec2);

private:
	float mX, mY;
};

