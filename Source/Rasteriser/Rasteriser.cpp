#include "Rasteriser.h"
#include "MD2Loader.h"
#include <vector>
#include <ctime>

// Loads & Initialises the Model
bool Rasteriser::InitialiseModel(HWND hWnd)
{
	_handler = hWnd;
	printf("\n Initialise: Called !!\n");
	if (!MD2Loader::LoadModel(_filePath, model, &Model::AddPolygon, &Model::AddVertex))
	{
		return false;
	}

	srand((unsigned)time(NULL));

	for (size_t i = 0; i < model.GetPolygonCount(); i++)
	{															// OR FOR SINGLE COLOR: 
		model.GetPolygons()[i].SetColor(getRandomColor());		// model.GetPolygons()[i].SetColor(RGB(255, 0, 50));
	}
	
	GetClientRect(hWnd, &_rect);
	_height = _rect.bottom - _rect.top;
	_width = _rect.right - _rect.left;

	directLight = DirectionalLighting(Vertex(0, 0, -100), RGB(255, 255, 255));
	pointLight = PointLighting(Vertex(0, 0, -100), RGB(255, 255, 255));
	pointLight;
	pointLightEnabled = false;
	flatShadingEnabled = false;
	perspective = false;
	bfcullingEnabled = false;
	sortingEnabled = false;
	solidEnabled = false;
	focus = 60.0f;
	camX = 0.0f;
	camY = 0.0f;
	camZ = 1500.0f;

	modelTransformation = Matrix::UnitMatrix();
	if (_filePath == "ball.md2")
		modelTransformation = Matrix::ScalingMatrix(6.0f, 6.0f, 6.0f) * modelTransformation;
	else if (_filePath == "eva.md2")
		modelTransformation = Matrix::ScalingMatrix(2.0f, 2.0f, 2.0f) * modelTransformation;
	cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
	perspectiveTransformation = Matrix::UnitMatrix();
	viewportTransformation = Matrix::ViewportMatrix((_width / 2), (_height / 2), 10, 10, 0);
	tempmodel = model;

	return true;
}


// Applies any tranformations & Renders the Model
void Rasteriser::RenderModel(HDC hdc)
{
	model = tempmodel;
	model.ApplyTransformation(modelTransformation);
	
	if (sortingEnabled == true)
		model.Sort();
	
	if (bfcullingEnabled == true)
		model.CalculateBackfaces(camX, camY, camZ);
	
	if (flatShadingEnabled == true)
	{
		if (pointLightEnabled == true)
			model.CalculatePointLighting(pointLight.GetPosition(), -camX, -camY, -100, pointLight.GetIntensity());
		else
			model.CalculateDirectionalLighting(directLight.GetPosition(), directLight.GetIntensity());
	}
	
	model.ApplyTransformation(cameraTransformation);
	model.ApplyTransformation(perspectiveTransformation);
	model.DehomogeniseCoords();
	model.ApplyTransformation(viewportTransformation);

	if (solidEnabled == true)
		DrawSolidModel(hdc);
	else
		DrawWireFrameModel(hdc);
}


// Draws Model's WireFrame
void Rasteriser::DrawWireFrameModel(HDC hdc)
{
	std::vector<Polygon3D> polygons = model.GetPolygons();
	std::vector<Vertex> vertices = model.GetVertices();
	
	Vertex ver0, ver1, ver2;

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HGDIOBJ oldPen = SelectObject(hdc, pen);

	for (size_t i = 0; i < model.GetPolygonCount(); i++)
	{
		if (polygons[i].GetBackface() == false)
		{
			ver0 = vertices[polygons[i].GetIndex(0)];
			ver1 = vertices[polygons[i].GetIndex(1)];
			ver2 = vertices[polygons[i].GetIndex(2)];

			MoveToEx(hdc, (int)ver0.GetX(), (int)ver0.GetY(), NULL);
			LineTo(hdc, (int)ver1.GetX(), (int)ver1.GetY());
			LineTo(hdc, (int)ver2.GetX(), (int)ver2.GetY());
			LineTo(hdc, (int)ver0.GetX(), (int)ver0.GetY());
		}	
	}
	SelectObject(hdc, oldPen);
	DeleteObject(pen);
}


