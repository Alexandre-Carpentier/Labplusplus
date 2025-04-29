/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <Windows.h>
#include "Wingraph.h"

POINT GetMousePosition(void);
bool isMouseHover(HGRAPH hGraph);
void DisplayPointer(HGRAPH hGraph);