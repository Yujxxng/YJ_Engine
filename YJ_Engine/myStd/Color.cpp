#include "Color.h"

Color Color4fMake(float r, float g, float b, float a)
{
	Color c;
	c.r = (unsigned char)(r * 0xFF);
	c.g = (unsigned char)(g * 0xFF);
	c.b = (unsigned char)(b * 0xFF);
	c.a = (unsigned char)(a * 0xFF);

	return c;
}