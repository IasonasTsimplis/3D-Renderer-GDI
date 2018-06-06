#include "Rasteriser.h"
#include "Model.h"
#include <algorithm>
#include <cmath>

// Constructor
Model::Model()
{
	kaR = 0.1f;
	kaG = 0.0f;
	kaB = 0.0f;

	kdR = 0.2f;
	kdG = 0.0f; 
	kdB = 0.0f;

	ksR = 0.5f;
	ksG = 0.5f; 
	ksB = 0.5f;

	shininess = 16;
}

// Destructor
Model::~Model()
{
}

// Polygon List Accessor
std::vector<Polygon3D>& Model::GetPolygons()
{
	return _polygons;
}

// Vertex List Accessor
std::vector<Vertex>& Model::GetVertices()
{
	return _vertices;
}

// Polygon List Size Accessor
size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

// Vertex List Size Accessor
size_t Model::GetVerticesCount() const
{
	return _vertices.size();
}


// Add a Vertex to the list
void Model::AddVertex(float x, float y, float z)
{
	Vertex newVertex(x, y, z);
	_vertices.push_back(newVertex);
}


// Add a Polygon to the list
void Model::AddPolygon(int i0, int i1, int i2)
{
	Polygon3D newPolygon3D(i0, i1, i2);
	_polygons.push_back(newPolygon3D);
}


// Apply a Transformation
void Model::ApplyTransformation(Matrix matrix)
{
	for (size_t i = 0; i < GetVerticesCount(); i++)
	{
		 _vertices[i] = matrix * _vertices[i];
	}
}


// Homogenising Coordinates
void Model::DehomogeniseCoords()
{
	for (size_t i = 0; i < GetVerticesCount(); i++)
	{
		_vertices[i].Dehomogenise();
	}
}


// Calculates Backfaces of Model
void Model::CalculateBackfaces(float eX, float eY, float eZ)
{	
	float result;
	Vertex ver0, ver1, ver2, verM;
	Vector3D vectorN;
	Matrix scale;

	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		ver0 = _vertices[_polygons[i].GetIndex(0)];
		ver1 = _vertices[_polygons[i].GetIndex(1)];
		ver2 = _vertices[_polygons[i].GetIndex(2)];

		vectorN = CalculateNormal(ver0, ver1, ver2);
		Vector3D vectorEye = ver0 - Vertex(eX, eY, eZ);
		result = vectorN.GetDotProduct(vectorEye);
		
		if (result <= 0)
			_polygons[i].SetBackface(true);
		else
			_polygons[i].SetBackface(false);
	}
}


// Sort Polygons based on average-Z
void Model::Sort()
{
	float average;
	Vertex ver0, ver1, ver2, verM;
	Matrix scale;

	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		ver0 = _vertices[_polygons[i].GetIndex(0)];
		ver1 = _vertices[_polygons[i].GetIndex(1)];
		ver2 = _vertices[_polygons[i].GetIndex(2)];

		average = ((ver0.GetZ() + ver1.GetZ() + ver2.GetZ()) / 3.0f);
		_polygons[i].SetAverageZ(average);
	}

	std::sort(_polygons.begin(), _polygons.end(), [](const Polygon3D& lhs, const Polygon3D& rhs) {return lhs.GetAverageZ() < rhs.GetAverageZ(); });
}


// Calculates Directional Lighting
void Model::CalculateDirectionalLighting(Vertex lightPos, COLORREF clr)
{
	Vertex ver0, ver1, ver2;
	Vector3D vectorN;

	COLORREF ambRGB, difRGB, specRGB;
	COLORREF tempRGB;
	COLORREF totalRGB;
	COLORREF finalRGB;
	float diffDot, specDot;
	Vector3D lightVector;
	
	lightVector = Vector3D(lightPos.GetX(), lightPos.GetY(), lightPos.GetZ());
	lightVector.Normalise();

	// For every Polygon
	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		ver0 = _vertices[_polygons[i].GetIndex(0)];
		ver1 = _vertices[_polygons[i].GetIndex(1)];
		ver2 = _vertices[_polygons[i].GetIndex(2)];

		vectorN = CalculateNormal(ver0, ver1, ver2);
		_polygons[i].SetNormal(vectorN);

		totalRGB = RGB(0, 0, 0);

		for (int j = 0; j < 1; j++)		// since i only have 1 light
		{
			// Ambient
			ambRGB = RGB(GetRValue(clr) * kaR, GetGValue(clr) * kaG, GetBValue(clr) * kaB);
			tempRGB = RGB(GetRValue(clr), GetGValue(clr), GetBValue(clr));

			// Diffuse
			tempRGB = RGB((GetRValue(tempRGB) * kdR), (GetGValue(tempRGB) * kdG), (GetBValue(tempRGB) * kdB));
			diffDot = (lightVector).GetDotProduct(_polygons[i].GetNormal());
			difRGB = RGB(GetRValue(tempRGB) * diffDot, GetGValue(tempRGB) * diffDot, GetBValue(tempRGB) * diffDot);

			//Specular
			tempRGB = RGB(GetRValue(clr), GetGValue(clr), GetBValue(clr));
			tempRGB = RGB((GetRValue(tempRGB) * ksR), (GetGValue(tempRGB) * ksG), (GetBValue(tempRGB) * ksB));
			specDot = (lightVector).GetDotProduct(_polygons[i].GetNormal());
			specDot = pow(specDot, 4);
			specRGB = RGB(GetRValue(tempRGB) * specDot, GetGValue(tempRGB) * specDot, GetBValue(tempRGB) * specDot);

			// Total
			tempRGB = RGB((GetRValue(ambRGB) + GetRValue(difRGB) + GetRValue(specRGB)), 
					      (GetGValue(ambRGB) + GetGValue(difRGB) + GetGValue(specRGB)),
						  (GetBValue(ambRGB) + GetBValue(difRGB) + GetBValue(specRGB)));
			totalRGB = RGB((GetRValue(totalRGB) + GetRValue(tempRGB)), (GetGValue(totalRGB) + GetGValue(tempRGB)), (GetBValue(totalRGB) + GetBValue(tempRGB)));
			
			finalRGB = ClampColor(totalRGB);
			_polygons[i].SetColor(finalRGB);
		}
	}
}


