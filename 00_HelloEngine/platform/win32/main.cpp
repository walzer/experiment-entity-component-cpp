// 00_HelloEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "GameView.h"
#include "Resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE s_Instance;					        // current instance

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    s_Instance = hInstance;
    MSG msg;
    HACCEL hAccelTable;
    GameView view;

    // Perform application initialization:
    if (! view.init())
    {
        return FALSE;
    }
    ShowWindow(view.getHandle(), nCmdShow);
    UpdateWindow(view.getHandle());

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY00_HELLOENGINE));

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    view.done();
    return (int) msg.wParam;
}
