#include <windows.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include "resource.h"

// MWinLib
//#include "mwinlib/mwinlib.h"

MMessageMap messageMap;
DEF_WNDPROC(WndProc, messageMap)

MMessageMap aboutMessageMap;
DEF_DLGPROC(AboutWndProc, aboutMessageMap);

void RegisterHandlers();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	MWindowClass windowClass;
	windowClass.Generate(hInstance, CS_HREDRAW | CS_VREDRAW, L"MyWindow", WndProc);
	windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TUTORIAL));
	windowClass.lpszMenuName = MAKEINTRESOURCEW(IDC_TUTORIAL);
	windowClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	windowClass.Register();

	RegisterHandlers();

	MWindow window;
	window.Create(hInstance, windowClass, L"My Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0);

	if (!window.InitInstance(hInstance, nCmdShow))
		return FALSE;

	return RunMessageLoop();
}

void RegisterHandlers()
{
	REGISTER_MSG(messageMap, WM_COMMAND,
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutWndProc);
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