// MWinLib.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Demo.h"

// This would normally be where you put your library
#include "mwinlib/mwinlib.h"

#include <algorithm>

#include <shobjidl.h>

WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

MMessageMap messageMap;
DEF_WNDPROC(WndProc, messageMap);

MMessageMap aboutMessageMap;
DEF_DLGPROC(AboutWndProc, aboutMessageMap);

void RegisterHandlers();
void AddControls();

HINSTANCE hInst;
MWindow window;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	hInst = hInstance;

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MWINLIB, szWindowClass, MAX_LOADSTRING);

	MWindowClass windowClass;
	windowClass.Generate(hInstance, CS_HREDRAW | CS_VREDRAW, szWindowClass, WndProc);
	// Extra details
	windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MWINLIB));
	windowClass.lpszMenuName = MAKEINTRESOURCEW(IDC_MWINLIB);
	windowClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

	windowClass.Register();
	
	RegisterHandlers();

	window.Create(hInstance, windowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0);

	if (!window.InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	AddControls();

	

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MWINLIB));

	return RunMessageLoop(hAccelTable);
}

void AddControls()
{
	MStatic text(&window);
	text.SetBackgroundColor(RGB(255, 255, 255), &messageMap);
	text.Create(L"Sample Text", 0, 0, 200, 200);

	MButton btn(&window);
	btn.Create(L"Button", 100, 0, 150, 20);
}

void RegisterHandlers()
{
	// Main Window
	REGISTER_MSG(messageMap, WM_COMMAND,
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutWndProc);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
			}

			return 0;
		});
	REGISTER_MSG(messageMap, WM_DESTROY, { PostQuitMessage(0); return 0; });

	// About
	REGISTER_MSG(aboutMessageMap, WM_INITDIALOG, { return (INT_PTR)TRUE; });
	REGISTER_MSG(aboutMessageMap, WM_COMMAND,
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hWnd, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			return (INT_PTR)TRUE;
		});
}