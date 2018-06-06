#pragma once

class Vector3D
{

private:
	float _x;
	float _y;
	float _z;

public:
	Vector3D();
	~Vector3D();
	Vector3D(float x, float y, float z); 
	Vector3D(const Vector3D& v);

	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);

	float GetDotProduct(const Vector3D& rhs) const;
	Vector3D GetCrossProduct(const Vector3D& rhs) const;
	float Length();
	void Normalise();
	
	const Vector3D operator+(const Vector3D& rhs) const;
	Vector3D& operator=(const Vector3D& rhs);
	bool operator==(const Vector3D& rhs) const;

};

