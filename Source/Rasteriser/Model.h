#pragma once
#include "Polygon3D.h"
#include "Vector3D.h"
#include "Vertex.h"
#include "Matrix.h"
#include <vector>

class Model
{
private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;

	float kaR, kaB, kaG;
	float kdR, kdB, kdG;
	float ksR, ksB, ksG;
	float shininess;

public:
	Model();
	~Model();

	std::vector<Polygon3D>& GetPolygons();
	std::vector<Vertex>& GetVertices();
	
	size_t GetPolygonCount() const;
	size_t GetVerticesCount() const;
	
	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);

	void Sort(void);
	void ApplyTransformation(Matrix matrix);
	void CalculateBackfaces(float eX, float eY, float eZ);
	void CalculateDirectionalLighting(Vertex lightPos, COLORREF clr);
	void CalculatePointLighting(Vertex lightPos, float cX, float cY, float cZ, COLORREF clr);
	void DehomogeniseCoords();
	Vector3D CalculateNormal(Vertex ver0, Vertex ver1, Vertex ver2);
	COLORREF ClampColor(COLORREF color);
};