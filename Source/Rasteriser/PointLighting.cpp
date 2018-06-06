#include "PointLighting.h"

PointLighting::PointLighting()
{
}


PointLighting::~PointLighting()
{
}

PointLighting::PointLighting(Vertex position, COLORREF color)
{
	_position = position;
	_color = color;
}

// Accessors
COLORREF PointLighting::GetIntensity()
{
	return _color;
}
Vertex PointLighting::GetPosition()
{
	return _position;
}
