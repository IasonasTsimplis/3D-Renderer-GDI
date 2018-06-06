#pragma once
#include <windows.h>
#include "Resource.h"
#include <exception>

using namespace std;

class Framework
{
public:
	Framework();
	virtual ~Framework();

	int Run(HINSTANCE hInstance, int nCmdShow);

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual unsigned int GetInitialWindowHeight() const;
	virtual unsigned int GetInitialWindowWidth() const;
	virtual bool Initialise();
	virtual void Shutdown();
	void CreateControls(HWND hWnd);

private:
	HINSTANCE		_hInstance;
	HWND			_hWnd;
	HWND			h1, h2, h3, h4, h5, h6;
	HMENU hMenu;

	// Used in timing loop
	double			_timeSpan;
	bool InitialiseMainWindow(int nCmdShow);
	int MainLoop();
};

