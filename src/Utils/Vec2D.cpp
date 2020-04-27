#include "Vec2D.h"

std::ostream& operator<<(std::ostream& consoleOut, const Vec2D& vec2) 
{
	std::cout << "X: " << vec2.mX << ", Y: " << vec2.mY << std::endl;
	return consoleOut;
}