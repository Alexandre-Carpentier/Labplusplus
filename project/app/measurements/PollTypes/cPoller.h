/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     02/11/25
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "cObjectmanager.h"
#include "cMeasurementmanager.h"
#include "cCycleControler.h"
#include <variant>
#include "cSimPoll.h" // Simulated aka mock
#include "cSimplePoll.h" // Real instruments controler: read ONE point per instruments
#include "cMultiPoll.h" // Real instruments controler: read MULTIPLES points per instruments

// Static polymorphism  via std::variant
using Poller = std::variant<cSimPoll, cSimplePoll, cMultiPoll>;

