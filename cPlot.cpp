/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cPlot.h"
#include "cSignalTable.h"
#include <print>

cPlot::cPlot(wxWindow* inst, int nbPoints, cSignalTable* signal_table)
{
	inst_ = inst;
	sig_ = signal_table;
	std::cout << "cPlot ctor...\n";

	std::cout << "cMeasurementmanager->getInstance()\n";
	meas_manager = meas_manager->getInstance();

	plot_leftpanel_ = new wxPanel(inst, PLOTLEFTPANEL, wxDefaultPosition, inst->FromDIP(wxSize(300, 600)));
	plot_leftpanel_->SetBackgroundColour(wxColor(220, 220, 225));

	wxScrolled<wxPanel>* plot_legendpanel_ = new wxScrolled<wxPanel>(plot_leftpanel_, PLOTLENGENDPANEL, wxDefaultPosition, inst->FromDIP(wxSize(300, 400)));
	plot_legendpanel_->SetBackgroundColour(wxColor(90, 90, 93));
	plot_legendpanel_->SetScrollRate(0, 10);
	//plot_legendpanel_->SetScrollbar(wxVERTICAL, 0, 5, 50);

	legend_vsizer = new wxBoxSizer(wxVERTICAL);

	// Build 64 (MAX_SIG) legend signal button and hide
	for (int i = 0; i < MAX_SIG; i++)
	{
		chan_info_btn_pool[i] = new wxCustomButton((wxFrame*)plot_legendpanel_, IDCCHANINFO0 + i, "slot free", "DevX/sig0", wxColor(90, 90, 100));
		legend_vsizer->Add(chan_info_btn_pool[i], 0, wxTOP | wxLEFT | wxRIGHT | wxEXPAND, 3);
		chan_info_btn_pool[i]->Hide();
		// 
		// 
		//chan_info_btn_pool[i] = new wxButton(plot_legendpanel_, IDCCHANINFO0 + i, "slot free", wxPoint(0, 0), wxSize(300, 20), wxNO_BORDER);
		//chan_info_btn_pool[i]->SetBitmap(wxBitmap(wxT("black_sine.bmp"), wxBITMAP_TYPE_BMP));
		//legend_vsizer->Add(chan_info_btn_pool[i], 0, wxTOP | wxLEFT | wxRIGHT | wxEXPAND, 3);
		//chan_info_btn_pool[i]->Hide();

		//init_chan_to_gui("slot free", "/addr", "Volt", wxColor(i * 10, 90, 90));
	}
	CURRENT_SIG = MAX_SIG; // Save current signal legend button

	plot_legendpanel_->SetSizer(legend_vsizer);

	left_vsizer = new wxBoxSizer(wxVERTICAL);
	left_vsizer->Add(plot_legendpanel_, 1, wxALL | wxEXPAND, inst->FromDIP(5));

	plot_leftpanel_->SetSizerAndFit(left_vsizer);

	plot_rightpanel_ = new wxPanel(inst, PLOTRIGHTPANEL, wxDefaultPosition, inst->FromDIP(wxSize(600, 600)));

	// Create the plot window
	//
	plot = std::make_unique<PlotWindow>(plot_rightpanel_, 600, 600);

	plot_hsizer_ = new wxBoxSizer(wxHORIZONTAL);
	plot_hsizer_->Add(plot_leftpanel_, 0, wxEXPAND);
	plot_hsizer_->Add(plot_rightpanel_, 1, wxEXPAND);
	plot_hsizer_->Show(false);
}

cPlot::~cPlot()
{
	std::cout << "cPlot dtor...\n";
}

void cPlot::update_gui()
{
	std::list<CHAN_LEGEND_STRUCT> chan_legend_struct_list = sig_->get_signal_table();

	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_legend_struct_list.begin();

	// Hide every signals
	for (int i = 0; i < CURRENT_SIG; i++)
	{
		chan_info_btn_pool[i]->Hide();
	}
	
	// update Wingraph internal struct
	
		// update signal total number	
		int sig_count = 0;
		for (auto &chan : chan_legend_struct_list)
		{
			if (chan.id != MEAS_TYPE::VOID_INSTR)
			{
				if (chan.channel_legend_name.compare("slot free") != 0)
				{
					sig_count++;
				}
			}
		}
		plot->SetSignalCount(sig_count);

		// set every valid signal in Wingraph + display in cPlot gui
		int k = 0;
		for (auto &chan : chan_legend_struct_list)
		{
			if (chan.id != MEAS_TYPE::VOID_INSTR)
			{
				if (chan.channel_legend_name.compare("slot free") != 0)
				{
					plot->SetSignalLabel(chan.channel_legend_name.c_str(), k);
					plot->SetSignalColor((int)chan.channel_legend_color.Red(), (int)chan.channel_legend_color.Green(), (int)chan.channel_legend_color.Blue(), k);
				
					
					chan_info_btn_pool[k]->set_name(chan.channel_legend_name);
					chan_info_btn_pool[k]->set_address(chan.channel_legend_addr);
					chan_info_btn_pool[k]->set_unit(chan.channel_legend_unit);
					chan_info_btn_pool[k]->set_color(chan.channel_legend_color);

					chan_info_btn_pool[k]->Show();
					std::cout << "update gui->name: " << chan.channel_legend_name << "\n";
					k++;
				}
			}
		}
	
	/*
	// Display every active signals in plot windows
	it = chan_legend_struct_list.begin();

	int current_pos = 0;
	while (it->type != MEAS_TYPE::VOID_INSTR)
	{
		if (it->channel_legend_name.compare("slot free") != 0)
		{
			chan_info_btn_pool[current_pos]->set_name(it->channel_legend_name);
			chan_info_btn_pool[current_pos]->set_address(it->channel_legend_addr);
			chan_info_btn_pool[current_pos]->set_unit(it->channel_legend_unit);
			chan_info_btn_pool[current_pos]->set_color(it->channel_legend_color);

			chan_info_btn_pool[current_pos]->Show();		
		}
		current_pos++;
		it++;
	}
	*/
}

