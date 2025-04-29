/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "Mouse.h"
#include "WinGraph.h"

POINT GetMousePosition(void)
{
	POINT POINT;
	if (GetCursorPos(&POINT) == FALSE)
		return { 0, 0 };

	return POINT;
}

bool isMouseHover(HGRAPH hGraph)
{
	POINT POINT;
	if (GetCursorPos(&POINT) == FALSE)
		return FALSE;

	RECT client;
	HWND hwndGraph = NULL;
	hwndGraph = GetGraphWnd(hGraph);
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

void DisplayPointer(HGRAPH hGraph)
{
	if (isMouseHover(hGraph))
	{
	}
}