// Draws Solid Model with Color
void Rasteriser::DrawSolidModel(HDC hdc)
{
	std::vector<Polygon3D> polygons = model.GetPolygons();
	std::vector<Vertex> vertices = model.GetVertices();
	POINT p[3];
	Vertex ver0, ver1, ver2;

	for (size_t i = 0; i < model.GetPolygonCount(); i++)
	{
		if (polygons[i].GetBackface() == false)
		{
			HBRUSH brush = CreateSolidBrush(polygons[i].GetColor());
			HGDIOBJ oldBrush = SelectObject(hdc, brush);
			HPEN pen = CreatePen(PS_SOLID, 1, polygons[i].GetColor());
			HGDIOBJ oldPen = SelectObject(hdc, pen);

			ver0 = vertices[polygons[i].GetIndex(0)];
			ver1 = vertices[polygons[i].GetIndex(1)];
			ver2 = vertices[polygons[i].GetIndex(2)];

			p[0].x = (long)ver0.GetX();
			p[0].y = (long)ver0.GetY();
			p[1].x = (long)ver1.GetX();
			p[1].y = (long)ver1.GetY();
			p[2].x = (long)ver2.GetX();
			p[2].y = (long)ver2.GetY();

			Polygon(hdc, p, 3);

			SelectObject(hdc, oldBrush);
			DeleteObject(brush);
			SelectObject(hdc, oldPen);
			DeleteObject(pen);
		}
	}
}


