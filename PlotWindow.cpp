/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/09/25
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
// 
// PlotWindow.cpp
#include "PlotWindow.h"
#include <cstdio>
#include <format>
#include <print>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "datetimeapi.h"
#include "fileapi.h"

#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "User32.lib")
#pragma comment (lib, "Opengl32.lib")
#pragma comment (lib, "Glu32.lib")

wxBEGIN_EVENT_TABLE(PlotWindow, wxGLCanvas)
EVT_PAINT(PlotWindow::OnPaint)
EVT_SIZE(PlotWindow::OnSize)
wxEND_EVENT_TABLE()

PlotWindow::PlotWindow(wxWindow* parent, int width, int height)
    : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxSize(width, height), wxFULL_REPAINT_ON_RESIZE),
    ctx_(std::make_unique<wxGLContext>(this)),
    gl_initialized_(false),
	WinGraph(parent->GetHWND(), {0, 0, width, height}, MAX_SIGNAL_COUNT, 1000) // Example: 64 signals, buffer size 1000
{
	std::print("[*] PlotWindow created with size {}x{}\\n", width, height);
}

PlotWindow::~PlotWindow()
{
    //if (ctx_)
        //delete ctx_;
}

void PlotWindow::OnPaint(wxPaintEvent& evt)
{
    wxPaintDC dc(this); // obligatoire sous wxWidgets
    SetCurrent(*ctx_);

    if (!gl_initialized_)
        InitGL();

    Render();

    SwapBuffers();
}

void PlotWindow::OnSize(wxSizeEvent& evt)
{
    if (IsShownOnScreen())
    {
        SetCurrent(*ctx_);
        wxSize sz = evt.GetSize();
        glViewport(0, 0, sz.x, sz.y);
		WinGraph::ReshapeGraph(0, 0, sz.x, sz.y);
    }
    evt.Skip();
}

void PlotWindow::InitGL()
{
    // état OpenGL de base
    glClearColor(0.98f, 0.98f, 0.98f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // projection orthographique [-1,1] pour un rendu 2D simple
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gl_initialized_ = true;
}

void PlotWindow::Render()
{
	bool bres = WinGraph::Render(); // delegate
#ifdef TRIANGLE
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Exemple : triangle couleur par sommet (OpenGL legacy)
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.6f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-0.6f, -0.6f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0.6f, -0.6f);
    glEnd();

    // remettez la couleur par défaut si nécessaire
    glColor3f(0.0f, 0.0f, 0.0f);
#endif
}

void CheckErr(void);

void KillFont(GLvoid);
GLvoid glPrint(const char* fmt, ...);

bool FindGlobalMaxScale(double& Xmin, double& Xmax, double& Ymin, double& Ymax);
void DrawWave();
void DrawString(float x, float y, char* string);
void DrawGraphSquare(void);
void DrawGridLines(void);
void DrawCursor(float x, float y);


inline double TakeFiniteNumber(double x);
double FindFirstFiniteNumber(double* tab, int length);
LPSTR dtos(LPSTR str, int len, double value);
double GetStandardizedData(double X, double min, double max);
void normalize_data(double Xmin, double Xmax, double Ymin, double Ymax);
void UpdateBorder();
int GetBufferSize();
bool GetUniqueFilename(char* lpFilename, char* lpFileExtension);


inline long long PerformanceFrequency();
inline long long PerformanceCounter();

void ZeroObject(DATA* pgraph, int iBufferSize);

// Global access

RECT DispArea;									// RECT struct for the OpenGL area dimensions stored in WinProc
GLuint  base;                                   // Base Display List For The Font Set
SIZE dispStringWidth;							// The size  in pixel of "-0.000" displayed on screen
CRITICAL_SECTION cs;							// Sync purpose

int runonce;									// Used by UpdateBorder

// High precision time measurements
long long frequency;
long long start;
long long finish;

/*-------------------------------------------------------------------------
	StartGraph: Setup a new log file and zero memory
  -------------------------------------------------------------------------*/

bool WinGraph::StartGraph(const char* opt_header)
{

	// Sanity check

	if (TRUE == m_graph.bRunning)
	{
		printf("[!] Error at StartGraph() graph already running\n");
		return FALSE;
	}


	EnterCriticalSection(&cs);

	// reset counters and data array of signals

	m_graph.cur_nbpoints = 0;
	m_graph.cur_nbpoints_swap = 0; //when autoscale is off

	for (int index = 0; index < m_graph.signalcount; index++)
	{
		memset(m_graph.signals[index]->X, 0, sizeof(double) * m_graph.BufferSize);
		memset(m_graph.signals[index]->Y, 0, sizeof(double) * m_graph.BufferSize);
		memset(m_graph.signals[index]->Xnorm, 0, sizeof(double) * m_graph.BufferSize);
		memset(m_graph.signals[index]->Ynorm, 0, sizeof(double) * m_graph.BufferSize);
		m_graph.signals[index]->Xmin = 0.0f;
		m_graph.signals[index]->Xmax = 0.0f;
		m_graph.signals[index]->Ymin = 0.0f;
		m_graph.signals[index]->Ymax = 0.0f;
		m_graph.signals[index]->show = true;
	}

	// Save the start time x=0

	frequency = PerformanceFrequency();
	start = PerformanceCounter();

	// Update status -> Graph ON

	m_graph.bRunning = TRUE;


	// reset runonce flag
	runonce = 0;
	LeaveCriticalSection(&cs);
	return TRUE;
}

/*-------------------------------------------------------------------------
	StopGraph: Close the logfile if needed and
	update state flags
  -------------------------------------------------------------------------*/

void WinGraph::StopGraph()
{
	//Close the log file properly

	EnterCriticalSection(&cs);

	// Update status -> Graph OFF

	m_graph.bRunning = FALSE;
	LeaveCriticalSection(&cs);
}

/*-------------------------------------------------------------------------
	FreeGraph: Free every buffer allocated by malloc
	Realease the device context and delete the object
  -------------------------------------------------------------------------*/

WinGraph::~WinGraph()
{
	EnterCriticalSection(&cs);

	wglMakeCurrent(m_graph.hDC, NULL);
	wglDeleteContext(m_graph.hRC);
	ReleaseDC(m_graph.hParentWnd, m_graph.hDC);

	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
}

/*-------------------------------------------------------------------------
	CreateGraph: Initialize the structure, signals,
	OpenGL and critical section. return HGRAPH
  -------------------------------------------------------------------------*/

WinGraph::WinGraph(HWND parent, RECT GraphArea, const int SignalCount, const int BufferSize)
{

	if (SignalCount == 0 || SignalCount > MAX_SIGNAL_COUNT || BufferSize <=0)
	{
		printf("[!] SignalCount not in range in CreateGraph()\n");
		return;
	}

	// Initialyze sync

	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);

	// Allocate each signal buffers on the Heap and fill with zero

	for (int i = 0; i < SignalCount * 2; i++)
	{
		m_graph.signals.push_back(std::make_unique<DATA>(BufferSize));

		//memset(&m_graph.signals[i]->X, 0, sizeof(double) * BufferSize);
		//memset(&m_graph.signals[i]->Y, 0, sizeof(double) * BufferSize);
		//memset(&m_graph.signals[i]->Xnorm, 0, sizeof(double) * BufferSize);
		//memset(&m_graph.signals[i]->Ynorm, 0, sizeof(double) * BufferSize);

		m_graph.signals[i]->Xmax = 0.0;
		m_graph.signals[i]->Xmin = 0.0;
		m_graph.signals[i]->Ymax = 0.0;
		m_graph.signals[i]->Ymin = 0.0;

		m_graph.signals[i]->stat.min_value = 0.0;
		m_graph.signals[i]->stat.average_value_accumulator = 0.0;
		m_graph.signals[i]->stat.average_value_counter = 0;
		m_graph.signals[i]->stat.average_value = 0.0;
		m_graph.signals[i]->stat.max_value = 0.0;

		// set default signal name
		m_graph.signals[i]->signame = std::format("Analog{}", i);
		//snprintf(m_graph.signals[index]->signame.c_str(), sizeof(m_graph.signals[index]->signame) - 1, reinterpret_cast<char*>("Analog%i"), i);

		// set default signal color

		m_graph.signals[i]->color[0] = 0.5f; m_graph.signals[i]->color[1] = 0.05f * i; m_graph.signals[i]->color[2] = 0.01f * i;

		// set default filtering mode

		m_graph.signals[i]->filter = FILTER_NONE;
		m_graph.signals[i]->filter_threshold = 0.5f;

		// Update the number of signal inside object HGRAPH

		m_graph.signalcount++;
	}

	// Hide half of the signals to swap for autoscaling

	m_graph.signalcount = m_graph.signalcount / 2;

	m_graph.totalsignalcount = m_graph.signalcount;
	m_graph.BufferSize = BufferSize;


	// Allocate a temp struct for computing
	// TEMP struct at the end
	m_graph.SnapPlot = std::make_unique<DATA>(BufferSize);


	m_graph.hParentWnd = parent;

	m_graph.scale_factor = 1;
	m_graph.bAutoscale = true;
	m_graph.bDisplayCursor = true;
	m_graph.Logging = LOGGER_NONE;
	m_graph.logfilename = NULL;
	m_graph.Filtering = FILTER_NONE;
	//GetClientRect(m_graph.hGraphWnd, &DispArea);
	//InitGL(DispArea.right, DispArea.bottom);
	//ReshapeGraph(DispArea.left, DispArea.top, DispArea.right, DispArea.bottom);
	LeaveCriticalSection(&cs);
}

/*-------------------------------------------------------------------------
	SetSignalCount: set a new total signal count ;
	must be in range between [0;MAX_SIGNAL_COUNT]
-------------------------------------------------------------------------*/

