#include "mwindow.h"
#include <minwindef.h>
#include "winpredef.h"

//  ==== MWindowClass ==== //

void MWindowClass::Generate(HINSTANCE hInstance, int style, const TCHAR* className, WNDPROC wndProc)
{
	// Clear struct
	*this = {};

	this->cbSize = sizeof(WNDCLASSEX);

	this->style = style;
	this->lpfnWndProc = wndProc;
	this->cbClsExtra = 0;
	this->cbWndExtra = 0;
	this->hInstance = hInstance;
	this->hCursor = LoadCursor(nullptr, IDC_ARROW);
	this->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	this->lpszClassName = className;
}

void MWindowClass::Register()
{
	RegisterClassExW(this);
}

map<HWND, MWindowBase*> windowMap;

MWindowBase* GetWindowFromHWnd(HWND hWnd)
{
	auto it = windowMap.find(hWnd);
	if (it != windowMap.end())
	{
		return it->second;
	}
	return NULL;
}

// ==== MWindowBase ==== //
void MWindowBase::CreateEx(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int width, int height, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	hWnd = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, width, height, hWndParent, hMenu, hInstance, lpParam);
	windowMap[hWnd] = this;
}

void MWindowBase::Create(LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int width, int height, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	CreateEx(0L, lpClassName, lpWindowName, dwStyle, x, y, width, height, hWndParent, hMenu, hInstance, lpParam);
}

HWND MWindowBase::GetHWnd()
{
	return hWnd;
}

// ==== MWindow ==== //
BOOL MWindow::InitInstance(HINSTANCE hInst, int nCmdShow)
{
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void MWindow::Create(HINSTANCE hInst, MWindowClass windowClass, const TCHAR* windowTitle, int style, int x, int y, int width, int height)
{
	hInstance = hInst;

	MWindowBase::Create(windowClass.lpszClassName, windowTitle, style,
		x, y, width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		MessageBox(NULL, L"Failed to create window", L"Error", 0);
		exit(1);
	}
}

// ==== MControl ==== //
void SetDefaultFont(HWND hWnd)
{
	NONCLIENTMETRICS metrics = {};
	metrics.cbSize = sizeof(metrics);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);

	HFONT guiFont = CreateFontIndirect(&metrics.lfCaptionFont);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)guiFont, LOWORD(TRUE));
}

MControl::MControl(HWND parentHWnd, int ctrlId)
{
	this->parentHWnd = parentHWnd;
	this->ctrlId = ctrlId;
}

MControl::MControl(MWindow* parentWindow, int ctrlId) : MControl(parentWindow->GetHWnd())
{}

void MControl::Create(const TCHAR* className, const TCHAR* text, int style, int x, int y, int width, int height)
{
	MWindowBase::Create(className, text, style, x, y, width, height, parentHWnd, (HMENU)ctrlId, NULL, NULL);
	SetDefaultFont(hWnd);
}

void MControl::SetText(const TCHAR* text)
{
	SetWindowText(hWnd, text);
}

int MControl::GetId()
{
	return ctrlId;
}
