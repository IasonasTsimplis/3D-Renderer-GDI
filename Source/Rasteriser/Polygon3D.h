#pragma once
#include "Vector3D.h"
#include <windows.h>

class Polygon3D
{
private:
	int _indices[3];
	bool _backface;
	float _averageZ;
	Vector3D _normal;
	COLORREF _color;

public:
	Polygon3D();
	~Polygon3D();
	Polygon3D(int index0, int index1, int index2);
	Polygon3D(const Polygon3D& p);
	
	int GetIndex(int) const;

	float GetAverageZ() const;
	void SetAverageZ(float value);
	
	bool GetBackface() const;
	void SetBackface(bool value);
	
	COLORREF GetColor() const;
	void SetColor(COLORREF value);

	Vector3D GetNormal() const;
	void SetNormal(Vector3D value);
	
	Polygon3D& operator= (const Polygon3D &rhs);
};