bool WinGraph::SetSignalCount(const int iSignalNumber)
{
	if (iSignalNumber > MAX_SIGNAL_COUNT)
	{
		std::print("[!] Error at SetSignalCount()\n%i signal max is reached\n", m_graph.signalcount);
		return FALSE;
	}

	m_graph.signalcount = iSignalNumber;
	return TRUE;
}

/*-------------------------------------------------------------------------
	SetSignalLabel: set a name to a signal [0;MAXSIG-1]
-------------------------------------------------------------------------*/

void WinGraph::SetSignalLabel(const char szLabel[260], int iSignalNumber)
{

	if (iSignalNumber < 0 || iSignalNumber >= m_graph.signalcount)
	{
		std::print("[!] Error at SetSignalLabel()\nrange must be [1;%i] and is %i\n", m_graph.signalcount, iSignalNumber);
		return;
	}

	printf("[*] a new signal name is assigned: %s\n", szLabel);
	DATA* signal = (DATA*)&m_graph.signals[iSignalNumber];
	signal->signame = szLabel;
}

/*-------------------------------------------------------------------------
	SetSignalColor: Specify a RGB color [0-255]
-------------------------------------------------------------------------*/

void WinGraph::SetSignalColor(int R, int G, int B, int iSignalNumber)
{
	if (iSignalNumber > MAX_SIGNAL_COUNT - 1)
	{
		printf("[!] Error at SetSignalColor() MAX_SIGNAL_COUNT reached \n");
		return;
	}

	if (R < 0 || R> 255)
	{
		printf("[!] Error at SetSignalColor() R value overflow R:%i\n", R);
		return;
	}

	if (G < 0 || G> 255)
	{
		printf("[!] Error at SetSignalColor() G value overflow G:%i\n", G);
		return;
	}

	if (B < 0 || B> 255)
	{
		printf("[!] Error at SetSignalColor() B value overflow B:%i\n", B);
		return;
	}

	//printf("[*] a new signal color is assigned: RGBf (%i %i %i) at position: %i\n", R, G, B, iSignalNumber);
	DATA* signal = (DATA*)&m_graph.signals[iSignalNumber];
	if (NULL == signal)
	{
		printf("[!] Error at SetSignalColor() graph signal %i is null\n", iSignalNumber);
		return;

	}
	signal->color[0] = (float)R / 255.0f;
	signal->color[1] = (float)G / 255.0f;
	signal->color[2] = (float)B / 255.0f;


	//////////////////////////////////////////////////

	for (int i = 0; i < m_graph.signalcount; i++)
	{
		signal = (DATA*)&m_graph.signals[i];
		//printf("[WINGRAPH]%i %s (%i %i %i)\n", i, signal->signame, (int)(signal->color[0] * 255.0f), (int)(signal->color[1] * 255.0f), (int)(signal->color[2] * 255.0f));
	}
}

/*-------------------------------------------------------------------------
	SetSignalVisible: Enable or disable specific signal on graph
-------------------------------------------------------------------------*/

void WinGraph::SetSignalVisible(bool bDisplay, int iSignalNumber)
{
	if (iSignalNumber < 0 || iSignalNumber >= m_graph.signalcount)
	{
		printf("[!] Error at SetSignalVisible()\nrange must be [1;%i] and is %i\n", m_graph.signalcount, iSignalNumber);
		return;
	}

	DATA* signal = (DATA*)&m_graph.signals[iSignalNumber];
	signal->show = bDisplay;

	printf("[*] Signal visibility changed at signal number: %i\n", iSignalNumber);
}

/*-------------------------------------------------------------------------
	SetRecordingMode: set the graph reccording state with bLogging
  -------------------------------------------------------------------------*/

void WinGraph::SetRecordingMode(LOGGER_M logging)
{
	m_graph.Logging = logging;
}

/*-------------------------------------------------------------------------
	SwapPtr: Swap 2 pointers
  -------------------------------------------------------------------------*/
void inline SwapPtr(void** p1, void** p2)
{
	void* p3 = NULL;
	p3 = *p1; // temporary store
	*p1 = *p2;
	*p2 = p3;
}

/*-------------------------------------------------------------------------
	SwapRecBuffer: Swap first half of the signal with the second half
	- This is used to swap the signal buffer when autoscale is disabled
	- So the logger can continue to log data and add points when only the
	- current point will be displayed.
  -------------------------------------------------------------------------*/
void WinGraph::SwapRecBuffer(bool swap)
{
	// Swap half first signals to half last signals (hidden signals)

	if (swap == false && m_graph.bSwapDone == false)
	{
		// Lock, save and swap

		EnterCriticalSection(&cs); // Lock

		m_graph.cur_nbpoints_swap = m_graph.cur_nbpoints;// Save	
		for (int i = 0; i < m_graph.signalcount; i++)
		{
			DATA* pDATAcurrent = (DATA*)&m_graph.signals[i];
			DATA* pDATAsaved = (DATA*)&m_graph.signals[i + m_graph.signalcount];

			for (int k = 0; k < m_graph.BufferSize; k++)
			{
				pDATAsaved->X[k] = pDATAcurrent->X[k];
				pDATAsaved->Y[k] = pDATAcurrent->Y[k];
				pDATAsaved->Xnorm[k] = pDATAcurrent->Xnorm[k];
				pDATAsaved->Ynorm[k] = pDATAcurrent->Ynorm[k];
			}
			pDATAsaved->signame= pDATAcurrent->signame;
			pDATAsaved->color[0] = pDATAcurrent->color[0];
			pDATAsaved->color[1] = pDATAcurrent->color[1];
			pDATAsaved->color[2] = pDATAcurrent->color[2];
			pDATAsaved->show = pDATAcurrent->show;
			pDATAsaved->filter = pDATAcurrent->filter;
			pDATAsaved->filter_threshold = pDATAcurrent->filter_threshold;
			pDATAsaved->stat = pDATAcurrent->stat; // to check
			pDATAsaved->Xmin = pDATAcurrent->Xmin;
			pDATAsaved->Xmax = pDATAcurrent->Xmax;
			pDATAsaved->Ymin = pDATAcurrent->Ymin;
			pDATAsaved->Ymax = pDATAcurrent->Ymax;
			pDATAsaved->Yaverage = pDATAcurrent->Yaverage;
		}
		for (int i = 0; i < m_graph.signalcount; i++) // Swap
		{
			//std::swap(m_graph.signal[i], m_graph.signal[i + m_graph.signalcount]);
			SwapPtr((void**)&m_graph.signals[i], (void**)&m_graph.signals[i + m_graph.signalcount]);
		}
		m_graph.bSwapDone = true;
		LeaveCriticalSection(&cs); //unlock
	}
	else if (swap == true && m_graph.bSwapDone == true)
	{
		// Lock, save and swap

		EnterCriticalSection(&cs); // Lock

		for (int i = 0; i < m_graph.signalcount; i++) // Swap
		{
			SwapPtr((void**)&m_graph.signals[i], (void**)&m_graph.signals[i + m_graph.signalcount]);
			//std::swap(m_graph.signal[i], m_graph.signal[i + m_graph.signalcount]);
		}
		m_graph.bSwapDone = false;
		LeaveCriticalSection(&cs);
	}



}
/*-------------------------------------------------------------------------
	SetAutoscaleMode: set autoscale
  -------------------------------------------------------------------------*/

void WinGraph::SetAutoscaleMode(bool mode)
{

	m_graph.bAutoscale = mode;

	if (m_graph.bRunning)
	{

		// Swap buffers

		SwapRecBuffer(mode);

		// Save current position in the buffer when stopping autoscale

		if (m_graph.bAutoscale == false)
		{
			m_graph.cur_nbpoints_swap = m_graph.cur_nbpoints;
		}
	}
}

/*-------------------------------------------------------------------------
	SetDisplayCursor: Add indicator bellow the mouse cursor with X and Y values
  -------------------------------------------------------------------------*/

void WinGraph::SetDisplayCursor(bool isActive)
{
	m_graph.bDisplayCursor = isActive;
}

/*-------------------------------------------------------------------------
	SetYminVal: set the Ymin scale value
  -------------------------------------------------------------------------*/

void WinGraph::SetYminVal(double ymin)
{
	MessageBox(0, 0, 0, 0);
}

/*-------------------------------------------------------------------------
	SetYmaxVal: set the Ymin scale value
  -------------------------------------------------------------------------*/

void WinGraph::SetYmaxVal(double ymax)
{
	MessageBox(0, 0, 0, 0);
}

void WinGraph::SetZoomFactor(int zoom)
{
	if (zoom < 0)
	{
		printf("[!] Error at SetZoomFactor() zoom can't be <0\n");
		return;
	}

	if ((m_graph.BufferSize) / zoom < 1.0)
	{
		zoom = 1;
	}

	m_graph.scale_factor = zoom;
	return;
}

/*-------------------------------------------------------------------------
	SetFilteringMode: set the EMA filtering state
  -------------------------------------------------------------------------*/

void WinGraph::SetFilteringMode(FILTER_M filtering, int iSignalNumber)
{
	if (iSignalNumber > MAX_SIGNAL_COUNT || iSignalNumber < 0)
	{
		printf("[!] Error at SetFilteringMode() signal number not in range\n");
		return;
	}

	m_graph.signals[iSignalNumber]->filter = filtering;
}

void WinGraph::SetFilteringThreshold(float intensity, int iSignalNumber)
{
	if (iSignalNumber > MAX_SIGNAL_COUNT || iSignalNumber < 0)
	{
		printf("[!] Error at SetFilteringThreshold() signal number not in range\n");
		return;
	}

	if (intensity > 1 || intensity < 0)
	{
		printf("[!] Error at SetFilteringThreshold() threshold not in range [0;1]\n");
		return;
	}

	m_graph.signals[iSignalNumber]->filter_threshold = intensity;
}

