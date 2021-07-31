# Tutorial

Welcome to the MWinLib tutorial!

In this doc we will go over how to make your first Win32 application (actually, window) using MWinLib.

This app will support all the features present in the default Win32 demo provided with Visual Studio. (Desktop Application Wizard)

## Table of Contents:

1. [Setting up](#setting-up)
2. [Creating a window class](#creating-our-window-class)
3. [Creating a window](#creating-our-window)
4. [Creating and registering our message map](#creating-our-message-map)
5. [Running our app for the first time](#running-our-app-for-the-first-time)
6. [Registering our message handlers](#registering-our-message-handlers)
7. [Adding a menu bar](#adding-a-menu-bar)
8. [Adding an about box](#adding-an-about-box)
9. [Enabling visual styles](#enabling-visual-styles)

## Setting up

First, you will need to clone the repository

<img width="50%" src="https://uh09h043tgr.shx.is/55fZT0oWp.png" />

Copy the "mwinlib" folder into your project directory or libs folder.

<img width="70%" src="https://uh09h043tgr.shx.is/55g0Tu4L2.png" />

Now, create your source file.

To start off, you will need to include the windows.h file and have a WinMain function.

The entire mwinlib library is available as a header, simply include it like this:

```cpp
#include <windows.h>
#include "resource.h"

// MWinLib
#include "mwinlib/mwinlib.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// Code goes here
}
```

## Creating our window class

To get started, we need to define our window class.

MWinLib takes care of for you, simply use the `MWindowClass` class to generate one for you.

First, you must instantiate it.

```cpp
MWindowClass windowClass;
```

Then, you must generate all of the fields. You can use the `Generate` function to do that for you.

The `Generate` function takes four parameters, the hInstance, the style, the class name, and the WindowProc.

For the WindowProc, we wil be taking care of that later in [Creating and registering our message map](#creating-our-message-map)

Here is what it should look like so far

```cpp
MWindowClass windowClass;
windowClass.Generate(hInstance, CS_HREDRAW | CS_VREDRAW, L"MyWindow", WndProc);
```

Last but not least we've got to actually register our class.
You can do this by simply calling the `Register` function

This should be your finished code:
```cpp
MWindowClass windowClass;
windowClass.Generate(hInstance, CS_HREDRAW | CS_VREDRAW, L"MyWindow", WndProc);
windowClass.Register();
```

## Creating our window

Much like `MWindowClass`, there also exists a class for windows.
You can use `MWindow` to create your window.

First instantiate it, then call `Create` on it.

The create function takes in eight parameters:
1. The `HINSTANCE`
2. The `MWindowClass`
3. The title (wide-string)
4. The style(s)
5. x
6. y
7. width
8. height

In the default app provided in the desktop wizard, these are the values used:

```cpp
MWindow window;
window.Create(hInstance, windowClass, L"My Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0);
```

What's next is to call `InitInstance`. For this, you could simply use the following to catch any errors:
```cpp
if (!window.InitInstance(hInstance, nCmdShow))
{
	return FALSE;
}
```

Finally, you need to run the message map. To do this, simply call `RunMessageLoop` and return its return value to `WinMain`.
```cpp
return RunMessageLoop();
```
(Optionally, you could pass in the accelerator table as the first argument to support accelerators)

## Creating our message map
Currently, our `WndProc` has not yet been created.

You could optionally pass in your own `WndProc`, but it's much easier to just use the provided `MMessageMap`. It also allows you to access exclusive features to the message map within controls.

Creating the message map is pretty easy, as of the moment, it is not possible to create a local message map (inside of a function) and it has to be global, but this is subject to change.

Simply add the following lines **above** your `WinMain` function:
```cpp
MMessageMap messageMap;

DEF_WNDPROC(WndProc, messageMap)
```

All that the `DEF_WNDPROC` macro does is define the `WndProc` function and pass the values to another helper function our message map.

The first argument is the name of the function, when we were defining our class, we put "WndProc" so that is what we need to call it.

The second argument is just the message map itself.

## Running our app for the first time

We are pretty much done for the essentials.

You can go ahead and press "Run" in your IDE.
Hopefully, no errors pop up.

But now you might notice that when you close the window, it doesn't actually end the process. This is because we haven't hooked onto the window quit message. For now, click the stop button in Visual Studio.

Our window also looks very barren, it has no icon, no toolbar, no controls, and doesn't even quit properly! We will solve this in the next couple steps.

## Registering our message handlers

To register a message handler, you could call the `Register` function on your message map, however, the signature of the lambda for each handler is very long. An easier way is to just use `REGISTER_MSG`

It takes in three parameters, first is the message map itself, second is the ID of the message your are handling, and third is the **body** of the lambda.

Because the parameters are defined inside the macro, you don't get to see what they are called.

The parameters are as follows:
1. **HWND** hWnd
2. **WPARAM** wParam
3. **LPARAM**	lParam
4. **LPARAM** custom

The return type is `LRESULT`

That last one, `custom` only matters if you use the `REGISTER_MSG_C` macro and passed in a fourth parameter. The fourth parameter is an `LPARAM` that gets passed into the handler as custom.

To make sure things aren't cluttered, I create another void function called `RegisterHandlers` and I call it before the window is created.

Here is an example of a handler:
```cpp
REGISTER_MSG(messageMap, WM_DESTROY, { PostQuitMessage(0); return 0; });
```

Here, it captures the `WM_DESTROY` message, and calls `PostQuitMessage(0)`

Here is what `RegisterHandlers` should currently look like:
```cpp
void RegisterHandlers()
{
	REGISTER_MSG(messageMap, WM_DESTROY, { PostQuitMessage(0); return 0; });
}
```

After running, now you should notice that the program actually quits when you press the `X` button.

## Adding a menu bar

Adding a menu bar is fairly easy. If you have created your app using the Desktop Application Wizard, you should already be supplied with icons, an about box (which will be useful in the next step), and a menu bar.

Unfortunately, MWinLib doesn't have a function for adding a menu bar yet, but it isn't too difficult to manually set the field.

Insert the following before you register your window class (but after you generate):
```cpp
windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MWINLIB));
windowClass.lpszMenuName = MAKEINTRESOURCEW(IDC_MWINLIB);
windowClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
```
The above will set an icon, and the menu bar for you.

Now if you press run, you will notice that in the corner, a menu bar appears, and the icon actually shows up.

![](https://uh09h043tgr.shx.is/55NTpUX9P.png)

But you might notice that neither the "Exit" button in "File" nor the "About..." button in "Help" actually work.

This is because we haven't handled the message for them.
(In the future, there will be a class for handling all this)

To do this, simply insert the following code into your `RegisterHandlers` function:
```cpp
REGISTER_MSG(messageMap, WM_COMMAND,
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutWndProc);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
		}

		return 0;
	});
```
For now, you can keep the `DialogBox` macro call commented, because we will be dealing with that [in the next step](#creating-an-about-box).

Now, after re-running, the "Exit" button should now work!

## Creating an about box

To create a dialog box, the following need to be done:
1. Create another message map for the dialog
2. Add handlers for `WM_INITDIALOG` and `WM_COMMAND`
3. Call the `DialogBox` macro

For number one, you could simply add these lines below the message map for your main window:
```cpp
MMessageMap aboutMessageMap;
DEF_DLGPROC(AboutWndProc, aboutMessageMap);
```

For number two, add these lines to your `RegisterHandlers` function:
```cpp
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
```

For number three, just uncomment the "DialogBox" function call in the `WM_COMMAND` handler. 

Your about box should now work.

## Enabling visual styles

Your window has finally been created, but it looks like it was made in the year 2000.

To fix this, you need to enable visual styles.
The simplest way to do this is to simply add this at the beginning of your file:
```cpp
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
```



# End of tutorial

This is the end of the tutorial. You should have completed by now your first desktop application using MWinLib.
