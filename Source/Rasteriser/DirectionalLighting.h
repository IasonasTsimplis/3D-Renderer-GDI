#pragma once
#include "Vertex.h"
#include <windows.h>

class DirectionalLighting
{
private:
	Vertex _position;
	COLORREF _color;

public:

	DirectionalLighting();
	~DirectionalLighting();

	DirectionalLighting(Vertex position, COLORREF color);

	COLORREF GetIntensity();
	Vertex GetPosition();
};