void WinGraph::SetSignalMinValue(int SIGNB, double val)
{
	if (SIGNB > MAX_SIGNAL_COUNT || SIGNB < 0)
	{
		printf("[!] Error at SetSignalMinValue() signal number not in range\n");
		return;
	}

	EnterCriticalSection(&cs);

	if (0 == m_graph.signalcount)
	{
		printf("[!] Error at SetSignalMinValue() graph signal count is 0\n");
		LeaveCriticalSection(&cs);
		return;
	}

	if (m_graph.cur_nbpoints >= 0)
	{
		m_graph.signals[SIGNB]->stat.min_value = val;
		m_graph.signals[SIGNB]->stat.average_value_accumulator = 0.0;
		m_graph.signals[SIGNB]->stat.average_value_counter = 0;
	}
	LeaveCriticalSection(&cs);
}

void WinGraph::SetSignalAverageValue(int SIGNB, double val)
{
	if (SIGNB > MAX_SIGNAL_COUNT || SIGNB < 0)
	{
		printf("[!] Error at SetSignalAverageValue() signal number not in range\n");
		return;
	}

	EnterCriticalSection(&cs);

	if (NULL == m_graph.signalcount)
	{
		printf("[!] Error at SetSignalAverageValue() graph signal count is null\n");
		LeaveCriticalSection(&cs);
		return;
	}

	if (m_graph.cur_nbpoints >= 0)
	{
		m_graph.signals[SIGNB]->stat.average_value = val;
		m_graph.signals[SIGNB]->stat.average_value_accumulator = 0.0;
		m_graph.signals[SIGNB]->stat.average_value_counter = 0;

	}
	LeaveCriticalSection(&cs);
}

void WinGraph::SetSignalMaxValue(int SIGNB, double val)
{
	if (SIGNB > MAX_SIGNAL_COUNT || SIGNB < 0)
	{
		printf("[!] Error at SetSignalMaxValue() signal number not in range\n");
		return;
	}

	EnterCriticalSection(&cs);

	if (0 == m_graph.signalcount)
	{
		printf("[!] Error at SetSignalMaxValue() graph signal count is 0\n");
		LeaveCriticalSection(&cs);
		return;
	}

	if (m_graph.cur_nbpoints >= 0)
	{
		m_graph.signals[SIGNB]->stat.average_value_accumulator = 0.0;
		m_graph.signals[SIGNB]->stat.average_value_counter = 0;
		m_graph.signals[SIGNB]->stat.max_value = val;
	}
	LeaveCriticalSection(&cs);
}

/*-------------------------------------------------------------------------
	GetGraphRC: return the graph state from bRunning
  -------------------------------------------------------------------------*/

bool WinGraph::GetGraphState()
{
	return m_graph.bRunning;
}

/*-------------------------------------------------------------------------
	GetGraphFilename: return the filename used to save data on disk. If not exist return NULL ptr.
  -------------------------------------------------------------------------*/

char* WinGraph::GetGraphFilename()
{
	return m_graph.logfilename;
}


/*-------------------------------------------------------------------------
	GetGraphRC: return the graph render context
  -------------------------------------------------------------------------*/

HGLRC WinGraph::GetGraphRC()
{
	if (NULL == m_graph.hRC)
	{
		printf("[!] Error at GetGraphRC() graph RC is null\n");
		return NULL;
	}
	return m_graph.hRC;
}

/*-------------------------------------------------------------------------
	GetGraphDC: return the graph device context
  -------------------------------------------------------------------------*/

HDC WinGraph::GetGraphDC()
{
	if (NULL == m_graph.hDC)
	{
		printf("[!] Error at GetGraphDC() graph DC is null\n");
		return NULL;
	}

	return m_graph.hDC;
}

/*-------------------------------------------------------------------------
	GetGraphParentWnd: return the graph parent HWND
  -------------------------------------------------------------------------*/

HWND WinGraph::GetGraphParentWnd()
{
	if (NULL == m_graph.hParentWnd)
	{
		printf("[!] Error at GetGraphDC() graph parent windows is null\n");
		return NULL;
	}

	return m_graph.hParentWnd;
}

/*-------------------------------------------------------------------------
	GetGraphWnd: return the graph HWND
  -------------------------------------------------------------------------*/

HWND WinGraph::GetGraphWnd()
{
	if (NULL == m_graph.hGraphWnd)
	{
		printf("[!] Error at GetGraphWnd() parent Windows is null\n");
		return NULL;
	}
	return m_graph.hGraphWnd;
}

/*-------------------------------------------------------------------------
	GetGraphSignalNumber: return the total signals number
  -------------------------------------------------------------------------*/

int WinGraph::GetGraphSignalCount()
{

	if (NULL == m_graph.signalcount)
	{
		printf("[!] Error at GetGraphSignalCount() graph signal count is null\n");
		return -1;
	}

	return m_graph.signalcount;
}

int WinGraph::GetZoomFactor()
{
	return m_graph.scale_factor;
}

/*-------------------------------------------------------------------------
	GetGraphSignalNumber: return the total signals number
  -------------------------------------------------------------------------*/

double WinGraph::GetGraphLastSignalValue(int SIGNB)
{
	EnterCriticalSection(&cs);
	

	if (0 == m_graph.signalcount)
	{
		printf("[!] Error at GetGraphLastSignalValue() graph signal count is 0\n");
		LeaveCriticalSection(&cs);
		return 0.0f;
	}

	if (m_graph.cur_nbpoints > 0)
	{
		LeaveCriticalSection(&cs);
		return m_graph.signals[SIGNB]->Y[m_graph.cur_nbpoints - 1];
	}
	else
	{
		LeaveCriticalSection(&cs);
		return 0.0f;
	}
}

double WinGraph::GetSignalMinValue(int SIGNB)
{
	if (0 == m_graph.signalcount)
	{
		printf("[!] Error at GetSignalMinValue() graph signal count is 0\n");
		return 0.0f;
	}

	if (SIGNB > MAX_SIGNAL_COUNT || SIGNB < 0)
	{
		printf("[!] Error at GetSignalMinValue() signal number not in range\n");
		return 0.0;
	}

	if (m_graph.cur_nbpoints > 0)
	{
		return m_graph.signals[SIGNB]->stat.min_value;
	}
	return 0.0;
}

double WinGraph::GetSignalAverageValue(int SIGNB)
{
	if (0 == m_graph.signalcount)
	{
		printf("[!] Error at GetSignalAverageValue() graph signal count is 0\n");
		return 0.0f;
	}

	if (SIGNB > MAX_SIGNAL_COUNT || SIGNB < 0)
	{
		printf("[!] Error at GetSignalAverageValue() signal number not in range\n");
		return 0.0;
	}

	if (m_graph.cur_nbpoints > 0)
	{
		return m_graph.signals[SIGNB]->stat.average_value;
	}
	return 0.0;
}

double WinGraph::GetSignalMaxValue(int SIGNB)
{
	if (0 == m_graph.signalcount)
	{
		printf("[!] Error at GetSignalMaxValue() graph signal count is 0\n");
		return 0.0f;
	}

	if (SIGNB > MAX_SIGNAL_COUNT || SIGNB < 0)
	{
		printf("[!] Error at GetSignalMaxValue() signal number not in range\n");
		return 0.0;
	}

	if (m_graph.cur_nbpoints > 0)
	{
		return m_graph.signals[SIGNB]->stat.max_value;
	}
	return 0.0;
}

void WinGraph::SignalResetStatisticValue(const int SIGNB)
{
	if (SIGNB > MAX_SIGNAL_COUNT || SIGNB < 0)
	{
		printf("[!] Error at SignalResetStatisticValue() signal number not in range\n");
		return;
	}

	EnterCriticalSection(&cs);

	if (0 == m_graph.signalcount)
	{
		printf("[!] Error at SignalResetStatisticValue() graph signal count is 0\n");
		LeaveCriticalSection(&cs);
		return;
	}

	if (m_graph.cur_nbpoints > 0)
	{
		m_graph.signals[SIGNB]->stat.min_value = m_graph.signals[SIGNB]->Y[m_graph.cur_nbpoints - 1];
		m_graph.signals[SIGNB]->stat.average_value = m_graph.signals[SIGNB]->Y[m_graph.cur_nbpoints - 1];
		m_graph.signals[SIGNB]->stat.average_value_accumulator = 0.0;
		m_graph.signals[SIGNB]->stat.average_value_counter = 0;
		m_graph.signals[SIGNB]->stat.max_value = m_graph.signals[SIGNB]->Y[m_graph.cur_nbpoints - 1];
		printf("[*] SignalResetStatisticValue() min: %lf	avg: %lf	max: %lf\n", m_graph.signals[SIGNB]->stat.min_value, m_graph.signals[SIGNB]->stat.average_value, m_graph.signals[SIGNB]->stat.max_value);
	}
	LeaveCriticalSection(&cs);
}

void WinGraph::ShowDataInConsole()
{
	for (int index = 0; index < m_graph.signalcount; index++)
	{
		if (index == 0)
		{
			printf("\n");
			printf("\n");
			printf("SIG0\n");
			for (int j = 0; j < m_graph.BufferSize; j++)
			{
				printf("%.1lf ", m_graph.signals[index]->X[j]); printf("\t");
				printf("%.1lf ", m_graph.signals[index]->Y[j]); printf("\n");
			}

			printf("SIG4\n");
			for (int j = 0; j < m_graph.BufferSize; j++)
			{
				printf("%.1lf ", m_graph.signals[index + m_graph.signalcount]->X[j]); printf("\t");
				printf("%.1lf ", m_graph.signals[index + m_graph.signalcount]->Y[j]); printf("\n");
			}
		}
	}
	printf("m_graph.cur_nbpoints: %i\n", m_graph.cur_nbpoints);
}
/*-------------------------------------------------------------------------
	AddPoint: Add one POINT for every signal
  -------------------------------------------------------------------------*/
