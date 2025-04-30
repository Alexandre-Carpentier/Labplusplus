/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <windows.h>

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

// Public declaration goes here
typedef void* HGRAPH;
#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport)bool StartGraph(HGRAPH hGraph, const char* opt_header);
	__declspec(dllexport)void StopGraph(HGRAPH hGraph);
	__declspec(dllexport)void FreeGraph(HGRAPH* hGraph);
	__declspec(dllexport)HGRAPH CreateGraph(HWND hWnd, RECT GraphArea, const int SignalCount, const int BufferSize);
	__declspec(dllexport)bool SetSignalCount(HGRAPH hGraph, const int iSignalNumber);
	__declspec(dllexport)void SetSignalLabel(HGRAPH hGraph, const char szLabel[260], const int iSignalNumber);
	__declspec(dllexport)void SetSignalColor(HGRAPH hGraph, const int R, const int G, const int B, const int iSignalNumber);
	__declspec(dllexport)void SetSignalVisible(HGRAPH hGraph, bool bDisplay, const int iSignalNumber);
	__declspec(dllexport)void SetRecordingMode(HGRAPH hGraph, LOGGER_M logging);
	__declspec(dllexport)void SetAutoscaleMode(HGRAPH hGraph, bool mode);
	__declspec(dllexport)void SetDisplayCursor(HGRAPH hGraph, bool isActive);
	__declspec(dllexport)void SetYminVal(HGRAPH hGraph, double ymin); // Set the Y min axe value of the graph, autoscale clear this value when enable
	__declspec(dllexport)void SetYmaxVal(HGRAPH hGraph, double ymax); // Set the Y max axe value of the graph, autoscale clear this value when enable
	__declspec(dllexport)void SetZoomFactor(HGRAPH hGraph, int zoom); // Set the X scale factor value of the graph
	__declspec(dllexport)void SetFilteringMode(HGRAPH hGraph, FILTER_M filtering, const int iSignalNumber);
	__declspec(dllexport)void SetFilteringThreshold(HGRAPH hGraph, const float intensity, const int iSignalNumber);
	__declspec(dllexport)void SetSignalMinValue(HGRAPH hGraph, const int SIGNB, double val);
	__declspec(dllexport)void SetSignalAverageValue(HGRAPH hGraph, const int SIGNB, double val);
	__declspec(dllexport)void SetSignalMaxValue(HGRAPH hGraph, const int SIGNB, double val);
	__declspec(dllexport)bool GetGraphState(HGRAPH hGraph);
	__declspec(dllexport)char* GetGraphFilename(HGRAPH hGraph);
	__declspec(dllexport)HGLRC GetGraphRC(HGRAPH);
	__declspec(dllexport)HDC GetGraphDC(HGRAPH);
	__declspec(dllexport)HWND GetGraphParentWnd(HGRAPH);
	__declspec(dllexport)HWND GetGraphWnd(HGRAPH hGraph);
	__declspec(dllexport)int GetGraphSignalCount(HGRAPH hGraph);
	__declspec(dllexport)int GetZoomFactor(HGRAPH hGraph);
	__declspec(dllexport)double GetGraphLastSignalValue(HGRAPH hGraph, const int SIGNB);
	__declspec(dllexport)double GetSignalMinValue(HGRAPH hGraph, const int SIGNB);
	__declspec(dllexport)double GetSignalAverageValue(HGRAPH hGraph, const int SIGNB);
	__declspec(dllexport)double GetSignalMaxValue(HGRAPH hGraph, const int SIGNB);
	__declspec(dllexport)void SignalResetStatisticValue(HGRAPH hGraph, const int SIGNB);

	__declspec(dllexport)void AddPoint(HGRAPH hGraph, double* y, const int SignalCount);
	__declspec(dllexport)void AddMultiplePoints(HGRAPH hGraph, double** Chunks, const int SignalCount, const int BufferLength);
	__declspec(dllexport)bool Render(HGRAPH hGraph);
	__declspec(dllexport)void ReshapeGraph(HGRAPH hGraph, int left, int top, int right, int bottom);
#ifdef __cplusplus
}
#endif

#endif