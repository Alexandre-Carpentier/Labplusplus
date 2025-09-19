/*
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitPixelFormat(HDC hDC);
void InitGL();
void Render(HDC hDC, RECT const& rc);
HGLRC g_hRC = NULL;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    const char CLASS_NAME[] = "OpenGLTriangleClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Triangle OpenGL 1.0",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    if (!hWnd) return -1;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hDC = NULL;

    switch (message)
    {
    case WM_CREATE:
        hDC = GetDC(hWnd);
        InitPixelFormat(hDC);
        g_hRC = wglCreateContext(hDC);
        wglMakeCurrent(hDC, g_hRC);
        InitGL();
        return 0;

    case WM_SIZE:
    {
        RECT rc;
        GetClientRect(hWnd, &rc);
        glViewport(0, 0, rc.right - rc.left, rc.bottom - rc.top);
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        Render(hDC, rc);
        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        if (g_hRC)
        {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(g_hRC);
            g_hRC = NULL;
        }
        if (hDC)
        {
            ReleaseDC(hWnd, hDC);
            hDC = NULL;
        }
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void InitPixelFormat(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    int pf = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pf, &pfd);
}

void InitGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // fond sombre
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void Render(HDC hDC, RECT const& rc)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Dessin d'un triangle (OpenGL 1.0)
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // sommet rouge
    glVertex2f(0.0f, 0.6f);

    glColor3f(0.0f, 1.0f, 0.0f); // sommet vert
    glVertex2f(-0.6f, -0.6f);

    glColor3f(0.0f, 0.0f, 1.0f); // sommet bleu
    glVertex2f(0.6f, -0.6f);
    glEnd();

    SwapBuffers(hDC);

    return;
}
*/
/*
void Render(HDC hDC, RECT const& rc)
{
    (void)hDC;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Dessin d'un triangle (OpenGL 1.0)
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // sommet rouge
    glVertex2f(0.0f, 0.6f);

    glColor3f(0.0f, 1.0f, 0.0f); // sommet vert
    glVertex2f(-0.6f, -0.6f);

    glColor3f(0.0f, 0.0f, 1.0f); // sommet bleu
    glVertex2f(0.6f, -0.6f);
    glEnd();

    SwapBuffers(GetDC(WindowFromDC(hDC)));
}
*/