void WinGraph::AddPoint(double* y, const int SignalCount)
{

	if (FALSE == m_graph.bRunning)
	{
		printf("[!] Error at AddPoint() graph not strated\n");
		return;
	}

	// Check if signalcount = length of y!

	if (m_graph.signalcount != SignalCount)
	{
		printf("[!] Error at AddPoint() signalcount not egual to y length\n");
		return;
	}

	EnterCriticalSection(&cs);

	//transfert recording to the second half arrays

	int offset = 0;
	if (m_graph.bAutoscale == false && m_graph.bSwapDone == true)
	{
		offset = m_graph.signalcount;
	}

	// If the maximum points are reached 
	// in the buffer, shift left the array and
	// dec the current number of points

	if (m_graph.cur_nbpoints == m_graph.BufferSize)
	{
		for (int index = 0 + offset; index < m_graph.signalcount + offset; index++)
		{
			for (int j = 0; j < m_graph.BufferSize - 1; j++)
			{
				m_graph.signals[index]->X[j] = m_graph.signals[index]->X[j + 1];																// Shift left X
				m_graph.signals[index]->Y[j] = m_graph.signals[index]->Y[j + 1];																// Shift left Y
			}
		}
		m_graph.cur_nbpoints--;																				// Update the current POINT number
	}

	// Save the actual timestamp

	if (m_graph.cur_nbpoints == 0)
	{
		finish = start;
	}
	else
	{
		finish = PerformanceCounter();
	}

	for (int index = 0 + offset; index < m_graph.signalcount + offset; index++)
	{
		if (NULL == m_graph.signals[index])
		{
			LeaveCriticalSection(&cs);
			printf("[!] Error at AddPoint() data buffer is null\n");
			return;
		}

		// Is filter enable ?

		if (m_graph.signals[index]->filter != FILTER_NONE)
		{
			// Low pass filter 

			if (m_graph.signals[index]->filter == FILTER_EMA)
			{
				//double a = 0.1; // Custom cut freq 
				float a = m_graph.signals[index]->filter_threshold;
				if (m_graph.cur_nbpoints == 0)
					y[index] = y[index]; // First POINT skip to prevent INF
				else
					y[index] = a * y[index] + (1 - a) * m_graph.signals[index]->Y[m_graph.cur_nbpoints - 1];						// Low pass filter EMA "f(x) = x1 * a + (1-a) * x0" where a [0;1]
			}

			// Hanning window filter

			if (m_graph.signals[index]->filter == FILTER_HANNING) // experimental (not properly working)
			{
				const int window_size = 20;																		// Define the Hann windows size here
				static double dataOut[window_size];
				static int accumulator = 0;
				if (accumulator < window_size)
				{
					printf("[*] Hanning filter collecting...\n");
					LeaveCriticalSection(&cs);
					accumulator++;
					return;
				}
				accumulator = 0;

				for (int i = 0; i < window_size; i++)
				{

					const double PI = 3.14159;
					double multiplier = 0.5 * (1 - cos(2 * PI * i / window_size));
					dataOut[i] = multiplier * y[index];
				}
			}

			// Besel filter

			if (m_graph.signals[index]->filter == FILTER_BESEL)
			{
			}
		}

		// Add points to the selected buffer	

		m_graph.signals[index]->X[m_graph.cur_nbpoints] = (double)((finish - start)) / frequency;							// Save in X the elapsed time from start
		m_graph.signals[index]->Y[m_graph.cur_nbpoints] = y[index - offset];															// Save Y

		// Perform some statistics

		// period
		m_graph.signals[index]->stat.period_s = m_graph.signals[index]->X[m_graph.cur_nbpoints] - m_graph.signals[index]->X[0];								// Update current period

		//min
		if (m_graph.signals[index]->Y[m_graph.cur_nbpoints] < m_graph.signals[index]->stat.min_value)
		{
			m_graph.signals[index]->stat.min_value = m_graph.signals[index]->Y[m_graph.cur_nbpoints];											// Update current min value displayed
		}

		//average
		if (m_graph.cur_nbpoints > 0)
		{
			m_graph.signals[index]->stat.average_value_accumulator += m_graph.signals[index]->Y[m_graph.cur_nbpoints];
			m_graph.signals[index]->stat.average_value_counter++;

			m_graph.signals[index]->stat.average_value = m_graph.signals[index]->stat.average_value_accumulator / m_graph.signals[index]->stat.average_value_counter;
		}

		//max
		if (m_graph.signals[index]->Y[m_graph.cur_nbpoints] > m_graph.signals[index]->stat.max_value)
		{
			m_graph.signals[index]->stat.max_value = m_graph.signals[index]->Y[m_graph.cur_nbpoints];											// Update current max value displayed
		}
	}


	// Inc the number of points

	m_graph.cur_nbpoints++;

	LeaveCriticalSection(&cs);

	//ShowDataInConsole(hGraph);

	return;


	///////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////

	EnterCriticalSection(&cs);

	// If the maximum points are reached 
	// in the buffer, shift left the array and
	// dec the current number of points

	if (m_graph.cur_nbpoints == m_graph.BufferSize)
	{
		for (int index = 0; index < m_graph.signalcount; index++)
		{
			for (int j = 0; j < m_graph.BufferSize - 1; j++)
			{
				m_graph.signals[index]->X[j] = m_graph.signals[index]->X[j + 1];																// Shift left X
				m_graph.signals[index]->Y[j] = m_graph.signals[index]->Y[j + 1];																// Shift left Y
			}
		}
		m_graph.cur_nbpoints--;																				// Update the current POINT number
	}
	LeaveCriticalSection(&cs);

	// Save the actual timestamp

	if (m_graph.cur_nbpoints == 0)
	{
		finish = start;
	}
	else
	{
		finish = PerformanceCounter();
	}

	EnterCriticalSection(&cs);
	for (int index = 0; index < m_graph.signalcount; index++)
	{
		if (NULL == m_graph.signals[index])
		{
			LeaveCriticalSection(&cs);
			printf("[!] Error at AddPoint() data buffer is null\n");
			return;
		}

		// Is filter enable ?

		if (m_graph.signals[index]->filter != FILTER_NONE)
		{
			// Low pass filter 

			if (m_graph.signals[index]->filter == FILTER_EMA)
			{
				//double a = 0.1; // Custom cut freq 
				float a = m_graph.signals[index]->filter_threshold;
				if (m_graph.cur_nbpoints == 0)
					y[index] = y[index]; // First POINT skip to prevent INF
				else
					y[index] = a * y[index] + (1 - a) * m_graph.signals[index]->Y[m_graph.cur_nbpoints - 1];						// Low pass filter EMA "f(x) = x1 * a + (1-a) * x0" where a [0;1]
			}

			// Hanning window filter

			if (m_graph.signals[index]->filter == FILTER_HANNING) // experimental (not properly working)
			{
				const int window_size = 20;																		// Define the Hann windows size here
				static double dataOut[window_size];
				static int accumulator = 0;
				if (accumulator < window_size)
				{
					printf("[*] Hanning filter collecting...\n");
					LeaveCriticalSection(&cs);
					accumulator++;
					return;
				}
				accumulator = 0;

				for (int i = 0; i < window_size; i++)
				{

					const double PI = 3.14159;
					double multiplier = 0.5 * (1 - cos(2 * PI * i / window_size));
					dataOut[i] = multiplier * y[index];
				}
			}

			// Besel filter

			if (m_graph.signals[index]->filter == FILTER_BESEL)
			{
			}
		}

		// Add points to the selected buffer	

		m_graph.signals[index]->X[m_graph.cur_nbpoints] = (double)((finish - start)) / frequency;							// Save in X the elapsed time from start
		m_graph.signals[index]->Y[m_graph.cur_nbpoints] = y[index];															// Save Y

		// Perform some statistics

		// period
		m_graph.signals[index]->stat.period_s = m_graph.signals[index]->X[m_graph.cur_nbpoints] - m_graph.signals[index]->X[0];								// Update current period

		//min
		if (m_graph.signals[index]->Y[m_graph.cur_nbpoints] < m_graph.signals[index]->stat.min_value)
		{
			m_graph.signals[index]->stat.min_value = m_graph.signals[index]->Y[m_graph.cur_nbpoints];											// Update current min value displayed
		}

		//average
		if (m_graph.cur_nbpoints > 0)
		{
			m_graph.signals[index]->stat.average_value_accumulator += m_graph.signals[index]->Y[m_graph.cur_nbpoints];
			m_graph.signals[index]->stat.average_value_counter++;

			m_graph.signals[index]->stat.average_value = m_graph.signals[index]->stat.average_value_accumulator / m_graph.signals[index]->stat.average_value_counter;
		}

		//max
		if (m_graph.signals[index]->Y[m_graph.cur_nbpoints] > m_graph.signals[index]->stat.max_value)
		{
			m_graph.signals[index]->stat.max_value = m_graph.signals[index]->Y[m_graph.cur_nbpoints];											// Update current max value displayed
		}

	}
	// Inc the number of points

	m_graph.cur_nbpoints++;

	LeaveCriticalSection(&cs);
}
/*-------------------------------------------------------------------------
	AddMultiplePoints: Add a chunk of data in the graph buffer for each signal count
  -------------------------------------------------------------------------*/
