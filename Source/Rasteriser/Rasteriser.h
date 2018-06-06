#pragma once
#include "Framework.h"
#include "DirectionalLighting.h"
#include "PointLighting.h"
#include "Model.h"
#include "Matrix.h"
#include <vector>


class Rasteriser : public Framework
{
private:
	const char* _filePath = "cube.md2";
	HWND _handler;
	RECT _rect;
	int _width;
	int _height;
	LPCWSTR _textName;
	
	DirectionalLighting directLight;
	PointLighting pointLight;
	Model model, tempmodel;
	Matrix modelTransformation;
	Matrix viewportTransformation;
	Matrix cameraTransformation;
	Matrix perspectiveTransformation;
	Matrix matrix;
	Matrix m;

	bool pointLightEnabled;
	bool flatShadingEnabled;
	bool perspective;
	bool sortingEnabled;
	bool bfcullingEnabled;
	bool solidEnabled;
	float focus;
	float camX;
	float camY;
	float camZ;

public:
	bool InitialiseModel(HWND hWnd);
	void UpdateModel(int action);
	void RenderModel(HDC hdc);
	void DrawWireFrameModel(HDC hdc);
	void DrawSolidModel(HDC hdc);
	COLORREF getRandomColor();
	void SetTextBox(LPCWSTR value);
	LPCWSTR GetTextBox();
};

