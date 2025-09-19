#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "kernel32.lib")
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "User32.lib")
#pragma comment (lib, "Comctl32.lib")

#include <Windows.h>

#include <process.h>
#include <commctrl.h>
#include "resource.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Wingraph.h"

// Must load the last Windows UXTheme
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HINSTANCE hInst;                    // Global process instance
HGRAPH hGraph;                      // Graph handle

HANDLE LaunchThreadHandle = NULL;
unsigned int acquireThread(void* data);
HANDLE acquireThreadHandle = NULL;      // Acquisition thread handle
HANDLE hEvent = NULL;                      // Event to stop the thread

short ACQUIRE_FREQ =500;
const short DISP_FREQ = 250 ;           // ms
const short SIGNB = 1 ;                 // Count of signal to acquire
bool bFiltering;

HWND hTrack;                        // Slider
HWND hCurrentFreq;                  // Indicator
LRESULT cur_freq;                   // SendMessage res

inline long long PerformanceFrequency()
{
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    return li.QuadPart;
}

inline long long PerformanceCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart;
}

unsigned int launchThread(void* data)
{
    // Retrive hWnd 

    HWND hWnd = (HWND)data;
    if (hWnd == NULL)
        return -1;

    // Enable the reccording mode

    if (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_CHECK))
        SetRecordingMode(hGraph, LOGGER_ASCII);
    else
        SetRecordingMode(hGraph, LOGGER_NONE);

    SetSignalCount(hGraph, 1);
	SetSignalLabel(hGraph, "Force", 0);
	SetSignalColor(hGraph, 255, 0, 0, 0);
	SetAutoscaleMode(hGraph, true);
	SetDisplayCursor(hGraph, true);
	SetZoomFactor(hGraph, 1);
	SetFilteringMode(hGraph, FILTER_NONE, 0);


    // Start the Graph

    StartGraph(hGraph, "");

    // Grey out ctrls

    EnableWindow(GetDlgItem(hWnd, IDC_CHECK), FALSE);
    EnableWindow(GetDlgItem(hWnd, IDC_ACQUIRE), FALSE);
    EnableWindow(GetDlgItem(hWnd, IDC_STOP_ACQUIRE), TRUE);

    // create Event and launch the acquisition thread

    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    acquireThreadHandle = (HANDLE)_beginthreadex(0, 0, (_beginthreadex_proc_type)&acquireThread, &hEvent, 0, 0);
    if (acquireThreadHandle)
    {
        SetDlgItemTextW(hWnd, IDC_EDIT1, L"Measure started");
    }
    else
    {
        SetDlgItemTextW(hWnd, IDC_EDIT1, L"Measure Failed");
    }
    return 0;
}


unsigned int acquireThread(void* data)
{
    HANDLE Event = *(HANDLE*)data;
    long long t1 = PerformanceCounter();
    long long t2;
    long long delta;
    long long freq = PerformanceFrequency();

    while (WaitForSingleObject(hEvent, 0) != WAIT_OBJECT_0)
    {
        t2 = PerformanceCounter();
        delta = (double)((t2 - t1)) / freq * 1000;

        if (delta >= ACQUIRE_FREQ)
        {
            t1 = PerformanceCounter();

            float result = rand() % 10;


            double fdata[SIGNB];

            // add point to the graph

            fdata[0] = result;
            AddPoint(hGraph, fdata, SIGNB);

            // update TextCtrl with current value
            // and not draw the same value twice

            wchar_t buffer[MAX_PATH] = L"";
            wchar_t last_buffer[MAX_PATH] = L"";

            swprintf(buffer, 200, L"%.1f N", result);

            GetDlgItemTextW(GetGraphParentWnd(hGraph), IDC_EDIT1, last_buffer, MAX_PATH - 1);
            if (wcscmp(buffer, last_buffer) != 0)
            {
                SetDlgItemTextW(GetGraphParentWnd(hGraph), IDC_EDIT1, buffer);
            }
        }
    }
    return 0;
}

