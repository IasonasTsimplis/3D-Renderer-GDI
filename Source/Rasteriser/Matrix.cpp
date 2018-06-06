#include "Matrix.h"
#include <cmath>

// Constructor
Matrix::Matrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			elements[i][j] = 0.0f;
		}
	}
}

// Destructor
Matrix::~Matrix()
{
}

// Copy constructor
Matrix::Matrix(const Matrix& other)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			elements[i][j] = other.GetM(i, j);
		}
	}
}

// Accessor to Get element
float Matrix::GetM(const int row, const int column) const
{
	return elements[row][column];
}


// Accessor to Set element
void Matrix::SetM(const int row, const int column, const float value)
{
	elements[row][column] = value;
}


// Matrix = Matrix
Matrix& Matrix::operator=(const Matrix &rhs)
{
	if (this != &rhs)
	{	
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				elements[i][j] = rhs.GetM(i, j);
			}
		}
	}
	return *this;
}

// Matrix * Matrix
const Matrix Matrix::operator*(const Matrix& rhs) const
{
	Matrix matrixResult;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrixResult.elements[i][j] =
			GetM(i, 0) * rhs.GetM(0, j) +
			GetM(i, 1) * rhs.GetM(1, j) +
			GetM(i, 2) * rhs.GetM(2, j) +
			GetM(i, 3) * rhs.GetM(3, j);
		}
	}

	return matrixResult;
}

// Matrix * Vertex
const Vertex Matrix::operator*(const Vertex& rhs) const
{
	Vertex vertexResult;
	float temp[4];		

	vertexResult = rhs;

	for (int i = 0; i < 4; i++) 
	{
		temp[i] =
		GetM(i, 0) * rhs.GetX() +
		GetM(i, 1) * rhs.GetY() +
		GetM(i, 2) * rhs.GetZ() +
		GetM(i, 3) * rhs.GetW();
	}

	vertexResult.SetX(temp[0]);
	vertexResult.SetY(temp[1]);
	vertexResult.SetZ(temp[2]);
	vertexResult.SetW(temp[3]);

	return vertexResult;
}

// Unit (also works as ortho)
Matrix Matrix::UnitMatrix()
{
	static Matrix m;

	m.SetM(0, 0, 1.0f);
	m.SetM(1, 1, 1.0f);
	m.SetM(2, 2, 1.0f);
	m.SetM(3, 3, 1.0f);

	return m;
}

// Translation
Matrix Matrix::TranslationMatrix(float Tx, float Ty, float Tz)
{
	static Matrix m;
	m = UnitMatrix();

	m.SetM(0, 3, Tx);
	m.SetM(1, 3, Ty);
	m.SetM(2, 3, Tz);

	return m;
}

// Scaling
Matrix Matrix::ScalingMatrix(float Sx, float Sy, float Sz)
{
	static Matrix m;
	m = UnitMatrix();

	m.SetM(0, 0, Sx);
	m.SetM(1, 1, Sy);
	m.SetM(2, 2, Sz);
	m.SetM(3, 3, 1.0f);

	return m;
}

// Rotation (X)
Matrix Matrix::XRotationMatrix(float d)
{	
	static Matrix m;
	m = UnitMatrix();
	d = (d*(m.pi)) / 180.0f;

	m.SetM(1, 1, cos(d));		m.SetM(1, 2, -sin(d));
	m.SetM(2, 1, sin(d));		m.SetM(2, 2, cos(d));

	return m;
}

// Rotation (Y)
Matrix Matrix::YRotationMatrix(float d)
{
	static Matrix m;
	m = UnitMatrix();
	d = (d*(m.pi)) / 180.0f;

	m.SetM(0, 0, cos(d));		m.SetM(0, 2, sin(d));
	m.SetM(2, 0, -sin(d));		m.SetM(2, 2, cos(d));

	return m;
}

// Rotation (Z)
Matrix Matrix::ZRotationMatrix(float d)
{
	static Matrix m;
	m = UnitMatrix();
	d = (d*(m.pi)) / 180.0f;

	m.SetM(0, 0, cos(d));		m.SetM(0, 1, -sin(d));
	m.SetM(1, 0, sin(d));		m.SetM(1, 1, cos(d));

	return m;
}

// Viewport
Matrix Matrix::ViewportMatrix(int x0, int y0, int width, int height, float d)
{
	static Matrix m;
	m = UnitMatrix();
	m.SetM(0, 0, (float)(width / 2));
	m.SetM(1, 1, (float)(-height / 2));
	m.SetM(2, 2, (float)(d / 2));
	m.SetM(0, 3, (float)(x0));
	m.SetM(1, 3, (float)(y0));
	m.SetM(2, 3, (float)(d / 2));
	m.SetM(3, 3, 1.0f);

	return m;
}

// Perspective
Matrix Matrix::PerspectiveMatrix(float d, float a)
{
	static Matrix m;

	m.SetM(0, 0, d / a);
	m.SetM(1, 1, d);
	m.SetM(2, 2, d);
	m.SetM(3, 2, 1.0f);

	return m;
}

// Camera
Matrix Matrix::CameraMatrix(float eyeX, float eyeY, float eyeZ, float thetaX, float thetaY, float thetaZ)
{
	static Matrix camera, rz, ry, rx, t;

	t = Matrix::TranslationMatrix(-eyeX, -eyeY, -eyeZ);
	rz = Matrix::ZRotationMatrix(thetaZ);
	ry = Matrix::YRotationMatrix(thetaY);
	rx = Matrix::XRotationMatrix(thetaX);
	
	camera = rx * ry * rz * t;

	return camera;

}