/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/09/25
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
// 
// // PlotWindow.h
#pragma once
#include <string>
#include "math.h"
#include <memory>
#include <vector>
#include <wx/wx.h>
#include <wx/glcanvas.h>
#ifdef WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#endif

const enum { MAX_SIGNAL_COUNT = 64 };

enum FILTER_M {
	FILTER_NONE = 100,
	FILTER_HANNING,
	FILTER_BESEL,
	FILTER_EMA
};

enum LOGGER_M {
	LOGGER_NONE = 200,
	LOGGER_ASCII,
	LOGGER_TDMS,
	LOGGER_XLSX
};

struct DATA_STATISTIC {
	double period_s;
	double min_value;
	double max_value;
	int average_value_counter;
	double average_value_accumulator;
	double average_value;
};

struct DATA{
public:
	~DATA() { delete[] X; delete[] Y; delete[] Xnorm; delete[] Ynorm; };
	DATA(size_t array_length) {
		X = new double[array_length];
		Y = new double[array_length];
		Xnorm = new double[array_length];
		Ynorm = new double[array_length];
		show = true;
		filter = FILTER_M::FILTER_NONE;
		filter_threshold = 0.0f;
		stat.period_s = 0.0;
		stat.min_value = DBL_MAX;
		stat.max_value = -DBL_MAX;
		stat.average_value_counter = 0;
		stat.average_value_accumulator = 0.0;
		stat.average_value = 0.0;
		Xmin = 0.0;
		Xmax = 0.0;
		Ymin = 0.0;
		Ymax = 0.0;
		Yaverage = 0.0;
	}
	std::string signame;
	float color[3] = { 0 };
	bool show;
	FILTER_M filter;
	float filter_threshold;
	DATA_STATISTIC stat;
	double* X;
	double* Y;
	double* Xnorm;
	double* Ynorm;
	double Xmin;
	double Xmax;
	double Ymin;
	double Ymax;
	double Yaverage;
};

#pragma warning(disable : 4200)					// Disable warning: DATA* signal[] -> Array size [0], See CreateGraph for signal allocation specifics
struct GRAPHSTRUCT {
	HWND hParentWnd;							// Parent handle of the object
	HWND hGraphWnd;								// Graph handle
	HDC hDC;									// OpenGL device context
	HGLRC hRC;									// OpenGL rendering context
	int totalsignalcount;						// Total signals in the struct
	int signalcount;							// signals in use in the struct
	int cur_nbpoints;							// Current total points in the arrays
	int cur_nbpoints_swap;						// Current points saved when swaping buff
	int BufferSize;								// The total amount of POINT to handle							
	bool bRunning;								// Status of the graph
	LOGGER_M Logging;							// Logging type
	char* logfilename;
	FILTER_M Filtering;							// Filtering type
	bool bAutoscale;							// Autoscale active
	bool bSwapDone;							// Autoscale active
	bool bDisplayCursor;						// Logging active
	int scale_factor;							// Fix the X scale factor (zoom)
	std::vector<std::unique_ptr<DATA>> signals;	// all working signals here
	std::unique_ptr<DATA> SnapPlot;								// SnapPlot: work with temp data on signals[], used to convert standard values to normalized values
};					

class WinGraph
{
public:
	WinGraph(HWND parent, RECT GraphArea, const int SignalCount, const int BufferSize);
	~WinGraph();
	bool StartGraph(const char* opt_header);
	void StopGraph();
	void FreeGraph();
	bool SetSignalCount(const int iSignalNumber);
	void SetSignalLabel(const char szLabel[260], const int iSignalNumber);
	void SetSignalColor(const int R, const int G, const int B, const int iSignalNumber);
	void SetSignalVisible(bool bDisplay, const int iSignalNumber);
	void SetRecordingMode(LOGGER_M logging);
	void SwapRecBuffer(bool swap);
	void SetAutoscaleMode(bool mode);
	void SetDisplayCursor(bool isActive);
	void SetYminVal(double ymin); // Set the Y min axe value of the graph, autoscale clear this value when enable
	void SetYmaxVal(double ymax); // Set the Y max axe value of the graph, autoscale clear this value when enable
	void SetZoomFactor(int zoom); // Set the X scale factor value of the graph
	void SetFilteringMode(FILTER_M filtering, const int iSignalNumber);
	void SetFilteringThreshold(const float intensity, const int iSignalNumber);
	void SetSignalMinValue(const int SIGNB, double val);
	void SetSignalAverageValue(const int SIGNB, double val);
	void SetSignalMaxValue(const int SIGNB, double val);
	bool GetGraphState();
	char* GetGraphFilename();
	HGLRC GetGraphRC();
	HDC GetGraphDC();
	HWND GetGraphParentWnd();
	HWND GetGraphWnd();
	int GetGraphSignalCount();
	int GetZoomFactor();
	double GetGraphLastSignalValue(const int SIGNB);
	double GetSignalMinValue(const int SIGNB);
	double GetSignalAverageValue(const int SIGNB);
	double GetSignalMaxValue(const int SIGNB);
	void SignalResetStatisticValue(const int SIGNB);
	void ShowDataInConsole();
	void AddPoint(double* y, const int SignalCount);
	void AddMultiplePoints(double** Chunks, const int SignalCount, const int BufferLength);
	bool Render_();
	bool Render();
	void ReshapeGraph(int left, int top, int right, int bottom);
	void InitGL(const int Width, const int Height);
	void SetGLView();
	bool BuildMyFont(const wchar_t* FontName, const int Fontsize);
	bool FindGlobalMaxScale(double& Xmin, double& Xmax, double& Ymin, double& Ymax);
	void DrawWave();
	void normalize_data(double Xmin, double Xmax, double Ymin, double Ymax);
	void UpdateBorder();
	int GetBufferSize();
	//mouse
	POINT GetMousePosition(void);
	bool isMouseHover();
	void DisplayPointer();
private:
	GRAPHSTRUCT m_graph;
};

class PlotWindow : public wxGLCanvas, public WinGraph
{
public:
	PlotWindow(wxWindow* parent, int width = 600, int height = 600);
    ~PlotWindow() override;

	void OnSize(wxSizeEvent& evt);
	void Render();
private:
    void OnPaint(wxPaintEvent& evt);

    void InitGL();
    

    std::unique_ptr<wxGLContext> ctx_;
    bool gl_initialized_;

    wxDECLARE_EVENT_TABLE();
};

