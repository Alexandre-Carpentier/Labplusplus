/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     03/11/25
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

struct cSimPoll
{
public:
	cSimPoll(cObjectmanager* obj_manager, cMeasurementmanager* meas_manager, std::shared_ptr<cCycleControler>);
	void runonce();
	void loop();
	void stop();
	~cSimPoll() = default;
private:

	// Loc variables
	CHUNK m_chunk;

	// Controlers refs
	cObjectmanager* m_obj_manager = nullptr; // gui refs singleton
	cMeasurementmanager* m_meas_manager = nullptr; // measurment refs singleton
	std::shared_ptr<cCycleControler> m_cycle_controler = nullptr; // cycle controler singleton

	// Gui elements
	wxStatusBar* m_statusbar = nullptr;
	cPlot* m_plot = nullptr;
	cFooter* m_footer = nullptr;

	// instruments objects
	std::vector<cMeasurement*> m_meas_pool; // hold the the measurement to perform on instruments

	void write_instruments();
	void read_instruments();
};