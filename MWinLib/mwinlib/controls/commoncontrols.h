#pragma once

#include <windows.h>

#include "../mwindow.h"
#include "../mwinmsg.h"

class MStatic: public MControl
{
protected:
	COLORREF bgColor;
	// Functions
public:
	MStatic(HWND parentHWnd);
	MStatic(MWindow* parentWindow);
public:
	void Create(const TCHAR* text, int x, int y, int width, int height);
	void SetBackgroundColor(COLORREF color, MMessageMap* messageMap);
};

class MButton: public MControl
{
public:
	MButton(HWND parentHWnd);
	MButton(MWindow* parentWindow);
public:
	void Create(const TCHAR* text, int x, int y, int width, int height);
	void SetImage(HBITMAP image);
	void SetImage(HICON icon);
};