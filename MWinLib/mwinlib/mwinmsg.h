#pragma once

#include <map>
#include <vector>
#include <tuple>
#include <functional>

using std::map;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::function;

int RunMessageLoop();
int RunMessageLoop(HACCEL hAccelTable);

#define DEF_WNDPROC(func_name, mMessageMap)\
	LRESULT CALLBACK func_name(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)\
	{\
		return _WndProcHelper(mMessageMap, hWnd, message, wParam, lParam);\
	}
#define DEF_DLGPROC(func_name, mMessageMap)\
	INT_PTR CALLBACK func_name(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)\
	{\
		return _DlgProcHelper(mMessageMap, hWnd, message, wParam, lParam);\
	}


typedef LRESULT(CALLBACK* MWNDPROC)(HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom);
typedef void(CALLBACK* MWNDPROCSIMUL)(HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom);

typedef LRESULT(CALLBACK* MCTRLPROC)(HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom);

class MMessageMap 
{
private:
	map<UINT, tuple<MWNDPROC, LPARAM>> messageMap;
	map<UINT, vector<tuple<MWNDPROCSIMUL, LPARAM>>> simulMessageMap;
	map<UINT, map<int, tuple<MCTRLPROC, LPARAM>>> ctrlMap;

	friend BOOL CALLBACK CommonProcHelper(MMessageMap mMessageMap, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result);
	//Functions
public:
	void Register(UINT message, MWNDPROC handler, LPARAM custom = NULL);
	void RegisterSimultaneous(UINT message, MWNDPROCSIMUL handler, LPARAM custom = NULL);
	void RegisterControl(int ctrlId, UINT message, MCTRLPROC handler, LPARAM custom);
};

#define REGISTER_MSG(messageMap, msg, funcBody)							(messageMap).Register((msg), [](HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom) -> LRESULT funcBody)
#define REGISTER_MSG_C(messageMap, msg, funcBody, custom_)				(messageMap).Register((msg), [](HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom) -> LRESULT funcBody, (custom_))

#define REGISTER_MSG_SIMUL(messageMap, msg, funcBody)					(messageMap).RegisterSimultaneous((msg), [](HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom) funcBody)
#define REGISTER_MSG_SIMUL_C(messageMap, msg, funcBody, custom_)		(messageMap).RegisterSimultaneous((msg), [](HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom) funcBody, (custom_))

#define REGISTER_CTRL_MSG(messageMap, hWnd, msg, funcBody)				(messageMap).RegisterControl((hWnd), (msg), [](HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom) -> LRESULT funcBody)
#define REGISTER_CTRL_MSG_C(messageMap, hWnd, msg, funcBody, custom_)	(messageMap).RegisterControl((hWnd), (msg), [](HWND hWnd, WPARAM wParam, LPARAM lParam, LPARAM custom) -> LRESULT funcBody, (custom_))

LRESULT CALLBACK _WndProcHelper(MMessageMap mMessageMap, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK _DlgProcHelper(MMessageMap mMessageMap, HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);