void WinGraph::AddMultiplePoints(double** Chunks, int SignalCount, int BufferLength)
{
	/*
	if (cs.DebugInfo == NULL)
	{
		printf("[!] Error at AddMultiplePoints() critical section not available\n");
		return;
	}
	*/

	// Sanity check

	if (false == m_graph.bRunning)
	{
		printf("[!] Error at AddMultiplePoints() graph not strated\n");
		return;
	}

	// TODO: Check if signalcount = length of y!

	if (m_graph.signalcount != SignalCount)
	{
		printf("[!] Error at AddMultiplePoints() signalcount not egual to y length\n");
		return;
	}

	EnterCriticalSection(&cs);

	// If the maximum points are reached 
	// in the buffer, shift left the array and
	// dec the current number of points

	size_t graph_overflow = m_graph.BufferSize - (m_graph.cur_nbpoints + BufferLength);
	if (graph_overflow <= 0)
	{
		graph_overflow = abs((int)graph_overflow);															//Absolute value

		for (int index = 0; index < m_graph.signalcount; index++)
		{

			for (int j = 0; j < m_graph.BufferSize - graph_overflow; j++)
			{
				m_graph.signals[index]->X[j] = m_graph.signals[index]->X[j + graph_overflow];																// Shift left X
				m_graph.signals[index]->Y[j] = m_graph.signals[index]->Y[j + graph_overflow];																// Shift left Y
			}
		}
		m_graph.cur_nbpoints = m_graph.BufferSize - graph_overflow;
	}
	LeaveCriticalSection(&cs);

	// Save the actual timestamp

	if (m_graph.cur_nbpoints == 0)
	{
		finish = start;
	}
	else
	{
		finish = PerformanceCounter();
	}

	EnterCriticalSection(&cs);
	for (int index = 0; index < m_graph.signalcount; index++)
	{
		if (NULL == m_graph.signals[index])
		{
			LeaveCriticalSection(&cs);
			printf("[!] Error at AddMultiplePoints() data buffer is null\n");
			return;
		}

		// Add points to the selected buffer	
		double x_step = ((double)((finish - start)) / frequency) / BufferLength;
		for (size_t i = 0; i < BufferLength; i++)
		{
			m_graph.signals[index]->X[m_graph.cur_nbpoints + i] = x_step * i;												// Save in X the elapsed time from start
		}

		for (size_t i = 0; i < BufferLength; i++)
		{
			m_graph.signals[index]->Y[m_graph.cur_nbpoints + i] = Chunks[index][i];													// Save Y										
		}


		// Perform some statistics

		// period
		m_graph.signals[index]->stat.period_s = m_graph.signals[index]->X[m_graph.cur_nbpoints] - m_graph.signals[index]->X[0];								// Update current period

		//min
		if (m_graph.signals[index]->Y[m_graph.cur_nbpoints] < m_graph.signals[index]->stat.min_value)
		{
			m_graph.signals[index]->stat.min_value = m_graph.signals[index]->Y[m_graph.cur_nbpoints];											// Update current min value displayed
		}

		//average
		if (m_graph.cur_nbpoints > 0)
		{
			m_graph.signals[index]->stat.average_value_accumulator += m_graph.signals[index]->Y[m_graph.cur_nbpoints];
			m_graph.signals[index]->stat.average_value_counter++;

			m_graph.signals[index]->stat.average_value = m_graph.signals[index]->stat.average_value_accumulator / m_graph.signals[index]->stat.average_value_counter;
		}

		//max
		if (m_graph.signals[index]->Y[m_graph.cur_nbpoints] > m_graph.signals[index]->stat.max_value)
		{
			m_graph.signals[index]->stat.max_value = m_graph.signals[index]->Y[m_graph.cur_nbpoints];											// Update current max value displayed
		}
	}

	// Inc the number of points

	m_graph.cur_nbpoints = m_graph.cur_nbpoints + BufferLength;

	LeaveCriticalSection(&cs);
}
/*-------------------------------------------------------------------------
	Render_: FAKE MOCK
  -------------------------------------------------------------------------*/
bool WinGraph::Render_()
{
	//HDC hDC = GetGraphDC();
	

	//RECT  rc;
	//GetClientRect(m_graph.hGraphWnd, &rc);

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

	//SwapBuffers(hDC);
	return true;
}
/*-------------------------------------------------------------------------
	Render: Analyze the data buffers and print waves to the
	OpenGL device context
  -------------------------------------------------------------------------*/
bool WinGraph::Render()
{
	//GLenum err_code;
	RECT r;
	const int div = 10;
	float txtlen = 0.0;
	float txtheight = 0.0;
	char value[260];
	const char Xname[] = "Time (s)";
	if (!m_graph.SnapPlot)
	{
		std::print("[!] Error at Render() SnapPlot is null\n");
		return false;
	}
	double reelval = m_graph.SnapPlot->Ymin;
	if (cs.DebugInfo == NULL)
	{
		return false;
	}
	EnterCriticalSection(&cs);

	if (m_graph.cur_nbpoints == 0 && start == 0.0f)														// Display a void graph when app start only
	{
		m_graph.SnapPlot->Xmin = 0.0f;
		m_graph.SnapPlot->Xmax = 1.0f;
		m_graph.SnapPlot->Ymin = 0.0f;
		m_graph.SnapPlot->Ymax = 1.0f;
	}
	else if (m_graph.cur_nbpoints > 0)
	{
		UpdateBorder();																			// border determination for each signal: meaning finding X and Y min max values																		
		//ZeroObject(&m_graph.SnapPlot, GetBufferSize());													// Clear SnapPlot
		FindGlobalMaxScale(m_graph.SnapPlot->Xmin, m_graph.SnapPlot->Xmax, m_graph.SnapPlot->Ymin, m_graph.SnapPlot->Ymax);		// Load the Y min and max of all the signals in SnapPlot
		normalize_data(m_graph.SnapPlot->Xmin, m_graph.SnapPlot->Xmax, m_graph.SnapPlot->Ymin, m_graph.SnapPlot->Ymax);			// normalize between [0;1]
	}
	


	// Draw graph frame and grid
	if (GetGraphState() == false)
	{
		printf("Graph state false\n");
	}

	DrawGraphSquare();
	DrawGridLines();

	if (m_graph.SnapPlot->Ymax != m_graph.SnapPlot->Ymin)
	{

		// draw points

		DrawWave();

		// draw indicators

		glColor3f(0.1f, 0.1f, 0.1f);

		// draw zero cursor

		double zero = GetStandardizedData(0, m_graph.SnapPlot->Ymin, m_graph.SnapPlot->Ymax);
		if (zero > 0 && zero < 1)
		{
			DrawString(-0.02f, (float)zero - 0.01f, (char*)"0");
			DrawCursor(0.0f, (float)zero);
		}

		// Determine the length of a typical string for resizing purpose

		if (!GetWindowRect(GetGraphParentWnd(), &r))
		{
			LeaveCriticalSection(&cs);
			return FALSE;
		}

		txtlen = (float)dispStringWidth.cx / r.right; // Normalize the width of the printed value characters between [0-1]
		txtheight = (float)dispStringWidth.cy / r.bottom;

		//Xmin

		DrawString(-txtlen / 1.2f, -0.05f, dtos(value, sizeof(value), m_graph.SnapPlot->Xmin));

		//Xmax

		DrawString(1 - txtlen / 1.2f, -0.05f, dtos(value, sizeof(value), m_graph.SnapPlot->Xmax));

		//Time (s)

		DrawString(0.5f - (txtlen / 2.0f), -0.05f, (char*)Xname);

		// Ymin to Ymax values

		reelval = m_graph.SnapPlot->Ymin;
		for (float ytmp = 0.0f; ytmp <= 1.1f; ytmp += 1.0f / (float)div)
		{
			DrawString(-txtlen * 1.5f, ytmp - ((txtheight * 0.8f) / 2.0f), dtos(value, sizeof(value), reelval));
			reelval += (m_graph.SnapPlot->Ymax - m_graph.SnapPlot->Ymin) / (float)div;
		}
	}


	// Display cursor with values if hoover

	if (m_graph.bDisplayCursor)
	{
		DisplayPointer();

		if (isMouseHover())
		{
			POINT pos = GetMousePosition();
			/////////////////////////////
			RECT client;
			HWND hwndGraph = NULL;
			hwndGraph = GetGraphParentWnd();
			if (GetWindowRect(hwndGraph, &client))
			{
				// Normalization [0-1] on client area
				// Xnorm = (X - min) / (max - min);
				float normal_pos_x = ((float)pos.x - (float)client.left) / ((float)client.right - (float)client.left);
				float normal_pos_y = ((float)pos.y - (float)client.bottom) / ((float)client.top - (float)client.bottom);

				// Normalization [0-1] on graph area
				float normal_pos_x_shifted = ((float)normal_pos_x - (float)0.07) / ((float)0.965 - (float)0.07);
				float normal_pos_y_shifted = ((float)normal_pos_y - (float)0.07) / ((float)0.98 - (float)0.07);

				if ((normal_pos_y_shifted < 1.0f) && (normal_pos_y_shifted > 0.0f))
				{
					if ((normal_pos_x_shifted < 1.0f) && (normal_pos_x_shifted > 0.0f))
					{
						// Display X indicator

						//DrawString(normal_pos_x, normal_pos_y - 0.05, dtos(value, sizeof(value), (normal_pos_x_shifted * (SnapPlot->Xmax - SnapPlot->Xmin)) + SnapPlot->Xmin));

						// Display Y indicator

						double dy = 0.0;
						dy = (m_graph.SnapPlot->Ymin - m_graph.SnapPlot->Ymax) * (1 - normal_pos_y_shifted);
						dy = m_graph.SnapPlot->Ymax + dy;
						DrawString(normal_pos_x + 0.05f, normal_pos_y - 0.05f, dtos(value, sizeof(value), dy));

						//printf("x=%lf	y=%lf	xnorm=%lf	ynorm=%lf\n", normal_pos_x, normal_pos_y, normal_pos_x_shifted, normal_pos_y_shifted);

						// Display lines indicator


						//glColor3f(0.0f, 1.0f, 0.5f);
						//glBegin(GL_LINE_STRIP);
						//glVertex2f(0, 0);
						//glVertex2f(normal_pos_x_shifted, normal_pos_y_shifted);
						//glEnd();

						glColor3f(0.0f, 0.4f, 0.5f);
						glBegin(GL_LINE_STRIP);
						glVertex2f(0, normal_pos_y_shifted);
						glVertex2f(1, normal_pos_y_shifted);
						glEnd();

						glBegin(GL_LINE_STRIP);
						glVertex2f(normal_pos_x_shifted, 0);
						glVertex2f(normal_pos_x_shifted, 1);
						glEnd();

						// OK
						//DrawString(normal_pos_x, normal_pos_y - 0.05, dtos(value, sizeof(value), (normal_pos_x * (SnapPlot->Xmax - SnapPlot->Xmin)) + SnapPlot->Xmin));


						//~OK
						//double dy = 0.0;
						//dy = (SnapPlot->Ymin - SnapPlot->Ymax)* (1-normal_pos_y);
						//dy = SnapPlot->Ymax + dy;
						//printf("dy:%lf, ymin:%lf ymax%lf\n", dy, SnapPlot->Ymin, SnapPlot->Ymax);


						// OK
						//DrawString(normal_pos_x + 0.05, normal_pos_y - 0.05, dtos(value, sizeof(value), dy));
					}
				}
			}
		}
	}
	/*
	if (!SwapBuffers(GetGraphDC()))
	{
		LeaveCriticalSection(&cs);
		return false;
	}
	*/
	LeaveCriticalSection(&cs);
	return TRUE;
}

