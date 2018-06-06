#include "Framework.h"
#include "Rasteriser.h"

#define DEFAULT_FRAMERATE	30
#define DEFAULT_WIDTH		1000
#define DEFAULT_HEIGHT		800

// Reference to ourselves - primarily used to access the message handler correctly
// This is initialised in the constructor
Framework *	_thisFramework = NULL;
Rasteriser rasteriser;
RECT client_rect;
long width;
long height;


// Forward declaration of our window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_	   HINSTANCE hInstance,
				  	  _In_opt_ HINSTANCE hPrevInstance,
					  _In_	   LPWSTR    lpCmdLine,
					  _In_	   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	if (_thisFramework)
	{
		return _thisFramework->Run(hInstance, nCmdShow);
	}
	return -1;
}

Framework::Framework()
{
	_thisFramework = this;
}

Framework::~Framework()
{
}

int Framework::Run(HINSTANCE hInstance, int nCmdShow)
{
	int returnValue;

	_hInstance = hInstance;
	if (!InitialiseMainWindow(nCmdShow))
	{
		return -1;
	}
	if (!Initialise())
	{
		return -1;
	}
	returnValue = MainLoop();
	Shutdown();
	return returnValue;
}

// Main program loop.  

int Framework::MainLoop()
{
	MSG msg;
	HACCEL hAccelTable = LoadAccelerators(_hInstance, MAKEINTRESOURCE(IDC_RASTERISER));
	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
	}
	return (int)msg.wParam;
}

// The initial required height of the window.  This can be overridden if a different initial height is required

unsigned int Framework::GetInitialWindowHeight() const
{
	return DEFAULT_HEIGHT;
}

// The initial required width of the window.  This can be overridden if a different initial width is required

unsigned int Framework::GetInitialWindowWidth() const
{
	return DEFAULT_WIDTH;
}


// Return false if the application cannot be initialised.
bool Framework::Initialise()
{	
	bool result;
	result = rasteriser.InitialiseModel(_hWnd);
	InvalidateRect(_hWnd, NULL, TRUE);
	return result;
	
}

// Perform any application shutdown that is needed
void Framework::Shutdown()
{
}

