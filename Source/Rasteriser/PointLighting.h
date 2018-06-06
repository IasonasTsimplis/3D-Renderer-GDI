#pragma once
#include "Vertex.h"
#include <windows.h>

class PointLighting
{
private:
	Vertex _position;
	COLORREF _color;

public:
	
	PointLighting();
	~PointLighting();

	PointLighting(Vertex position, COLORREF color);

	COLORREF GetIntensity();
	Vertex GetPosition();
};

