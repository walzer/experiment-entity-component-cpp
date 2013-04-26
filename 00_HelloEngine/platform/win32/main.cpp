// 00_HelloEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include <stdio.h>

#include "CCContext.h"

#include "GameView.h"
#include "Resource.h"
#include "CCEvent.h"

struct _OfpGuard
{
    FILE* ofp;
    _OfpGuard(const char * name = nullptr)
        : ofp(nullptr)
    {
        open(name);
    }
    ~_OfpGuard()
    {
        if (ofp)
        {
            fclose(ofp);
            ofp = nullptr;
        }
    }
    void open(const char * name)
    {
        if (name)
        {
                fopen_s(&ofp, name, "w");
        }
    }
} s_OfpGuard("application.log");

int printf_stdout_logfile(const char *format, ...)
{
  va_list ap;
  int  rc;

  va_start(ap, format);
  rc = vprintf(format, ap);
  if (s_OfpGuard.ofp)
  {
    rc = vfprintf(s_OfpGuard.ofp, format, ap);
  }
  va_end(ap);

  return rc;
}

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE s_Instance;					        // current instance
unique_ptr<CCContext> s_context(nullptr);
void initContext();
void doneContext();

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

    LARGE_INTEGER nFreq;
    LARGE_INTEGER nLast;
    LARGE_INTEGER nNow;
    LARGE_INTEGER nInterval;

    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nLast);
    nInterval.QuadPart = (LONGLONG)(0.5f * nFreq.QuadPart);

    initContext();

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
                s_context->run();
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

    doneContext();
    view.done();

    FreeConsole();
    fclose(console);
    return (int) msg.wParam;
}

#include "CCEntityManager.h"

void initContext()
{
    s_context.reset(new CCContext());
    s_context->add(make_shared<CCEntityManager>(), "entity_manager");
}

void doneContext()
{
    s_context->done();
    s_context.reset();
}