// Create Buttons on the main window
void Framework::CreateControls(HWND hWnd)
{
	h1 = CreateWindow(L"static", L" Projection: Orthographic", WS_CHILD | WS_VISIBLE, 10, 10, 173, 20, hWnd, (HMENU)50, nullptr, nullptr);
	h2 = CreateWindow(L"static", L" Backface Culling: OFF", WS_CHILD | WS_VISIBLE, 10, 30, 173, 20, hWnd, (HMENU)51, nullptr, nullptr);
	h3 = CreateWindow(L"static", L" Polygon Sorting: OFF", WS_CHILD | WS_VISIBLE, 10, 50, 173, 20, hWnd, (HMENU)52, nullptr, nullptr);
	h4 = CreateWindow(L"static", L" Flat Shading: OFF", WS_CHILD | WS_VISIBLE, 10, 70, 173, 20, hWnd, (HMENU)53, nullptr, nullptr);
	h6 = CreateWindow(L"static", L" Lighting: Point / Direct", WS_CHILD | WS_VISIBLE, 10, 90, 173, 20, hWnd, (HMENU)55, nullptr, nullptr);
	h5 = CreateWindow(L"static", L" Render Mode: WireFrame", WS_CHILD | WS_VISIBLE, 10, 110 , 173, 20, hWnd, (HMENU)54, nullptr, nullptr);

	CreateWindow(L"static", L"Rotate X", WS_CHILD | WS_VISIBLE, 10, 135, 60, 20, hWnd, NULL, nullptr, nullptr);
	CreateWindow(L"static", L"Rotate Y", WS_CHILD | WS_VISIBLE, 10, 160, 60, 20, hWnd, NULL, nullptr, nullptr);
	CreateWindow(L"static", L"Rotate Z", WS_CHILD | WS_VISIBLE, 10, 185, 60, 20, hWnd, NULL, nullptr, nullptr);

	CreateWindow(L"static", L"Translate X", WS_CHILD | WS_VISIBLE, 10, 235, 80, 20, hWnd, NULL, nullptr, nullptr);
	CreateWindow(L"static", L"Translate Y", WS_CHILD | WS_VISIBLE, 10, 260, 80, 20, hWnd, NULL, nullptr, nullptr);
	CreateWindow(L"static", L"Translate Z", WS_CHILD | WS_VISIBLE, 10, 285, 80, 20, hWnd, NULL, nullptr, nullptr);

	CreateWindow(L"static", L"Scale X", WS_CHILD | WS_VISIBLE, 10, 335, 55, 20, hWnd, NULL, nullptr, nullptr);
	CreateWindow(L"static", L"Scale Y", WS_CHILD | WS_VISIBLE, 10, 360, 55, 20, hWnd, NULL, nullptr, nullptr);
	CreateWindow(L"static", L"Scale Z", WS_CHILD | WS_VISIBLE, 10, 385, 55, 20, hWnd, NULL, nullptr, nullptr);

	CreateWindow(L"static", L"Camera X", WS_CHILD | WS_VISIBLE, 10, 435, 67, 20, hWnd, NULL, nullptr, nullptr);
	CreateWindow(L"static", L"Camera Y", WS_CHILD | WS_VISIBLE, 10, 460, 67, 20, hWnd, NULL, nullptr, nullptr);
	CreateWindow(L"static", L"Camera Z", WS_CHILD | WS_VISIBLE, 10, 485, 67, 20, hWnd, NULL, nullptr, nullptr);

	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 100, 135, 20, 20, hWnd, (HMENU)1, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 75, 135, 20, 20, hWnd, (HMENU)2, nullptr, nullptr);
	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 100, 160, 20, 20, hWnd, (HMENU)3, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 75, 160, 20, 20, hWnd, (HMENU)4, nullptr, nullptr);
	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 100, 185, 20, 20, hWnd, (HMENU)5, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 75, 185, 20, 20, hWnd, (HMENU)6, nullptr, nullptr);

	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 120, 235, 20, 20, hWnd, (HMENU)7, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 95, 235, 20, 20, hWnd, (HMENU)8, nullptr, nullptr);
	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 120, 260, 20, 20, hWnd, (HMENU)9, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 95, 260, 20, 20, hWnd, (HMENU)10, nullptr, nullptr);
	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 120, 285, 20, 20, hWnd, (HMENU)11, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 95, 285, 20, 20, hWnd, (HMENU)12, nullptr, nullptr);

	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 95, 335, 20, 20, hWnd, (HMENU)13, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 70, 335, 20, 20, hWnd, (HMENU)14, nullptr, nullptr);
	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 95, 360, 20, 20, hWnd, (HMENU)15, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 70, 360, 20, 20, hWnd, (HMENU)16, nullptr, nullptr);
	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 95, 385, 20, 20, hWnd, (HMENU)17, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 70, 385, 20, 20, hWnd, (HMENU)18, nullptr, nullptr);

	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 107, 435, 20, 20, hWnd, (HMENU)19, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 82, 435, 20, 20, hWnd, (HMENU)20, nullptr, nullptr);
	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 107, 460, 20, 20, hWnd, (HMENU)21, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 82, 460, 20, 20, hWnd, (HMENU)22, nullptr, nullptr);
	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 107, 485, 20, 20, hWnd, (HMENU)23, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 82, 485, 20, 20, hWnd, (HMENU)24, nullptr, nullptr);

	CreateWindow(L"static", L"Focus Length", WS_CHILD | WS_VISIBLE, 10, 535, 90, 20, hWnd, NULL, nullptr, nullptr);
	CreateWindow(L"Button", L"+", WS_CHILD | WS_VISIBLE, 130, 535, 20, 20, hWnd, (HMENU)26, nullptr, nullptr);
	CreateWindow(L"Button", L"--", WS_CHILD | WS_VISIBLE, 105, 535, 20, 20, hWnd, (HMENU)27, nullptr, nullptr);
	
	CreateWindow(L"Button", L"Perspective ON/OFF", WS_CHILD | WS_VISIBLE, 10, 585, 145, 20, hWnd, (HMENU)25, nullptr, nullptr);
	CreateWindow(L"Button", L"Backface Culling ON/OFF", WS_CHILD | WS_VISIBLE, 10, 610, 170, 20, hWnd, (HMENU)28, nullptr, nullptr);
	CreateWindow(L"Button", L"Polygon Sorting ON/OFF", WS_CHILD | WS_VISIBLE, 10, 635, 168, 20, hWnd, (HMENU)30, nullptr, nullptr);
	CreateWindow(L"Button", L"Solid / WireFrame", WS_CHILD | WS_VISIBLE, 10, 660, 124, 20, hWnd, (HMENU)29, nullptr, nullptr);
	CreateWindow(L"Button", L"Flat Shading ON/OFF", WS_CHILD | WS_VISIBLE, 10, 685, 146, 20, hWnd, (HMENU)31, nullptr, nullptr);

	CreateWindow(L"Button", L"Load Cube Model", WS_CHILD | WS_VISIBLE, 370, 10, 125, 20, hWnd, (HMENU)32, nullptr, nullptr);
	CreateWindow(L"Button", L"Load Ball Model", WS_CHILD | WS_VISIBLE, 500, 10, 120, 20, hWnd, (HMENU)33, nullptr, nullptr);
	CreateWindow(L"Button", L" Point / Directional", WS_CHILD | WS_VISIBLE, 10, 710, 126, 20, hWnd, (HMENU)34, nullptr, nullptr);
}

