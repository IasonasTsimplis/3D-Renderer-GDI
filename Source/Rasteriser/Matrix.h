#pragma once
#include "Vertex.h"

class Matrix
{
private:
	float elements[4][4];
	const float pi = 3.1415926535897932384626433832795f;

public:
	Matrix();
	Matrix(const Matrix& other);
	~Matrix();
	
	float GetM(const int row, const int column) const;
	void SetM(const int row, const int column, const float value);
	
	Matrix& operator= (const Matrix &rhs);
	const Matrix operator*(const Matrix &other) const;
	const Vertex operator*(const Vertex &other) const;

	static Matrix UnitMatrix();

	static Matrix TranslationMatrix(float Tx, float Ty, float Tz);

	static Matrix ScalingMatrix(float Sx, float Sy, float Sz);

	static Matrix XRotationMatrix(float degrees);
	
	static Matrix YRotationMatrix(float degrees);
	
	static Matrix ZRotationMatrix(float degrees);

	static Matrix ViewportMatrix(int x0, int y0, int width, int height, float d);

	static Matrix PerspectiveMatrix(float d, float aspectRatio);

	static Matrix CameraMatrix(float eyeX, float eyeY, float eyeZ, float thetaX, float thetaY, float thetaZ);
};