void cPlot::update_chan_name_to_gui(MEAS_TYPE type, std::string name, size_t position)
{
	// Update the sigtable
	//

	// Open
	std::list<CHAN_LEGEND_STRUCT> chan_legend_struct_list = sig_->get_signal_table();

	// iterate until finding item
	int pos = 0;
	for (auto &chan : chan_legend_struct_list)
	{
		
		if (chan.id == type)
		{
			if (pos == position)
			{
				if (chan.channel_legend_name.compare("slot free") != 0)
				{
					// update
					chan.channel_legend_name = name;
					std::cout << "update_chan_name_to_gui->name: " << chan.channel_legend_name << "\n";
					sig_->set_signal_table(chan_legend_struct_list);
					break;
				}
			}
			pos++;

		}
	}
	update_gui();

	chan_info_btn_pool[position]->set_name(name);
}

void cPlot::update_chan_physical_name_to_gui(std::string addr, size_t position)
{
	chan_info_btn_pool[position]->set_address(addr);
}

void cPlot::update_chan_physical_unit_to_gui(std::string unit, size_t position)
{
	chan_info_btn_pool[position]->set_unit(unit);
}

void cPlot::update_chan_statistic_labels()
{
	std::list<CHAN_LEGEND_STRUCT> chan_legend_struct_list = sig_->get_signal_table();

	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_legend_struct_list.begin();

	// BUG index different de i et decalle 
	int index = 0;
	for (size_t i = 0; i < chan_legend_struct_list.size(); i++)
	{
		if (it->id != MEAS_TYPE::VOID_INSTR)
		{
			if (it->channel_legend_name.compare("slot free") != 0)
			{
				chan_info_btn_pool[index]->set_min(get_signal_min_value(it->id, index));
				chan_info_btn_pool[index]->set_average(get_signal_average_value(index));
				chan_info_btn_pool[index]->set_max(get_signal_max_value(index));
				index++;
			}
			it++;
		}
	}
}

int cPlot::GetButtonBaseID()
{
	return chan_info_btn_pool[0]->GetId();
}

int cPlot::GuiPositionToWingraphPosition(wxWindowID id)
{

		// Deduce signal number

	int first_signal = chan_info_btn_pool[0]->GetId();
	int signal_position = id - first_signal;

		// Sanitize

	if (signal_position < 0)
	{
		std::cout << "[!] GuiPositionToWingraphPosition signal_position is invalid \n";
		return -1;
	}

	std::list<CHAN_LEGEND_STRUCT> chan_legend_struct_list = sig_->get_signal_table();
	int struct_pos = 0;
	for (auto&& chan : chan_legend_struct_list)
	{
		if (chan.id != MEAS_TYPE::VOID_INSTR)
		{
			if (chan.channel_legend_name.compare("slot free") != 0)
			{
				std::string target_name = chan_info_btn_pool[signal_position]->get_name().ToStdString();
				if (target_name.compare(chan.channel_legend_name) == 0)
				{
					std::cout << "[*] GuiPositionToWingraphPosition success, name found.\n";
					return struct_pos;
				}
				struct_pos++;
			}
		}
	}
	std::cout << "[!] GuiPositionToWingraphPosition signal_position is invalid \n";
	return -1;
}

void cPlot::update_chan_color(wxColor col, wxWindowID id)
{
	int index = GuiPositionToWingraphPosition(id);
	plot->SetSignalColor(col.Red(), col.Green(), col.Blue(), index);
}

void cPlot::reset_chan_statistic_labels(wxWindowID id)
{
	int signal_position = GuiPositionToWingraphPosition(id);
	plot->SignalResetStatisticValue(signal_position);
}

void cPlot::reset_chan_statistic_by_signal_position(size_t signal_position)
{
	plot->SignalResetStatisticValue(signal_position);
}