// Register the  window class, create the window.
bool Framework::InitialiseMainWindow(int nCmdShow)
{
	#define MAX_LOADSTRING 100

	WCHAR windowTitle[MAX_LOADSTRING];          
	WCHAR windowClass[MAX_LOADSTRING];            
	
	LoadStringW(_hInstance, IDS_APP_TITLE, windowTitle, MAX_LOADSTRING);
	LoadStringW(_hInstance, IDC_RASTERISER, windowClass, MAX_LOADSTRING);

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = NULL;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_RASTERISER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&wcex))
	{
		return false;
	}

	_hWnd = CreateWindowW(windowClass, 
						  windowTitle, 
					      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
						  CW_USEDEFAULT, 0, GetInitialWindowWidth(), GetInitialWindowHeight(),  
					      nullptr, nullptr, _hInstance, nullptr);

	CreateControls(_hWnd);
	SetWindowText(_hWnd, L"3D Renderer  -  By Iasonas Tsimplis");
	
	if (!_hWnd)
	{
		return false;
	}
	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);


	return true;
}

// The WndProc for the current window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_thisFramework != NULL)
	{
		// If framework is started, then we can call our own message proc
		return _thisFramework->MsgProc(hWnd, message, wParam, lParam);
	}
	else
	{
		// otherwise, we just pass control to the default message proc
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

// Our main WndProc
LRESULT Framework::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;

			// Back Buffer implemented for no flickering.
			HDC Memhdc;
			HDC hdc;
			HBITMAP Membitmap;
			hdc = BeginPaint(hWnd, &ps);
			Memhdc = CreateCompatibleDC(hdc);
			Membitmap = CreateCompatibleBitmap(hdc, width, height);
			SelectObject(Memhdc, Membitmap);

				rasteriser.RenderModel(Memhdc);

			BitBlt(hdc, 0, 0, width, height, Memhdc, 0, 0, SRCCOPY);
			DeleteObject(Membitmap);
			DeleteDC(Memhdc);
			DeleteDC(hdc);
			EndPaint(hWnd, &ps);
		}
		break;
		
		case WM_SIZE:
		{
			GetClientRect(hWnd, &client_rect);
			width = client_rect.right - client_rect.left;
			height = client_rect.bottom - client_rect.top;
			rasteriser.UpdateModel(0);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

		// On keyboard keys pressing - Continiuous smooth movement
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_RIGHT:	
					rasteriser.UpdateModel(3);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case VK_LEFT:
					rasteriser.UpdateModel(4);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case VK_UP:
					rasteriser.UpdateModel(2);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case VK_DOWN:
					rasteriser.UpdateModel(1);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				default:
					break;
			}
		}
		break;

		// On On-Screen-Button Clicks - Per click movement
		case WM_COMMAND:
		{
			switch (wParam)
			{
				case 1:
					rasteriser.UpdateModel(1);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 2:
					rasteriser.UpdateModel(2);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 3:
					rasteriser.UpdateModel(3);
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				case 4:
					rasteriser.UpdateModel(4);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 5:
					rasteriser.UpdateModel(5);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 6:
					rasteriser.UpdateModel(6);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 7:
					rasteriser.UpdateModel(7);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 8:
					rasteriser.UpdateModel(8);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 9:
					rasteriser.UpdateModel(9);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 10:
					rasteriser.UpdateModel(10);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 11:
					rasteriser.UpdateModel(11);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 12:
					rasteriser.UpdateModel(12);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 13:
					rasteriser.UpdateModel(13);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 14:
					rasteriser.UpdateModel(14);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 15:
					rasteriser.UpdateModel(15);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 16:
					rasteriser.UpdateModel(16);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 17:
					rasteriser.UpdateModel(17);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 18:
					rasteriser.UpdateModel(18);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 19:
					rasteriser.UpdateModel(19);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 20:
					rasteriser.UpdateModel(20);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 21:
					rasteriser.UpdateModel(21);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 22:
					rasteriser.UpdateModel(22);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 23:
					rasteriser.UpdateModel(23);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 24:
					rasteriser.UpdateModel(24);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 25:
					rasteriser.UpdateModel(25);
					SetWindowText(h1, rasteriser.GetTextBox());
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 26:
					rasteriser.UpdateModel(26);
					SetWindowText(h1, rasteriser.GetTextBox());
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 27:
					rasteriser.UpdateModel(27);
					SetWindowText(h1, rasteriser.GetTextBox());
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 28:
					rasteriser.UpdateModel(28);
					SetWindowText(h2, rasteriser.GetTextBox());
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 29:
					rasteriser.UpdateModel(29);
					SetWindowText(h5, rasteriser.GetTextBox());
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				case 30:
					rasteriser.UpdateModel(30);
					SetWindowText(h3, rasteriser.GetTextBox());
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 31:
					rasteriser.UpdateModel(31);
					SetWindowText(h4, rasteriser.GetTextBox());
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				case 32:
					rasteriser.UpdateModel(32);
					SetWindowText(h1, L" Projection: Orthographic");
					SetWindowText(h2, L" Backface Culling: OFF");
					SetWindowText(h3, L" Polygon Sorting: OFF");
					SetWindowText(h4, L" Flat Shading: OFF");
					SetWindowText(h5, L" Render Mode: WireFrame");
					SetWindowText(h6, L" Lighting: Point / Direct");
					InvalidateRect(hWnd, NULL, TRUE);
					break;
			
				case 33:
					rasteriser.UpdateModel(33);
					SetWindowText(h1, L" Projection: Orthographic");
					SetWindowText(h2, L" Backface Culling: OFF");
					SetWindowText(h3, L" Polygon Sorting: OFF");
					SetWindowText(h4, L" Flat Shading: OFF");
					SetWindowText(h5, L" Render Mode: WireFrame");
					SetWindowText(h6, L" Lighting: Point / Direct");
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				
				case 34:
					rasteriser.UpdateModel(34);
					InvalidateRect(hWnd, NULL, TRUE);
					break;

				default:
					break;
			}
		}

		// Return focus to the main window, so that keyboard keys can function.
		::SetFocus(hWnd);
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