void UpdateLabel(HWND hWnd)
{
    hTrack = GetDlgItem(hWnd, IDC_SLIDER);
    hCurrentFreq = GetDlgItem(hWnd, IDC_CURRENT_FREQ);
    LRESULT pos = SendMessageW(hTrack, TBM_GETPOS, 0, 0);
    wchar_t buf[7];
    pos *= 10;
    wsprintfW(buf, L"%ldms", pos);
    if ((int)pos >= 10 && (int)pos <= 1000)
    {
        ACQUIRE_FREQ = (int)pos;
        SetWindowTextW(hCurrentFreq, buf);
    }
}

INT_PTR CALLBACK WndProc(HWND hWnd, UINT message, WPARAM  wParam, LPARAM  lParam)
{
    static PAINTSTRUCT ps;              // paint struct for rendering
    static RECT client_rect;            // WM_SIZE
    static RECT item_r;                 // WM_SIZE
    static HWND hItem;                  // WM_SIZE

    int index = 0;
    int itempos = 0;
    int percentSPACE = 0;
    const int sizemap[9][2] = {
        {IDC_QUIT, 10 },
        {IDC_STOP_ACQUIRE, 10},
        {IDC_EDIT1, 10 },
        {IDC_ACQUIRE, 10 },
        {IDC_STATIC_FREQ, 10 },
        {IDC_SLIDER, 15 },
        {IDC_CURRENT_FREQ, 5 },
        {IDC_CHECK, 10 },
        {IDC_FILTER, 10 }
    };

    long long start = 0.0;
    long long finish = 0.0;
    long long frequency = 0.0;
    double freq = 0.0;

    switch (message)
    {
    case WM_NOTIFY:                                                     // Handle the slider 
    {
        if (((LPNMHDR)lParam)->idFrom == IDC_SLIDER)
        {
            UpdateLabel(hWnd);
            return TRUE;
        }
        break;
    }
    case WM_TIMER:                                                      // Refresh the graph with WM_PAINT
    {
        switch (wParam)
        {
        case IDT_TIMER:
        {
            SendMessage(hWnd, WM_PAINT, 0, 0);
            break;
        }
        }
    }
    /// ////////////////////////////
    /// COMMAND MSG
    /// ////////////////////////////
    case WM_COMMAND:                                                    // Analyze messages from controls
    {
        switch (wParam)
        {
        case IDC_QUIT:
        {
            StopGraph(hGraph);
            PostQuitMessage(0);
            break;
        }
        case IDC_FILTER:
        {
            if (BST_CHECKED == IsDlgButtonChecked(hWnd, IDC_FILTER))
                bFiltering = true;
            else
                bFiltering = false;

            break;
        }

        case IDC_ACQUIRE:
        {
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), true);
            LaunchThreadHandle = NULL;
            LaunchThreadHandle = (HANDLE)_beginthreadex(0, 0, (_beginthreadex_proc_type)launchThread, hWnd, 0, 0);
            break;
        }

        case IDC_STOP_ACQUIRE:
        {
            if (hEvent)
            {
                SetEvent(hEvent); // signal thread to end
                if (acquireThreadHandle)
                {
                    WaitForSingleObject(acquireThreadHandle, 5000); // wait up to 5 seconds
                    CloseHandle(hEvent);
                    CloseHandle(acquireThreadHandle);
                    acquireThreadHandle = NULL;
                }
                hEvent = NULL;
            }

            if (TRUE == GetGraphState(hGraph))
                StopGraph(hGraph);

            EnableWindow(GetDlgItem(hWnd, IDC_CHECK), TRUE);
            EnableWindow(GetDlgItem(hWnd, IDC_ACQUIRE), TRUE);
            EnableWindow(GetDlgItem(hWnd, IDC_STOP_ACQUIRE), FALSE);
            SetDlgItemText(hWnd, IDC_EDIT1, "Measure stop");
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), false);
            break;
        }
        }
        break;
    }
    /// ////////////////////////////
    /// WINDOWS MSG
    /// ////////////////////////////
    case WM_INITDIALOG:
        printf("[*] WM_INITDIALOG start\n");

        // Display the application icon on the dialog toolbar

        HICON hIcon;
        hIcon = LoadIcon((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDI_ICON1));
        SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

        // Retrieve the client windows size to draw the graph inside

        GetClientRect(hWnd, &client_rect);
        client_rect.bottom -= 100;
        client_rect.right -= 100;

        // Create the graph

        hGraph = NULL;
        hGraph = CreateGraph(hWnd,                              // Parent Window of the Graph
            client_rect,                                        // Size of the graph
            SIGNB,                                              // Number of signals to handle [1-16]
            1000                                                // Buffer of 1000 points   
        );

        if (hGraph == NULL)
        {
            wprintf(L"[*] Fail to CreateGraph()\n");
            break;
        }
        hTrack = GetDlgItem(hWnd, IDC_SLIDER);
        SendMessageW(hTrack, TBM_SETRANGE, TRUE, MAKELONG(1, 100));
        SendMessageW(hTrack, TBM_SETPAGESIZE, 0, 1);
        SendMessageW(hTrack, TBM_SETTICFREQ, 1, 0);
        SendMessageW(hTrack, TBM_SETPOS, FALSE, 50); // Set the cursor at 50 x 10 = 500 ms
        SendMessageW(hTrack, TBM_SETBUDDY, TRUE, (LPARAM)10);
        SendMessageW(hTrack, TBM_SETBUDDY, FALSE, (LPARAM)1000);

        SetTimer(hWnd, IDT_TIMER, DISP_FREQ, (TIMERPROC)NULL);   // refresh display every ~250ms
        EnableWindow(GetDlgItem(hWnd, IDC_STOP_ACQUIRE), false);
        EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), false);
        SetDlgItemText(hWnd, IDC_EDIT1, "Init success");
        bFiltering = false;
        ACQUIRE_FREQ = 250; // Set the initial acquitizion rate

        return 0;

    case WM_PAINT:

        BeginPaint(hWnd, &ps);
        Render(hGraph);
        EndPaint(hWnd, &ps);

        return 0;

    case WM_DESTROY:
    case WM_CLOSE:

        // Terminate the graph and acquisition loop properly

        SendMessage(hWnd, WM_COMMAND, IDC_STOP_ACQUIRE, NULL);

        // Free the graph lib and stop the application

        StopGraph(hGraph);
        if (hGraph)
        {
            FreeGraph(&hGraph);
            hGraph = NULL;
            PostQuitMessage(0);
        }
        return 0;

    case WM_SIZE:
        client_rect.right = LOWORD(lParam);
        client_rect.bottom = HIWORD(lParam);

        // Resize OPENGL Area

        ReshapeGraph(hGraph, 0, 0, client_rect.right, client_rect.bottom - 100);

        // Resize WIN32 Controls   

        percentSPACE = 0;
        for (int i = 0; i <sizeof(sizemap) / sizeof(int) / 2; i++)
        {
            percentSPACE += sizemap[i][1];
        }
        percentSPACE = 100 - percentSPACE;
        percentSPACE /= sizeof(sizemap) / sizeof(int) / 2;

        index = 0;
        itempos = client_rect.right;
        for (int item = IDC_QUIT; item <= IDC_FILTER; item++)
        {
            itempos -= percentSPACE + client_rect.right * sizemap[index][1] / 100;
            hItem = GetDlgItem(hWnd, item);
            GetClientRect(hItem, &item_r);
            SetWindowPos(hItem, NULL, itempos, client_rect.bottom - 30, 0, 0, SWP_NOSIZE);
            index++;
            InvalidateRect(hItem, NULL, false);
        }
        break;

    default:
        break;
    }
    return (INT_PTR)FALSE;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{

    // Setup classic Windows class

    WNDCLASSEX    wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "Test windows";
    if (!RegisterClassEx(&wc))
    {
        MessageBox(0, "Failed To Register The Window Class.", "Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }
    printf("[*] Class registered\n");
    hInst = hInstance;
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_GRAH_DLG), 0, (DLGPROC)WndProc);
    return 0;
}