/*-------------------------------------------------------------------------
	ReshapeGraph: When resize message is proceed update graph pos
  -------------------------------------------------------------------------*/

void WinGraph::ReshapeGraph(int left, int top, int right, int bottom)
{
	//HWND hitem = GetGraphParentWnd();
	//if (SetWindowPos(hitem, NULL, left, top, right, bottom, SWP_SHOWWINDOW))
	//{
		//SetGLView();
		//}
	//else
	//{
	//	printf("    [!] Error at SetWindowPos() with code: 0x%x\n", GetLastError());
	//}

			// Use the Projection Matrix

		glMatrixMode(GL_PROJECTION);
		if (glGetError() == GL_INVALID_ENUM || glGetError() == GL_INVALID_OPERATION)
		{
			LeaveCriticalSection(&cs);
			return;
		}

		// Reset Matrix

		glLoadIdentity();
		if (glGetError() == GL_INVALID_OPERATION)
		{
			LeaveCriticalSection(&cs);
			return;
		}

		// Set the correct perspective.

		const float orthoLeft = -0.08f; const float orthoRight = 1.04f;
		const float orthoBottom = -0.08f; const float orthoTop = 1.02f;
		gluOrtho2D(orthoLeft, orthoRight, orthoBottom, orthoTop);

		GetClientRect(GetGraphParentWnd(), &DispArea);
		glViewport(0, 0, DispArea.right, DispArea.bottom);

		// Use the Model Matrix

		glMatrixMode(GL_MODELVIEW);

		// Reset Matrix

		glLoadIdentity();
		if (glGetError() == GL_INVALID_OPERATION)
		{
			LeaveCriticalSection(&cs);
			return;
		}

		// Clear Color and Depth Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLenum err_code = glGetError();
		if (err_code == GL_INVALID_VALUE || err_code == GL_INVALID_OPERATION)
		{
			std::print("{}\n", (char*)gluErrorString(err_code));
			LeaveCriticalSection(&cs);
			return;
		}
}

/*-------------------------------------------------------------------------
	InitGL: Setup the font used, set the correct OpenGL view at init
  -------------------------------------------------------------------------*/

void WinGraph::InitGL(int Width, int Height)		// Called after the main window is created
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth drawing MSAA...
	glEnable(GL_LINE_SMOOTH);							// Enable Smooth drawing MSAA...
	glEnable(GL_BLEND);
	glDepthMask(false);

	if (!BuildMyFont(L"Verdana", 14))		// Build The Font BuildMyFont(char* FontName, int Fontsize)
	{
		// error on creating the Font
	}
}

/*-------------------------------------------------------------------------
	SetGLView: Make background and check for errors
  -------------------------------------------------------------------------*/

void WinGraph::SetGLView()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // fond sombre
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	CheckErr();
}

/*-------------------------------------------------------------------------
	BuildFont: Create a Windows Bitmap Font to write the device context with
  -------------------------------------------------------------------------*/

bool WinGraph::BuildMyFont(const wchar_t* FontName, const int Fontsize)
{
	HFONT hCustomFont = NULL;									// New font to create
	HFONT hCurrentFont = NULL;									// Store the current Windows font

	// Create an empty display list of 96 char (We are using ASCII char from 32 to 127 only)

	base = glGenLists(96);
	if (0 == base)
	{
		// error when generate the empty list
		printf("[!] Error at BuildMyFont() with code: 0x%x\nerror when generate the empty list\n", GetLastError());
		return FALSE;
	}

	hCustomFont = CreateFont(-1 * Fontsize,						// Font size
		0,														// Width Of Font
		0,														// Angle Of Escapement
		0,														// Orientation Angle
		FW_NORMAL,												// Font Weight
		FALSE,													// Italic
		FALSE,													// Underline
		FALSE,													// Strikeout
		ANSI_CHARSET,											// Character Set Identifier
		OUT_TT_PRECIS,											// Output Precision
		CLIP_DEFAULT_PRECIS,									// Clipping Precision
		ANTIALIASED_QUALITY,									// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,							// Family And Pitch
		FontName);												// Font Name

	if (NULL == hCustomFont)
	{
		// error when creating the font
		printf("[!] Error at BuildMyFont() with code: 0x%x\nerror when creating the font\n", GetLastError());
		return FALSE;
	}

	hCurrentFont = (HFONT)SelectObject(GetGraphDC(), hCustomFont);	// Select the custom Font and store the current font

	if (!wglUseFontBitmaps(GetGraphDC(), 32, 96, base))				// Builds 96 Characters Starting At Character 32 and store it in the list
	{
		// error when loading the font
		printf("[!] Error at BuildMyFont() with code: 0x%x\nerror when loading the font\n", GetLastError());
		return FALSE;
	}
	const char text[] = "-0.0000";
	SetTextCharacterExtra(GetGraphDC(), 1);
	GetTextExtentPoint32A(GetGraphDC(), text, strlen(text), &dispStringWidth);
	dispStringWidth.cx -= GetTextCharacterExtra(GetGraphDC()) * (strlen(text) - 2);

	SelectObject(GetGraphDC(), hCurrentFont);							// restore the initial Font
	DeleteObject(hCustomFont);												// We don't need the Custom Font anymore as we populate the list and load it in OpenGL
	return TRUE;
}

/*-------------------------------------------------------------------------
	KillFont: Free the font from OpenGL
  -------------------------------------------------------------------------*/

void KillFont(GLvoid)														// Delete The Font List
{
	glDeleteLists(base, 96);												// Delete All 96 Characters ( NEW )
}

/*-------------------------------------------------------------------------
	glPrint: TBD
  -------------------------------------------------------------------------*/
  //https://nehe.gamedev.net/tutorial/bitmap_fonts/17002/

GLvoid glPrint(const char* fmt, ...)										// Custom GL "Print" Routine
{
	char        text[256];													// Holds Our String
	va_list     ap;															// Pointer To List Of Arguments

	if (fmt == NULL)														// If There's No Text
		return;																// Do Nothing

	va_start(ap, fmt);														// Parses The String For Variables
	vsprintf_s(text, fmt, ap);												// And Converts Symbols To Actual Numbers
	va_end(ap);																// Results Are Stored In Text
	glPushAttrib(GL_LIST_BIT);												// Pushes The Display List Bits     ( NEW )
	glListBase(base - 32);													// Sets The Base Character to 32    ( NEW )
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);						// Draws The Display List Text  ( NEW )
	glPopAttrib();															// Pops The Display List Bits   ( NEW )
}

/*-------------------------------------------------------------------------
	FindGlobalMaxScale: Scanning arrays to determine the border
  -------------------------------------------------------------------------*/

bool WinGraph::FindGlobalMaxScale(double& Xmin, double& Xmax, double& Ymin, double& Ymax)
{
	if (0 != m_graph.signalcount)
	{
		int first_sig = 0;
		for (int index = 0; index < m_graph.signalcount; index++)			// find first displayed signal (show=true)
		{
			if (m_graph.signals[index]->show == false)
			{
				continue;
			}
			else
			{
				first_sig = index;											// save first signal number
				break;
			}
		}

		Xmin = m_graph.signals[first_sig]->Xmin;										// save first value
		Xmax = m_graph.signals[first_sig]->Xmax;										// save first value	
		Ymin = m_graph.signals[first_sig]->Ymin;										// save first value
		Ymax = m_graph.signals[first_sig]->Ymax;										// save first value

		for (int index = first_sig + 1; index < m_graph.signalcount; index++)			// Iterate every signals
		{
			if (m_graph.signals[index]->show == true)						// Update limit only if signal displayed
			{
				if (Xmin > m_graph.signals[index]->Xmin)
					Xmin = m_graph.signals[index]->Xmin;							// Update if needed

				if (Xmax < m_graph.signals[index]->Xmax)
					Xmax = m_graph.signals[index]->Xmax;							// Update if needed

				if (Ymin > m_graph.signals[index]->Ymin)
					Ymin = m_graph.signals[index]->Ymin;							// Update if needed

				if (Ymax < m_graph.signals[index]->Ymax)
					Ymax = m_graph.signals[index]->Ymax;							// Update if needed
			}
		}

		// prevent div by 0

		if ((Ymax - Ymin) == 0)
		{
			Ymax += 0.5;
			Ymin -= 0.5;
		}

		// Optimize display with rounded units

		const double rangeMult[] = { /*0.1, 0.2, 0.25, 0.5, 2.5,*/10.0, 1.0, 2.0, 5.0 };
		const int segnumber = 10;
		double magnitude = floor(log10(Ymax - Ymin));

		for (int i = 0; i < sizeof(rangeMult) / sizeof(double); i++)
		{
			double step_size = rangeMult[i] * pow(10, magnitude);
			double low = floor(Ymin / step_size) * step_size;
			double high = ceil(Ymax / step_size) * step_size;

			double segment = round((high - low) / step_size);

			if (segment <= segnumber)
			{
				Ymax = high;
				Ymin = low;
				return TRUE;
			}
		}
		printf("[!] Error at FindGlobalMaxScale()\nimpossible to calculate good proportion with the algorythm\n");
		return FALSE;

	}
	printf("[!] Error at FindGlobalMaxScale()\nsignal count  = 0\n");
	return FALSE;
}



