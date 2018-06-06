#pragma once
#include "Vector3D.h"

class Vertex
{
private:
	float _x;
	float _y;
	float _z;
	float _w;

public:
	Vertex();
	Vertex(float x, float y, float z);
	Vertex(float x, float y, float z, float w);  
	Vertex(const Vertex& v);
	~Vertex();

	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);
	float GetW() const;
	void SetW(const float w);

	void Dehomogenise();

	Vertex& operator=(const Vertex& rhs);
	bool operator==(const Vertex& rhs) const;
	const Vertex operator+(const Vertex& rhs) const;
	const Vector3D operator-(const Vertex &rhs) const;
};