// Calculates Point Lighting
void Model::CalculatePointLighting(Vertex lightPos, float cX, float cY, float cZ, COLORREF clr)
{
	Vertex ver0, ver1, ver2;

	Vector3D vectorToLightSource;
	Vector3D vectorToCamera;
	Vector3D halfVector;

	COLORREF ambRGB, diffRGB, specRGB;
	COLORREF tempRGB;
	COLORREF totalRGB;
	COLORREF finalRGB;
	
	float difDot, specDot;

	Vertex lightPosition;
	lightPosition = lightPos;

	Vertex cameraPosition;
	cameraPosition = Vertex(cX, cY, cZ);

	// For every Polygon
	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		ver0 = _vertices[_polygons[i].GetIndex(0)];
		ver1 = _vertices[_polygons[i].GetIndex(1)];
		ver2 = _vertices[_polygons[i].GetIndex(2)];

		_polygons[i].SetNormal(CalculateNormal(ver0, ver1, ver2));

		totalRGB = RGB(0, 0, 0);
		
		for (int j = 0; j < 1; j++)		// since i only have 1 light
		{
			// Ambient
			ambRGB = RGB(GetRValue(clr) * kaR, GetGValue(clr) * kaG, GetBValue(clr) * kaB);
			tempRGB = RGB(GetRValue(clr), GetGValue(clr), GetBValue(clr));

			// Diffuse
			tempRGB = RGB((GetRValue(tempRGB) * kdR), (GetGValue(tempRGB) * kdG), (GetBValue(tempRGB) * kdB));
			vectorToLightSource = lightPosition - ver0;
			vectorToCamera = cameraPosition - ver0;
			halfVector = vectorToLightSource + vectorToCamera;
			halfVector.Normalise();
			vectorToLightSource.Normalise();
			difDot = (vectorToLightSource).GetDotProduct(_polygons[i].GetNormal());
			diffRGB = RGB(GetRValue(tempRGB) * difDot, GetGValue(tempRGB) * difDot, GetBValue(tempRGB) * difDot);

			// Specular
			tempRGB = RGB(GetRValue(clr), GetGValue(clr), GetBValue(clr));
			tempRGB = RGB((GetRValue(tempRGB) * ksR), (GetGValue(tempRGB) * ksG), (GetBValue(tempRGB) * ksB));
			specDot = (halfVector).GetDotProduct(_polygons[i].GetNormal());
			specDot = specDot * specDot*specDot*specDot;
			specRGB = RGB(GetRValue(tempRGB) * specDot, GetGValue(tempRGB) * specDot, GetBValue(tempRGB) * specDot);

			// Total
			tempRGB = RGB((GetRValue(ambRGB) + GetRValue(diffRGB) + GetRValue(specRGB)), 
				          (GetGValue(ambRGB) + GetGValue(diffRGB) + GetRValue(specRGB)),
				          (GetBValue(ambRGB) + GetBValue(diffRGB) + GetRValue(specRGB)));
			totalRGB = RGB((GetRValue(totalRGB) + GetRValue(tempRGB)), (GetGValue(totalRGB) + GetGValue(tempRGB)), (GetBValue(totalRGB) + GetBValue(tempRGB)));

			finalRGB = ClampColor(totalRGB);
			_polygons[i].SetColor(finalRGB);
		}
	}
}


// Calculates the Normal for each Polygon
Vector3D Model::CalculateNormal(Vertex ver0, Vertex ver1, Vertex ver2)
{		
	Vector3D vectorA, vectorB, vectorN;
	
	vectorA = Vector3D(ver0 - ver1);
	vectorB = Vector3D(ver0 - ver2);

	vectorA.Normalise();
	vectorB.Normalise();
	vectorN = vectorB.GetCrossProduct(vectorA);
	vectorN.Normalise();
		
	return vectorN;
}


// Clamps the color if needed, to create the final color.
COLORREF Model::ClampColor(COLORREF color)
{
	COLORREF result;
	BYTE r, g, b;

	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if (r < 0)
		r = 0;
	else if (r > 255)
		r = 255;

	if (g < 0)
		g = 0;
	else if (g > 255)
		g = 255;

	if (b < 0)
		b = 0;
	else if (b > 255)
		b = 255;

	result = RGB(r, g, b);

	return result;
}