/*-------------------------------------------------------------------------
	DrawWave: Compute every signals for display
  -------------------------------------------------------------------------*/

void WinGraph::DrawWave()
{

	// Select last point in the buffer when autoscale is running or not
	int last_point = m_graph.cur_nbpoints;
	if (m_graph.bAutoscale == false)
	{
		// Save current position in the buffer
		last_point = m_graph.cur_nbpoints_swap;
	}

	if (last_point > 0)
	{
		glLineWidth(1.5);

		for (int index = 0; index < m_graph.signalcount; index++)
		{
			if (m_graph.signals[index]->show == false)																	// Skip signal display if requiered
			{
				continue;
			}
			glColor3f(m_graph.signals[index]->color[0], m_graph.signals[index]->color[1], m_graph.signals[index]->color[2]);								// Colors of the signal
			glBegin(GL_LINE_STRIP);

			int first_point = last_point - (last_point / GetZoomFactor());	// first_point [0-last_point]
			if (first_point < 0)
			{
				first_point = 0;
			}

			for (int i = first_point; i < last_point; i++)	// Handle zoom here
			{
				//printf("\n [!] X%i = %lf \n", i, TakeFiniteNumber(m_graph.signals[index]->Xnorm[i]));

				// prevent NAN

				if (m_graph.signals[index]->Xnorm[i] != m_graph.signals[index]->Xnorm[i] || m_graph.signals[index]->Ynorm[i] != m_graph.signals[index]->Ynorm[i])
					continue;

				glVertex2f(TakeFiniteNumber(m_graph.signals[index]->Xnorm[i]), TakeFiniteNumber(m_graph.signals[index]->Ynorm[i]));		// Create GPU wave
			}
			glEnd();
		}
	}
}

/*-------------------------------------------------------------------------
	DrawString: Display character in OpenGL
  -------------------------------------------------------------------------*/

void DrawString(float x, float y, char* string)
{
	glRasterPos2f(x, y);
	glPrint(string);  // Print GL Text To The Screen
}

/*-------------------------------------------------------------------------
	DrawGraphSquare: Make the graph boxing here
  -------------------------------------------------------------------------*/

void DrawGraphSquare(void)
{
	// Set boxing

	glLineWidth(3);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2i(0, 0);
	glVertex2i(1, 0);
	glVertex2i(1, 1);
	glVertex2i(0, 1);
	glVertex2i(0, 0);
	glEnd();

	// Set colored font rectangle

	glBegin(GL_POLYGON);
	glColor3f(0.988f, 0.988f, 0.988f);
	glVertex2i(0, 0);

	glColor3f(0.888f, 0.888f, 0.890f);
	glVertex2i(0, 1);

	glColor3f(0.888f, 0.888f, 0.890f);
	glVertex2i(1, 1);

	glColor3f(0.988f, 0.988f, 0.988f);
	glVertex2i(1, 0);

	glEnd();
}

/*-------------------------------------------------------------------------
	DrawGridLines: draw a grid in the square
  -------------------------------------------------------------------------*/

void DrawGridLines(void)
{
	// Draw fine grid 

	glLineWidth(0.5);
	glColor3f(0.8F, 0.8F, 0.8F);
	glBegin(GL_LINES);
	for (float xtmp = 0.0f; xtmp < 1.0f; xtmp += 0.05f)
	{
		glVertex2f(xtmp, 0.0);
		glVertex2f(xtmp, 1.0);
		glVertex2f(0.0, xtmp);
		glVertex2f(1.0, xtmp);
	}
	glEnd();

	//Draw Grid 

	glLineWidth(1);
	glColor3f(0.5F, 0.5F, 0.5F);
	glBegin(GL_LINES);
	for (float xtmp = 0.0f; xtmp < 1.0f; xtmp += 0.20f)
	{
		glVertex2f(xtmp, 0.0);
		glVertex2f(xtmp, 1.0);
		glVertex2f(0.0, xtmp);
		glVertex2f(1.0, xtmp);
	};
	glEnd();
}

/*-------------------------------------------------------------------------
	DrawCursor: draw a moveable triangle on the graph
  -------------------------------------------------------------------------*/