bool cPlot::get_graph_state()
{
	if (!plot)
	{
		std::print("[*] get_graph_state() return false (not loaded)\n");
		return false;
	}

	return plot->GetGraphState();
}

std::string cPlot::get_graph_filename()
{
	return logger.get_filename();
}

int cPlot::get_graph_signal_count()
{
	return plot->GetGraphSignalCount();
}

void cPlot::set_signal_filter(FILTER_M isFiltering, int position)
{
	int index = GuiPositionToWingraphPosition(position);
	assert(index >= 0);
	assert(index < MAX_SIG);
	plot->SetFilteringMode(isFiltering, index);
}

void cPlot::set_signal_filter_threshold(float intensity, int position)
{
	int index = GuiPositionToWingraphPosition(position);
	assert(index >= 0);
	assert(index < MAX_SIG);
	plot->SetFilteringThreshold(intensity, index);
}

void cPlot::set_signal_name(std::string signame, int position)
{
	int index = GuiPositionToWingraphPosition(position);
	assert(index >= 0);
	assert(index < MAX_SIG);
	plot->SetSignalLabel(signame.c_str(), index);
}

void cPlot::start_graph(LOGGER_M ReccordingType, int SignalNumber, std::string opt_header)
{
	if (plot->GetGraphState() == FALSE)
	{
		if (plot->GetGraphSignalCount() != SignalNumber)
		{
			if (plot->SetSignalCount(SignalNumber) == FALSE)
			{
				std::cout << "[!] Start graph is canceled, imposible to set signals\n";
				return;
			}
		}

		if (!logger.set(LOGTYPE::CSV, "test.csv"))
		{
			std::cout << "[!] Fail to open log file\n";
			return;
		}

		plot->SetRecordingMode(ReccordingType);

		if (opt_header.size() > 0)
		{
			logger.add_header(opt_header);
			plot->StartGraph(opt_header.c_str());
		}
		else
		{
			plot->StartGraph(nullptr);
		}		
	}
}

void cPlot::stop_graph()
{
	logger.close();

	if (TRUE == plot->GetGraphState())
	{
		//cMeasurementControler* meas_controler = nullptr;
		//meas_controler = meas_manager->get_measurement_controler();
		//meas_controler->stop();
		plot->StopGraph();
	}
}

void cPlot::graph_addpoint(const int signb, double val[])
{
	plot->AddPoint(val, signb);

	for(size_t i=0; i< (size_t)signb; i++)
	{
		std::string s = std::to_string(val[i]);
		logger.add_value(s);
	}
	logger.new_line();
}

void cPlot::graph_addpoints(const int signb, double *val[], int chunk_size)
{
	MessageBox(0, L"Procedure graph_addpoints() not implemented.", L"Fail", S_OK);
	//AddMultiplePoints(hGraph, val, signb, chunk_size);
}

double cPlot::get_signal_min_value(size_t id, int SignalNumber)
{
	return plot->GetSignalMinValue(SignalNumber);
}

double cPlot::get_signal_average_value(int SignalNumber)
{
	return plot->GetSignalAverageValue(SignalNumber);
}

double cPlot::get_signal_max_value(int SignalNumber)
{
	return plot->GetSignalMaxValue(SignalNumber);
}

void cPlot::set_signal_visible(bool bShow, int SignalNumber)
{
	int index = GuiPositionToWingraphPosition(SignalNumber);
	if (index < 0)
	{
		MessageBox(0, L"Fail to resolve the signal index in set_signal_visible().", L"Fail", S_OK|MB_ICONERROR);
	}
	assert(index >= 0);
	assert(index < MAX_SIG);
	plot->SetSignalVisible(bShow, index);
}

void cPlot::set_autoscale(bool enable)
{
	plot->SetAutoscaleMode(enable);
}

void cPlot::set_zoom(int factor)
{
	plot->SetZoomFactor(factor);
}

void cPlot::show_all_signals(bool isDisplayed)
{
	int i = 1;
	for (auto &item: chan_info_btn_pool )
	{
		item->set_visible(true);
	}
}

int cPlot::get_zoom()
{
	return plot->GetZoomFactor();
}

void cPlot::SizeGraph()
{
	if (!inst_)
	{
		std::print("[!] SizeGraph() inst_ is nullptr\n");
		return;
	}

	if (!plot)
	{
		std::print("[!] SizeGraph() plot is nullptr\n");
		return;
	}
	
	wxSize sz = plot_rightpanel_->GetClientSize();
	plot->SetClientSize(sz.GetWidth(), sz.GetHeight());
	
}

void cPlot::RenderGraph()
{
	std::print("[*] Render\n");
	plot->Render();
}

void cPlot::Refresh()
{
	std::print("[*] Render\n");
	plot->Refresh();
}

wxPanel* cPlot::Getleftpan()
{
	return plot_leftpanel_;
}

wxPanel* cPlot::Getrightpan()
{
	return plot_rightpanel_;
}

wxBoxSizer* cPlot::Get_hsizer()
{
	return plot_hsizer_;
}

