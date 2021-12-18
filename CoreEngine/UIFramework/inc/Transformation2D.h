#pragma once
#include <iostream>
#include <math.h>
#include <stdlib.h>

template<class T>
void scale(T& x, T& y, T scaleX, T scaleY, T originX, T originY)
{
	x = (x - originX) * scaleX + originX;
	y = (y - originY) * scaleY + originY;
}

template<class T>
void rotate(T& x, T& y, T angle, T originX, T originY)
{
	//get radian
	double radians = 6.283185308 / (360.0 / angle);

	x = x - originX;
	y = y - originY;

	double rotatedX = x * cos(radians) - y * sin(radians);
	double rotatedY = y * cos(radians) + x * sin(radians);

	x = rotatedX + originX;
	y = rotatedY + originY;
}

template<class T>
void scale(T* quad, T scaleX, T scaleY, T originX, T originY)
{
	for (int i = 0; i < 8; i += 2)
	{
		scale(quad[i], quad[i + 1], scaleX, scaleY, originX, originY);
	}
}

template<class T>
void rotate(T* quad, T angle, T originX, T originY)
{
	//get radian
	double radians = 6.283185308 / (360.0 / angle);
	for (int i = 0; i < 8; i+=2)
	{
		rotate(quad[i], quad[i + 1], angle, originX, originY);
	}
}