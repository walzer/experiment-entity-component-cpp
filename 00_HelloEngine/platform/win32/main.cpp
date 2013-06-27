// 00_HelloEngine.cpp : Defines the entry point for the application.
//

#include "pch.h"

#include "Resource.h"

using namespace std;
using namespace cc;

#define MAX_LOADSTRING 100

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

    eventTest();

    return 0;
}
