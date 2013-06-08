#include "pch.h"

#include "Resource.h"

#include "GameView.h"

#define MAX_LOADSTRING 100

// Global Variables:
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int                 InitGLView(HWND);
void                SwapBuffer();

static ATOM s_gameViewClass;

GameView::GameView()
    : _hwnd(NULL)
{
}

GameView::~GameView()
{
}

bool GameView::init()
{
    BaseType::init();
    HINSTANCE hInstance = (HINSTANCE)CCRuntime::getInstance()->getAppInstance();
    if (! s_gameViewClass)
    {
        // Initialize global strings
        LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
        LoadString(hInstance, IDC_MY00_HELLOENGINE, szWindowClass, MAX_LOADSTRING);
        s_gameViewClass = MyRegisterClass(hInstance);
    }
    bool ret = false;
    do
    {
        if (! s_gameViewClass) break;

        _hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

        SetWindowLong(_hwnd, GWL_USERDATA, (LONG)this);
        InitGLView(_hwnd);
        if (! _hwnd) break;
        ret = true;
    } while (false);
    return ret;
}

void GameView::done()
{
    if (_hwnd)
    {
        _hwnd = NULL;
    }
    BaseType::done();
}

void GameView::swapBuffer()
{
    SwapBuffer();
}

void GameView::show()
{

}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW/* | CS_OWNDC*/;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY00_HELLOENGINE));
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= NULL;
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY00_HELLOENGINE);
    wcex.lpszClassName	= szWindowClass;
    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return (RegisterClassEx(&wcex) || 1410 == GetLastError()) ? true : false;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    CC_LOGI("WndProc message: %d(%x)\n", message, message);
    switch (message)
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_SIZE:
        {
            GameView *surface = (GameView *)GetWindowLong(hWnd, GWL_USERDATA);
            int w = LOWORD(lParam);
            int h = HIWORD(lParam);
            surface->setWidth(w);
            surface->setHeight(h);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            if (wParam == MK_LBUTTON)
            {
                GameView *surface = (GameView *)GetWindowLong(hWnd, GWL_USERDATA);
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                int halfW = surface->getWidth() / 2;
                int halfH = surface->getHeight() / 2;
                PointerArgs pointer = 
                {
                    0, 
                    (float)(x - halfW) / halfW,
                    (float)(halfH - y) / halfH,
                };
                surface->onPointerDownEvent(::std::cref(pointer));
            }
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code here...
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

#include <EGL/egl.h>
#include <GLES2/gl2.h>

EGLDisplay eglDisplay;
EGLContext eglContext;
EGLSurface eglSurface;

int InitGLView(HWND hWnd)
{
    EGLint configAttribList[] =
    {
        //EGL_RED_SIZE,       8,
        //EGL_GREEN_SIZE,     8,
        //EGL_BLUE_SIZE,      8,
        //EGL_ALPHA_SIZE,     8,
        EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
        EGL_DEPTH_SIZE,     EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   EGL_DONT_CARE,
//        EGL_SAMPLE_BUFFERS, 1,
        EGL_NONE
    };
    EGLint surfaceAttribList[] =
    {
        EGL_NONE, EGL_NONE
    };

    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
   
    // Get Display
    display = eglGetDisplay(GetDC(hWnd));
    if ( display == EGL_NO_DISPLAY )
    {
        return EGL_FALSE;
    }

    // Initialize EGL
    if ( !eglInitialize(display, &majorVersion, &minorVersion) )
    {
        return EGL_FALSE;
    }

    eglBindAPI(EGL_OPENGL_ES_API);

    // Get configs
    if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
    {
        return EGL_FALSE;
    }

    // Choose config
    if ( !eglChooseConfig(display, configAttribList, &config, 1, &numConfigs) )
    {
        return EGL_FALSE;
    }

    // Create a surface
    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, surfaceAttribList);
    if ( surface == EGL_NO_SURFACE )
    {
        return EGL_FALSE;
    }

    // Create a GL context
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
    if ( context == EGL_NO_CONTEXT )
    {
        return EGL_FALSE;
    }   
   
    // Make the context current
    if ( !eglMakeCurrent(display, surface, surface, context) )
    {
        return EGL_FALSE;
    }

    eglDisplay = display;
    eglSurface = surface;
    eglContext = context;
    return EGL_TRUE;
}

void SwapBuffer()
{
    eglSwapBuffers(eglDisplay, eglSurface);
}
