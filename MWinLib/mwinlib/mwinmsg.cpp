#include <windows.h>

#include "mwinmsg.h"
int RunMessageLoop()
{
	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

int RunMessageLoop(HACCEL hAccelTable)
{
	MSG msg;

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


void MMessageMap::Register(UINT message, MWNDPROC handler, LPARAM custom)
{
	if (messageMap.find(message) != messageMap.end())
	{
		TCHAR str[100];
		wprintf(str, L"Message for ID %d was already defined", message);
		MessageBox(NULL, str, L"Error", 0);
	}

	messageMap[message] = make_tuple(handler, custom);
}

void MMessageMap::RegisterSimultaneous(UINT message, MWNDPROCSIMUL handler, LPARAM custom)
{
	auto it = simulMessageMap.find(message);
	vector<tuple<MWNDPROCSIMUL, LPARAM>>* result;
	if (it == simulMessageMap.end())
		result = &(simulMessageMap[message] = vector<tuple<MWNDPROCSIMUL, LPARAM>>());
	else
		result = &it->second;
	(*result).push_back(make_tuple(handler, custom));
}

void MMessageMap::RegisterControl(int ctrlId, UINT message, MCTRLPROC handler, LPARAM custom)
{
	auto it = ctrlMap.find(message);
	map<int, tuple<MCTRLPROC, LPARAM>>* result;
	if (it == ctrlMap.end())
		result = &(ctrlMap[message] = map<int, tuple<MCTRLPROC, LPARAM>>());
	else
		result = &it->second;
	(*result)[ctrlId] = make_tuple(handler, custom);
}

// WARNING: Spaghetti to the max
// Why? Because there is no map.contains in cpp
BOOL CALLBACK CommonProcHelper(MMessageMap messageMap, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result)
{
	// simulMessageMap
	{
		auto it = messageMap.simulMessageMap.find(message);
		if (it != messageMap.simulMessageMap.end())
		{
			for (tuple<MWNDPROCSIMUL, LPARAM> msgTuple : it->second)
			{
				std::get<0>(msgTuple)(hWnd, wParam, lParam, std::get<1>(msgTuple));
			}
		}
	}

	// messageMap
	{
		auto it = messageMap.messageMap.find(message);
		if (it != messageMap.messageMap.end())
		{
			result = std::get<0>(it->second)(hWnd, wParam, lParam, std::get<1>(it->second));
			return TRUE;
		}
	}

	// ctrlMap
	{
		auto it = messageMap.ctrlMap.find(message);
		if (it != messageMap.ctrlMap.end())
		{
			auto map_ = it->second;
			int ctrlId = GetWindowLong((HWND)lParam, GWL_ID);
			auto it2 = map_.find(ctrlId);
			if (it2 != map_.end())
			{
				result = std::get<0>(it2->second)(hWnd, wParam, lParam, std::get<1>(it2->second));
				return TRUE;
			}
		}
	}

	return FALSE;
}

LRESULT CALLBACK _WndProcHelper(MMessageMap messageMap, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	if (CommonProcHelper(messageMap, hWnd, message, wParam, lParam, result))
		return result;

	return DefWindowProc(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK _DlgProcHelper(MMessageMap messageMap, HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;
	if (CommonProcHelper(messageMap, hDlg, message, wParam, lParam, result))
		return result;

	return (INT_PTR)FALSE;
}