#include "Vector3D.h"
#include <cmath>

// Empty Constructor
Vector3D::Vector3D()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

// Destructor
Vector3D::~Vector3D()
{
}

// Parameters Constructor
Vector3D::Vector3D(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

// Copy Constructor 
Vector3D::Vector3D(const Vector3D& v)
{
	_x = v.GetX();
	_y = v.GetY();
	_z = v.GetZ();
}

// Accessors Get & Set
//---------------------
float Vector3D::GetX() const
{
	return _x;
}

void Vector3D::SetX(const float x)
{
	_x = x;
}

float Vector3D::GetY() const
{
	return _y;
}

void Vector3D::SetY(const float y)
{
	_y = y;
}

float Vector3D::GetZ() const
{
	return _z;
}

void Vector3D::SetZ(const float z)
{
	_z = z;
}


// Length of a vector
float Vector3D::Length()
{
	return sqrtf((_x*_x) + (_y*_y) + (_z*_z));
}


// Normalise a vector
void Vector3D::Normalise()
{
	float Len = Length();

	_x = (_x / Len);
	_y = (_y / Len);
	_z = (_z / Len);
}


// Operators
Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
	if (this != &rhs)
	{
		_x = rhs.GetX();
		_y = rhs.GetY();
		_z = rhs.GetZ();
	}
	return *this;
}


const Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
	return Vector3D(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ());
}


bool Vector3D::operator==(const Vector3D& rhs) const
{
	return (_x == rhs.GetX() && _y == rhs.GetY() && _z == rhs.GetZ());
}


// Dot Product of 2 vectors
float Vector3D::GetDotProduct(const Vector3D& rhs) const
{
	return _x * rhs.GetX() + _y * rhs.GetY() + _z * rhs.GetZ();
}


// Cross Poduct of 2 vectors
Vector3D Vector3D::GetCrossProduct(const Vector3D& rhs) const
{
	return Vector3D(_y * rhs.GetZ() - _z * rhs.GetY(), _z * rhs.GetX() - _x * rhs.GetZ(), _x * rhs.GetY() - _y * rhs.GetX());
}

