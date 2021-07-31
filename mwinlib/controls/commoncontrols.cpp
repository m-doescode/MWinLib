#include "commoncontrols.h"

#include <map>

using std::map;

// == MStatic == //
#define MStatic_SetBackgroundColor(color, messageMap)\
	messageMap.Register(WM_CTLCOLORSTATIC, [](HWND hWnd, WPARAM wParam, LPARAM lParam) -> LRESULT\
		{\
			HDC hdcStatic = (HDC)wParam;\
			SetTextColor(hdcStatic, RGB(0, 0, 0));\
			SetBkColor(hdcStatic, color);\
			return (INT_PTR)CreateSolidBrush(color);\
		});

MStatic::MStatic(HWND parentHWnd) : MControl(parentHWnd)
{}

MStatic::MStatic(MWindow* parentWindow) : MControl(parentWindow)
{}

//map<HWND, COLORREF*> bgColorMap;

void MStatic::Create(const TCHAR* text, int x, int y, int width, int height)
{
	MControl::Create(L"Static", text, WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON, x, y, width, height);
	//hWnd = CreateWindow(L"Static", text, WS_VISIBLE | WS_CHILD, x, y, width, height, parentHWnd, NULL, NULL, NULL);
	//bgColorMap[hWnd] = &bgColor;
}

//bool handlersRegistered = false;
//void RegisterHandlers(MMessageMap* messageMap)
//{
//	if (handlersRegistered)
//		return;
//	handlersRegistered = true;
//
//	// Register handler for when static requests its color. A pointer to bgColor is passed so
//	// that if it is ever changed, it updates accordingly.
//	//(*messageMap).Register(WM_CTLCOLORSTATIC, [](HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom) -> LRESULT
//	REGISTER_MSG(*messageMap, WM_CTLCOLORSTATIC,
//		{
//			HDC hdcStatic = (HDC)wParam;
//			auto it = bgColorMap.find((HWND)lParam);
//			if (it != bgColorMap.end())
//			{
//				COLORREF color = *it->second;
//				SetTextColor(hdcStatic, RGB(0, 0, 0));
//				SetBkColor(hdcStatic, color);
//				return (INT_PTR)CreateSolidBrush(color);
//			}
//			else
//				return DefWindowProc(hWnd, WM_CTLCOLORSTATIC, wParam, lParam);
//		});
//}

void MStatic::SetBackgroundColor(COLORREF color, MMessageMap* messageMap)
{
	//bgColor = color;
	//RegisterHandlers(messageMap);
	REGISTER_CTRL_MSG_C(*messageMap, ctrlId, WM_CTLCOLORSTATIC,
				{
					HDC hdcStatic = (HDC)wParam;
					COLORREF color = (COLORREF)custom;
					SetTextColor(hdcStatic, RGB(0, 0, 0));
					SetBkColor(hdcStatic, color);
					return (INT_PTR)CreateSolidBrush(color);
				}, color);
}

// ==== MButton ==== //
MButton::MButton(HWND parentHWnd) : MControl(parentHWnd)
{}

MButton::MButton(MWindow* parentWindow) : MControl(parentWindow)
{}

void MButton::Create(const TCHAR* text, int x, int y, int width, int height)
{
	MControl::Create(L"Button", text, WS_VISIBLE | WS_CHILD, x, y, width, height);
}

void MButton::SetImage(HBITMAP image)
{
	SendMessage(hWnd, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)image);
}

void MButton::SetImage(HICON icon)
{
	SendMessage(hWnd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon);
}

