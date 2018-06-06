#include "DirectionalLighting.h"

DirectionalLighting::DirectionalLighting()
{
}


DirectionalLighting::~DirectionalLighting()
{
}

DirectionalLighting::DirectionalLighting(Vertex position, COLORREF color)
{
	_position = position;
	_color = color;
}

// Accessors
COLORREF DirectionalLighting::GetIntensity()
{
	return _color;
}
Vertex DirectionalLighting::GetPosition()
{
	return _position;
}