void DrawCursor(float x, float y)
{
	glLineWidth(2.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(x - 0.01f, y + 0.01f);
	glVertex2f(x, y);
	glVertex2f(x - 0.01f, y - 0.01f);
	glEnd();
}

/*-------------------------------------------------------------------------
	TakeFiniteNumber: Ensure to return a reel value as the graph can't plot
	+-INF value. A rounding happen here
  -------------------------------------------------------------------------*/

inline double TakeFiniteNumber(double x)
{
	// used to prevent -INF && +INF for computation
	// always return a real value closest to the limit

	if (x <= -DBL_MAX)
	{
		return -DBL_MAX;
	}
	if (x >= +DBL_MAX)
	{
		return DBL_MAX;
	}
	return x;
}

/*-------------------------------------------------------------------------
	FindFirstFiniteNumber: Iterate the array and returning the first reel
	finite number
  -------------------------------------------------------------------------*/

double FindFirstFiniteNumber(double* tab, int length)
{
	int i = 0;
	do
	{
		if (tab[i] == tab[i])
		{
			return tab[i];
		}
		i++;
	} while (i <= length);
	return 0;
}

/*-------------------------------------------------------------------------
	ftos: format a float value to a str representation
  -------------------------------------------------------------------------*/

LPSTR dtos(LPSTR str, int len, double value)
{
	if (value < 1.0 && value > -1.0)
	{
		sprintf_s(str, len, "%.2lf", value);
	}
	else if (value < 10.0 && value > -10.0)
	{
		sprintf_s(str, len, "%.2lf", value);
	}
	else
	{
		sprintf_s(str, len, "%.1lf", value);
	}

	return str;
}

/*-------------------------------------------------------------------------
	GetStandardizedData: return the reel value from
	the normalized data space [0;1]
  -------------------------------------------------------------------------*/

double GetStandardizedData(double X, double min, double max)
{
	double ret;
	ret = (X - min) / (max - min);
	// initial value = (max - X) / (max - min);
	return ret;
}

/*-------------------------------------------------------------------------
	normalize_data: Update the DATA object. The datas will
	be set between the range of 0 and 1 after executing.
  -------------------------------------------------------------------------*/

void WinGraph::normalize_data(double Xmin, double Xmax, double Ymin, double Ymax)
{


	for (int index = 0; index < m_graph.signalcount; index++)
	{
		if (m_graph.signals[index]->show == true)											// Show signal enable?
		{
			int min_point = 0;
			int zoom = GetZoomFactor();
			if (zoom > 1)
			{
				min_point = m_graph.cur_nbpoints - (m_graph.cur_nbpoints / zoom);
			}

			for (int x = min_point; x < m_graph.cur_nbpoints; x++)
			{

				// prevent Nan numbers

				if (m_graph.signals[index]->Y[x] != m_graph.signals[index]->Y[x] || Xmax == Xmin)
					continue;

				// Xnorm = (X - min) / (max - min);

				m_graph.signals[index]->Xnorm[x] = (m_graph.signals[index]->X[x] - Xmin) / (Xmax - Xmin);
				m_graph.signals[index]->Ynorm[x] = (m_graph.signals[index]->Y[x] - Ymin) / (Ymax - Ymin);

				//printf("\n [!] X%i = %lf \n", x, TakeFiniteNumber(m_graph.signal[index]->Xnorm[x]));

				// prevent Nan numbers in normalized buffer

				////////////////////////////////////////////////////////////////////////////////!
				// Error prone = 0.0 in place of +inf or -inf
				if (m_graph.signals[index]->Ynorm[x] != m_graph.signals[index]->Ynorm[x])
					m_graph.signals[index]->Y[x] = 0.0;

			}
		}
	}
}

/*-------------------------------------------------------------------------
	UpdateBorder: Update the min and max value of every signal object
	in the struct.
  -------------------------------------------------------------------------*/
void WinGraph::UpdateBorder()
{
	static int AnalizedPts;
	static int CurrentPoint = 0;
	/// ////////////////////////////////////////////
	static int xmaxpos;
	static int yminpos;
	static int ymaxpos;

	// Runonce

	if (0 == runonce)
	{
		AnalizedPts = 0;
		xmaxpos = 0;
		yminpos = 0;
		ymaxpos = 0;
		runonce++;
	}
	//fprintf("\rAnalizedPts%i xmax%i ymin%i ymax%i", AnalizedPts, xmaxpos, yminpos, ymaxpos);
	if (yminpos < 0 || ymaxpos < 0 || yminpos > m_graph.cur_nbpoints / 20 || ymaxpos > m_graph.cur_nbpoints / 20)
	{
		// Get scale factor position
		AnalizedPts = m_graph.cur_nbpoints - (m_graph.cur_nbpoints / GetZoomFactor());

		//AnalizedPts = 0;
		for (int index = 0; index <= m_graph.signalcount - 1; index++)
		{
			if (m_graph.signals[index]->show == false)
			{
				continue;
			}
			m_graph.signals[index]->Xmin = TakeFiniteNumber(m_graph.signals[index]->X[AnalizedPts]);
			m_graph.signals[index]->Xmax = TakeFiniteNumber(m_graph.signals[index]->X[AnalizedPts]);
			//m_graph.BufferSize = 50;

			if (m_graph.bAutoscale == TRUE)
			{
				m_graph.signals[index]->Ymin = TakeFiniteNumber(m_graph.signals[index]->Y[AnalizedPts]);
				m_graph.signals[index]->Ymax = TakeFiniteNumber(m_graph.signals[index]->Y[AnalizedPts]);
			}
			else
			{
				MessageBox(0, 0, 0, 0);
			}

		}
	}

	/// ////////////////////////////////////////////

	if (m_graph.cur_nbpoints == 0)
	{
		for (int index = 0; index <= m_graph.signalcount - 1; index++)
		{
			m_graph.signals[index]->Xmin = 0.0f;
			m_graph.signals[index]->Xmax = 0.0f;
			m_graph.signals[index]->Ymin = 0.0f;
			m_graph.signals[index]->Ymax = 0.0f;
		}
		return;
	}

	if (m_graph.cur_nbpoints == 1)
	{
		for (int index = 0; index <= m_graph.signalcount - 1; index++)
		{
			if (m_graph.signals[index]->show == false)
			{
				continue;
			}
			m_graph.signals[index]->Xmin = TakeFiniteNumber(m_graph.signals[index]->X[AnalizedPts]);
			m_graph.signals[index]->Xmax = TakeFiniteNumber(m_graph.signals[index]->X[AnalizedPts]);
			m_graph.signals[index]->Ymin = TakeFiniteNumber(m_graph.signals[index]->Y[AnalizedPts]);
			m_graph.signals[index]->Ymax = TakeFiniteNumber(m_graph.signals[index]->Y[AnalizedPts]);
		}
	}

	if (m_graph.cur_nbpoints > 1)
	{
		for (int index = 0; index <= m_graph.signalcount - 1; index++)
		{
			if (m_graph.signals[index]->show == false)
			{
				continue;
			}
			CurrentPoint = AnalizedPts;

			for (CurrentPoint; CurrentPoint < m_graph.cur_nbpoints; CurrentPoint++)
			{
				////////////////////////////////////////////
				int zoom = GetZoomFactor();
				if (zoom > 1)
				{
					zoom = m_graph.cur_nbpoints - (m_graph.cur_nbpoints / zoom);

				}
				else
				{
					zoom = 0;
				}

				m_graph.signals[index]->Xmin = TakeFiniteNumber(m_graph.signals[index]->X[zoom]);
				//m_graph.signal[index]->Xmin = TakeFiniteNumber(m_graph.signal[index]->X[0]);

				////////////////////////////////////////////
				if (TakeFiniteNumber(m_graph.signals[index]->X[CurrentPoint]) > m_graph.signals[index]->Xmax)
				{
					m_graph.signals[index]->Xmax = TakeFiniteNumber(m_graph.signals[index]->X[CurrentPoint]);
					xmaxpos = CurrentPoint;
				}

				if (TakeFiniteNumber(m_graph.signals[index]->Y[CurrentPoint]) < m_graph.signals[index]->Ymin)
				{
					m_graph.signals[index]->Ymin = TakeFiniteNumber(m_graph.signals[index]->Y[CurrentPoint]);
					yminpos = CurrentPoint;
				}

				if (TakeFiniteNumber(m_graph.signals[index]->Y[CurrentPoint]) > m_graph.signals[index]->Ymax)
				{
					m_graph.signals[index]->Ymax = TakeFiniteNumber(m_graph.signals[index]->Y[CurrentPoint]);
					ymaxpos = CurrentPoint;
				}
			}
		}
	}

	AnalizedPts = m_graph.cur_nbpoints - 1;
	if (m_graph.cur_nbpoints == m_graph.BufferSize)
	{
		AnalizedPts--;
	}
	/// ////////////////////////////////////////////
	xmaxpos--;
	yminpos--;
	ymaxpos--;
	/// ////////////////////////////////////////////
}

/*-------------------------------------------------------------------------
	ZeroObject: Reset DATA structure to 0
-------------------------------------------------------------------------*/

void ZeroObject(DATA* pgraph, int iBufferSize)
{
	// Sanity check

	if (NULL == pgraph)
	{
		printf("[!] Error at ZeroObject() pData ptr is null\n");
		return;
	}

	memset(&pgraph->X, 0, sizeof(double) * iBufferSize);
	memset(&pgraph->Y, 0, sizeof(double) * iBufferSize);
	memset(&pgraph->Xnorm, 0, sizeof(double) * iBufferSize);
	memset(&pgraph->Ynorm, 0, sizeof(double) * iBufferSize);
	pgraph->Xmin = 0.0f;
	pgraph->Xmax = 0.0f;
	pgraph->Ymin = 0.0f;
	pgraph->Ymax = 0.0f;
}

/*-------------------------------------------------------------------------
	GetBufferSize: return the total data buffer size
-------------------------------------------------------------------------*/

int WinGraph::GetBufferSize()
{
	return m_graph.BufferSize;
}

/*-------------------------------------------------------------------------
	GetUniqueFilename: return an unique filename for logging purpose
-------------------------------------------------------------------------*/

bool GetUniqueFilename(wchar_t* lpFilename, wchar_t* lpFileExtension)
{
	SYSTEMTIME t;

	// get daily filename

	wchar_t lpDateStr[MAX_PATH];
	wchar_t name_format[32];
	if (wcslen(lpFileExtension) > 10)
	{
		printf("[!] Error at GetUniqueFilename() lpFileExtension > 10 char (forbiden)\n");
		return FALSE;
	}
	swprintf_s(name_format, L"yyyy_MM_dd_%s", lpFileExtension);
	GetLocalTime(&t);
	int ok = GetDateFormat(LOCALE_USER_DEFAULT,
		0,
		&t,
		name_format,
		lpDateStr,
		sizeof(lpDateStr));

	wchar_t lpBaseFilename[MAX_PATH];
	wcscpy_s(lpBaseFilename, _countof(lpDateStr), lpDateStr);
	wcscpy_s(lpFilename, _countof(lpBaseFilename), lpBaseFilename);

	int i = 0;
try_next_file:
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(
		lpFilename,
		&FindFileData
	);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("[*] Make unique filename successfull: %ws\n", lpFilename);
		return TRUE;
	}

	// Build (0) str
	wchar_t temp[6];
	swprintf_s(temp, _countof(temp), L"(%i)", i);

	// Reset lpFilename with base file
	wcscpy_s(lpFilename, _countof(lpBaseFilename), lpBaseFilename);

	// Remove extension .lab
	int cut_offset = wcslen(lpFilename) - wcslen(lpFileExtension);
	lpFilename[cut_offset] = '\0';

	// Add (0)
	wcscat_s(lpFilename, _countof(lpBaseFilename), temp);

	// Add extension
	wcscat_s(lpFilename, _countof(lpBaseFilename), lpFileExtension);

	// Inc counter
	i++;

	// Max filenumber = 999
	if (i < 999)
		goto try_next_file;


	printf("[!] Make unique filename failed\n");
	return FALSE;
}

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

/*-------------------------------------------------------------------------
	CheckErr: try to catch openGL error messages
  -------------------------------------------------------------------------*/

void CheckErr(void)
{
	GLenum err = 0;
	err = glGetError();
	switch (err)
	{
	case GL_NO_ERROR:
		printf("	[!] GL_NO_ERROR\n");
		break;

	case GL_INVALID_ENUM:
		printf("	[!] GL_INVALID_ENUM");
		break;

	case GL_INVALID_VALUE:
		printf("	[!] GL_INVALID_VALUE");
		break;

	case GL_INVALID_OPERATION:
		printf("	[!] GL_INVALID_OPERATION");
		break;

	case GL_STACK_OVERFLOW:
		printf("	[!] GL_STACK_OVERFLOW");
		break;

	case GL_STACK_UNDERFLOW:
		printf("	[!] GL_STACK_UNDERFLOW");
		break;

	case GL_OUT_OF_MEMORY:
		printf("	[!] GL_OUT_OF_MEMORY");
		break;

	default:
		break;
	}
}


POINT WinGraph::GetMousePosition(void)
{
	POINT POINT;
	if (GetCursorPos(&POINT) == FALSE)
		return { 0, 0 };

	return POINT;
}

bool WinGraph::isMouseHover()
{
	POINT POINT;
	if (GetCursorPos(&POINT) == FALSE)
		return FALSE;

	RECT client;
	HWND hwndGraph = NULL;
	hwndGraph = GetGraphParentWnd();
	if (hwndGraph)
	{
		if (IsWindowVisible(hwndGraph))
		{
			if (GetWindowRect(hwndGraph, &client))
			{
				//printf("\t%i>%i--%i<%i--%i<%i--%i>%i--\n", POINT.x, client.left, POINT.x, client.right, POINT.y, client.bottom, POINT.y, client.top);
				if (((POINT.x > client.left) && (POINT.x < client.right)) && ((POINT.y < client.bottom) && (POINT.y > client.top)))
				{
					//printf("Hoover\n");
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

void WinGraph::DisplayPointer()
{
	if (isMouseHover())
	{
	}
}
