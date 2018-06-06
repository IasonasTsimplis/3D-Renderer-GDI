#include "Polygon3D.h"

// Empty constructor
Polygon3D::Polygon3D()
{
	_indices[0] = 0;
	_indices[1] = 0;
	_indices[2] = 0;
	_backface = false;
}

// Destructor
Polygon3D::~Polygon3D() {}

// Param constructor
Polygon3D::Polygon3D(int index0, int index1, int index2)
{
	_indices[0] = index0;
	_indices[1] = index1;
	_indices[2] = index2;
	_averageZ = 0;
	_backface = false;
}

// Copy constructor
Polygon3D::Polygon3D(const Polygon3D& p)
{
	_indices[0] = p.GetIndex(0);
	_indices[1] = p.GetIndex(1);
	_indices[2] = p.GetIndex(2);
	_backface = p.GetBackface();
	_averageZ = p.GetAverageZ();
	_color = p.GetColor();
	_normal = p.GetNormal();

}

// Accessors
int Polygon3D::GetIndex(int i) const
{
	return _indices[i];
}


bool Polygon3D::GetBackface() const
{
	return _backface;
}

void Polygon3D::SetBackface(bool value)
{
	_backface = value;
}


float Polygon3D::GetAverageZ() const
{
	return _averageZ;
}

void Polygon3D::SetAverageZ(float value)
{
	_averageZ = value;
}


COLORREF Polygon3D::GetColor() const
{
	return _color;
}

void Polygon3D::SetColor(COLORREF value)
{
	_color = value;
}


Vector3D Polygon3D::GetNormal() const
{
	return _normal;
}

void Polygon3D::SetNormal(Vector3D value)
{
	_normal = value;
}


// Operators
Polygon3D& Polygon3D::operator=(const Polygon3D &rhs)
{
	if (this != &rhs)
	{
		_indices[0] = rhs.GetIndex(0);
		_indices[1] = rhs.GetIndex(1);
		_indices[2] = rhs.GetIndex(2);
		_backface = rhs.GetBackface();
		_averageZ = rhs.GetAverageZ();
		_normal = rhs.GetNormal();
		_color = rhs.GetColor();
	}
	return *this;
}
