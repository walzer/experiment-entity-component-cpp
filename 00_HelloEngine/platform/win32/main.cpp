// 00_HelloEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "Game/GameApp.h"
#include "GameView.h"
#include "Resource.h"
#include "CCRuntime_win32.h"

using namespace std;

#define MAX_LOADSTRING 100

extern void pchTest();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    AllocConsole();
    FILE * console = nullptr;
    freopen_s(&console, "CONOUT$", "w", stdout);

    CCEventTest();
    pchTest();
    CCComponentTest();

    MSG msg;
    HACCEL hAccelTable;
    GameApp game;
    CCRuntime_win32 runtime;

    game.init();
    // Perform application initialization:
    if (! runtime.init(hInstance))
    {
        return FALSE;
    }
    GameView * view = static_cast<GameView*>(runtime.getSurface());
    ShowWindow(view->getHandle(), nCmdShow);
    UpdateWindow(view->getHandle());

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY00_HELLOENGINE));

    LARGE_INTEGER nFreq;
    LARGE_INTEGER nLast;
    LARGE_INTEGER nNow;
    LARGE_INTEGER nInterval;

    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nLast);
    nInterval.QuadPart = (LONGLONG)(0.5f * nFreq.QuadPart);

    // Main message loop:
    while (1)
    {
        if (! PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Get current time tick.
            QueryPerformanceCounter(&nNow);

            // If it's the time to draw next frame, draw it, else sleep a while.
            if (nNow.QuadPart - nLast.QuadPart > nInterval.QuadPart)
            {
                nLast.QuadPart = nNow.QuadPart;
                runtime.run();
            }
            else
            {
                Sleep(0);
            }
            continue;
        }

        if (WM_QUIT == msg.message)
        {
            // Quit message loop.
            break;
        }

        // Deal with windows message.
        if (! hAccelTable || ! TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    runtime.done();
    game.done();

    FreeConsole();
    fclose(console);
    return (int) msg.wParam;
}