// Updates the Model
void Rasteriser::UpdateModel(int action)
{
	switch (action)
	{

		// Rotation(X) (+)
	case 1:
		modelTransformation = Matrix::XRotationMatrix(1.0f) * modelTransformation;
		break;

		// Rotation(X) (-)
	case 2:
		modelTransformation = Matrix::XRotationMatrix(-1.0f) * modelTransformation;
		break;

		// Rotation(Y) (+)
	case 3:
		modelTransformation = Matrix::YRotationMatrix(1.0f) * modelTransformation;
		break;

		// Rotation(Y) (-)
	case 4:
		modelTransformation = Matrix::YRotationMatrix(-1.0f) * modelTransformation;
		break;

		// Rotation(Z) (+)
	case 5:
		modelTransformation = Matrix::ZRotationMatrix(1.0f) * modelTransformation;
		break;

		// Rotation(Z) (-) 
	case 6:
		modelTransformation = Matrix::ZRotationMatrix(-1.0f) * modelTransformation;
		break;

		// Translation(X) (+)
	case 7:
		modelTransformation = Matrix::TranslationMatrix(2.0f, 0.0f, 0.0f) * modelTransformation;
		break;

		// Translation(X) (-)
	case 8:
		modelTransformation = Matrix::TranslationMatrix(-2.0f, 0.0f, 0.0f) * modelTransformation;
		break;

		// Translation(Y) (+)
	case 9:
		modelTransformation = Matrix::TranslationMatrix(0.0f, 2.0f, 0.0f) * modelTransformation;
		break;

		// Translation(Y) (-)
	case 10:
		modelTransformation = Matrix::TranslationMatrix(0.0f, -2.0f, 0.0f) * modelTransformation;
		break;

		// Translation(Z) (+)
	case 11:
		modelTransformation = Matrix::TranslationMatrix(0.0f, 0.0f, 2.0f) * modelTransformation;
		break;

		// Translation(Z) (-)
	case 12:
		modelTransformation = Matrix::TranslationMatrix(0.0f, 0.0f, -2.0f) * modelTransformation;
		break;

		// Scaling(X) (+)
	case 13:
		modelTransformation = Matrix::ScalingMatrix(2.0f, 1.0f, 1.0f) * modelTransformation;
		break;

		// Scaling(X) (-)
	case 14:
		modelTransformation = Matrix::ScalingMatrix(0.5f, 1.0f, 1.0f) * modelTransformation;
		break;

		// Scaling(Y) (+)
	case 15:
		modelTransformation = Matrix::ScalingMatrix(1.0f, 2.0f, 1.0f) * modelTransformation;
		break;
		// Scaling(Y) (-)
	case 16:
		modelTransformation = Matrix::ScalingMatrix(1.0f, 0.5f, 1.0f) * modelTransformation;
		break;

		// Scaling(Z) (+)
	case 17:
		modelTransformation = Matrix::ScalingMatrix(1.0f, 1.0f, 2.0f) * modelTransformation;
		break;

		// Scaling(Z) (-)
	case 18:
		modelTransformation = Matrix::ScalingMatrix(1.0f, 1.0f, 0.5f) * modelTransformation;
		break;

		// Camera(X) (+)
	case 19:
		camX = camX + 10.0f;
		cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
		break;

		// Camera(X) (-)
	case 20:
		camX = camX - 10.0f;
		cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
		break;

		// Camera(Y) (+)
	case 21:
		camY = camY + 10.0f;
		cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
		break;

		// Camera(Y) (-)
	case 22:
		camY = camY - 10.0f;
		cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
		break;

		// Camera(Z) (+)
	case 23:
		camZ = camZ + 10.0f;
		cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
		break;

		// Camera(Z) (-)
	case 24:
		camZ = camZ - 10.0f;
		cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
		break;

		// Perspective (ON/OFF)
	case 25:
		if (perspective == false)
		{
			camZ = 80.0f;
			cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
			perspectiveTransformation = Matrix::PerspectiveMatrix(-focus, 1.0f);
			perspective = true;
			SetTextBox(L" Projection: Perspective");
		}
		else
		{
			camZ = 1500.0f;
			cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
			perspectiveTransformation = Matrix::UnitMatrix();
			perspective = false;
			SetTextBox(L" Projection: Orthographic");
		}
		break;

		// Focus Length (+)
	case 26:
		if (perspective == false)
		{
			camZ = 80.0f;
			cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
			perspective = true;
		}
		focus = focus + 10.0f;
		perspectiveTransformation = Matrix::PerspectiveMatrix(-focus, 1.0f);

		SetTextBox(L" Projection: Perspective");
		break;

		// Focus Length (-)
	case 27:
		if (perspective == false)
		{
			camZ = 80.0f;
			cameraTransformation = Matrix::CameraMatrix(camX, camY, camZ, 0, 0, 0);
			perspective = true;
		}
		focus = focus - 10.0f;
		perspectiveTransformation = Matrix::PerspectiveMatrix(-focus, 1.0f);

		SetTextBox(L" Projection: Perspective");
		break;

		// Backface Culling (ON/OFF)
	case 28:
		if (bfcullingEnabled == false)
		{
			bfcullingEnabled = true;
			SetTextBox(L" Backface Culling: ON");
		}
		else
		{
			bfcullingEnabled = false;
			SetTextBox(L" Backface Culling: OFF");
		}
		break;

		// Solid Model / WireFrame Model (Toggle)
	case 29:
		if (solidEnabled == false)
		{
			solidEnabled = true;
			SetTextBox(L" Render Mode: Solid");
		}
		else
		{
			solidEnabled = false;
			SetTextBox(L" Render Mode: WireFrame");
		}
		break;

		// Backface Culling (ON/OFF)
	case 30:
		if (sortingEnabled == false)
		{
			sortingEnabled = true;
			SetTextBox(L" Polygon Sorting: ON");
		}
		else
		{
			sortingEnabled = false;
			SetTextBox(L" Polygon Sorting: OFF");
		}
		break;

		// Flat Shading ON / OFF
	case 31:
		if (flatShadingEnabled == false)
		{
			flatShadingEnabled = true;
			SetTextBox(L" Flat Shading: ON");
		}
		else
		{
			flatShadingEnabled = false;
			SetTextBox(L" Flat Shading: OFF");
		}
		break;

	case 32:
		_filePath = "cube.md2";
		model.GetPolygons().clear();
		model.GetVertices().clear();
		InitialiseModel(_handler);
		break;

	case 33:
		_filePath = "ball.md2";
		model.GetPolygons().clear();
		model.GetVertices().clear();
		InitialiseModel(_handler);
		break;
	
	case 34:
		if (pointLightEnabled == false)
			pointLightEnabled = true;
		else
			pointLightEnabled = false;
		break;

	default:
		GetClientRect(_handler, &_rect);
		_height = _rect.bottom - _rect.top;
		_width = _rect.right - _rect.left;
		viewportTransformation = Matrix::ViewportMatrix((_width / 2), (_height / 2), 10, 10, 0);
		break;
	}
}


// Returns a random color.
COLORREF Rasteriser::getRandomColor()
{
		return RGB(rand() % 255, rand() % 255, rand() % 255);
}


// Sets the name of the textbox
void Rasteriser::SetTextBox(LPCWSTR value)
{
	_textName = value;

}


// Returns the name for the textbox
LPCWSTR Rasteriser::GetTextBox()
{
	return _textName;
}


