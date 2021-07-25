#pragma once

#include <Windows.h>

#include <map>

using std::map;

class MWindowClass: public WNDCLASSEX
{
public:
	void Generate(HINSTANCE hInst, int style, TCHAR* className, WNDPROC wndProc);
	void Register();
};

class MWindowBase
{
protected:
	HWND hWnd;
	// Functions
public:
	void CreateEx(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int width, int height, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	void Create(LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int x, int y, int width, int height, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	HWND GetHWnd();
};

class MWindow: public MWindowBase
{
protected:
	HINSTANCE hInstance;

	// Functions
protected:
public:
	void Create(HINSTANCE hInst, MWindowClass mWindowClass, TCHAR* windowTitle, int style = 0, int x = 0, int y = 0, int width = 0, int height = 0);
	virtual BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
};

class MControl : public MWindowBase
{
protected:
	HWND parentHWnd;
	int ctrlId;
	// Functions
public:
	MControl(HWND parentHWnd, int ctrlId = NULL);
	MControl(MWindow* parentWindow, int ctrlId = NULL);
public:
	void Create(const TCHAR* className, const TCHAR* text, int style, int x, int y, int width, int height);
	void SetText(const TCHAR* text);
	int GetId();
};

MWindowBase* GetWindowFromHWnd(HWND hWnd);