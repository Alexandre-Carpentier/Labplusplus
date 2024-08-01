#include "cPlot.h"

cPlot::cPlot(wxWindow* inst, int nbPoints)
{
	inst_ = inst;
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

	plot_rightpanel_ = new wxPanel(inst, PLOTRIGHTPANEL, wxDefaultPosition, wxSize(600, 600), wxSUNKEN_BORDER);
	//plot_rightpanel_->SetBackgroundColour(wxColor(00, 60, 60));
	plot_rightpanel_->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));

	/*
	wxRect r = plot_rightpanel_->GetClientRect();
	RECT rect;
	rect.left = r.GetLeft(); rect.right = r.GetRight(); rect.top = r.GetTop(); rect.bottom = r.GetBottom();
	hGraph = CreateGraph(plot_rightpanel_->GetHWND(), rect, 63, nbPoints);	// Max signal = 64
	
	*/

	wxWindow* wxGraph = new wxWindow(plot_rightpanel_, PLOTWINDOW, wxDefaultPosition, wxSize(600, 600), wxSUNKEN_BORDER| wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN);
	wxRect r = wxGraph->GetClientRect();
	RECT rect;
	rect.left = r.GetLeft(); rect.right = r.GetRight(); rect.top = r.GetTop(); rect.bottom = r.GetBottom();
	hGraph = CreateGraph(wxGraph->GetHWND(), rect, 63, nbPoints);	// Max signal = 64


	plot_hsizer_ = new wxBoxSizer(wxHORIZONTAL);
	plot_hsizer_->Add(plot_leftpanel_, 0, wxEXPAND);
	plot_hsizer_->Add(plot_rightpanel_, 1, wxEXPAND);
	plot_hsizer_->Show(false);
}

cPlot::~cPlot()
{
	std::cout << "cPlot dtor...\n";
	if (hGraph != nullptr)
	{
		FreeGraph(&hGraph);
	}
}

/*
void cPlot::init_chan_to_gui(std::string chan_name, std::string chan_addr, std::string chan_unit, wxColor chan_color)
{
	// Insert data structure at the end
	chan_legend_struct_listg.push_back({chan_name, chan_addr, chan_unit, 0.0, 0.0, 0.0, chan_color });

	draw_chan_to_gui();
}

// BUGGY BUGGY
void cPlot::resize_chan_number_to_gui(size_t max_item)
{
	if (max_item < CURRENT_SIG)
	{
		// remove data structure at the end
		for (int j = chan_legend_struct_list.size(); j > max_item; j--)
		{
			chan_legend_struct_list.pop_back();
		}
		for (int j = legend_vsizer->GetItemCount() - 1; j > max_item; j--)
		{
			//legend_vsizer->Remove(j);
			// Hide custom legend in place of removing sizer ?
		}

		CURRENT_SIG = max_item; // update MAX_SIG
	}
	else if (max_item > CURRENT_SIG)
	{
		// BUGGY !!!!!!!!!!!!!!!!!!!!
		// TODO: add new wxCustomButton as the max is 64
		// TODO: add to sizer the new item
		
		// add data structure at the end
		for (int j = CURRENT_SIG; j < max_item; j++)
		{
			chan_legend_struct_list.push_back({ "slot free", "/addr", "Volt", 0.0, 0.0, 0.0, wxColor(90, 90, 90) });
		}
	}

	std::cout << "SetSignalCount is now set to " << chan_legend_struct_list.size() << "\n";
	//SetSignalCount(hGraph, chan_legend_struct_list.size());
	draw_chan_to_gui();
}

int cPlot::gui_get_last_active_channel_number()
{
	// get total element size
	size_t index = chan_legend_struct_list.size()-1;

	// point to last element
	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_legend_struct_list.end();// after last element do not dereference it (std::list::end)
	it--;

	//dec
	while (index > 0)
	{

		if (it->channel_legend_name.compare("slot free") != 0)
		{
			return index;
		}
		it--;
		index--;
	}
	return index;
}

int cPlot::get_chan_number_to_gui()
{

	std::cout << "Get gui channel number " << chan_legend_struct_list.size() << "\n";
	return chan_legend_struct_list.size();
}

void cPlot::add_chan_to_gui(std::string chan_name, std::string chan_addr, std::string chan_unit, wxColor chan_color, size_t position)
{

	std::cout << "------------add_chan_to_gui--------------------\n";

	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_legend_struct_list.begin();

	// Iterate until the position is reached
	for (int i = 0; i < position; i++)
	{
		it++; // inc iterator until the position 
	}

	// Replace data structure
	*it = { chan_name,  chan_addr, chan_unit, 0.0, 0.0, 0.0, chan_color };
	//std::cout << "RGB:" << (int)chan_color.Red() << " " << (int)chan_color.Green() << " " << (int)chan_color.Blue() << "\n";
	std::cout << "at position: " << position << "\n";

	draw_chan_to_gui();
}

void cPlot::draw_chan_to_gui()
{
	// Wingraph

	if (hGraph != nullptr)
	{
		// update size
		int sig_count = 0;
		std::list<CHAN_LEGEND_STRUCT>::iterator it;
		it = chan_legend_struct_list.begin();
		for (int i = 0; i < chan_legend_struct_list.size(); i++)
		{
			if (it->channel_legend_name.compare("slot free") != 0)
			{
				sig_count++;
			}
			it++;
		}
		SetSignalCount(hGraph, sig_count);

		// set every valid signal in Wingraph
		int k = 0;
		for (auto chan : chan_legend_struct_list)
		{
			if (chan.channel_legend_name.compare("slot free") != 0)
			{
				SetSignalLabel(hGraph, chan.channel_legend_name.c_str(), k);
				SetSignalColor(hGraph, (int)chan.channel_legend_color.Red(), (int)chan.channel_legend_color.Green(), (int)chan.channel_legend_color.Blue(), k);
				k++;
			}
		}
	}

	// cPlot

	size_t element = 0;
	for (auto chan_legend_struct : chan_legend_struct_list)
	{

		if (chan_legend_struct.channel_legend_name.compare("slot free") != 0)
		{
			legend_vsizer->Remove(element);

			chan_info_btn_pool[element]->set_name(chan_legend_struct.channel_legend_name);
			chan_info_btn_pool[element]->set_address(chan_legend_struct.channel_legend_addr);
			chan_info_btn_pool[element]->set_unit(chan_legend_struct.channel_legend_unit);
			chan_info_btn_pool[element]->set_color(chan_legend_struct.channel_legend_color);

			chan_info_btn_pool[element]->Show();

			legend_vsizer->Insert(element, chan_info_btn_pool[element], 0, wxTOP | wxLEFT | wxRIGHT | wxEXPAND, inst_->FromDIP(3));
		}

		element++;
	}

	plot_leftpanel_->SetSizerAndFit(left_vsizer);
}

void cPlot::remove_chan_to_gui(size_t position)
{
	if (position > chan_legend_struct_list.size())
	{
		std::cout << "[!] Fatal error in cPlot::remove_chan_to_gui(size_t position)\n";
		return;
	}

	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_legend_struct_list.begin();

	// Iterate until the position is reached
	for (int i = 0; i < position; i++)
		it++; // inc iterator until the position

	// Replace data structure
	*it = { "slot free", "/addr", "Volt", 0.0, 0.0, 0.0, wxColor(90, 90, 90) };
	chan_info_btn_pool[position]->Hide();

	draw_chan_to_gui();
}
*/

void cPlot::update_gui()
{
	cSignalTable* sigt = sigt->getInstance();
	std::list<CHAN_LEGEND_STRUCT> chan_legend_struct_list = sigt->get_signal_table();

	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_legend_struct_list.begin();

	// Hide every signals
	for (int i = 0; i < CURRENT_SIG; i++)
	{
		chan_info_btn_pool[i]->Hide();
	}
	
	// update Wingraph internal struct
	
	if (hGraph != nullptr)
	{
		// update signal total number	
		int sig_count = 0;
		for (auto &chan : chan_legend_struct_list)
		{
			if (chan.type != MEAS_TYPE::VOID_INSTR)
			{
				if (chan.channel_legend_name.compare("slot free") != 0)
				{
					sig_count++;
				}
			}
		}
		SetSignalCount(hGraph, sig_count);

		// set every valid signal in Wingraph + display in cPlot gui
		int k = 0;
		for (auto &chan : chan_legend_struct_list)
		{
			if (chan.type != MEAS_TYPE::VOID_INSTR)
			{
				if (chan.channel_legend_name.compare("slot free") != 0)
				{
					SetSignalLabel(hGraph, chan.channel_legend_name.c_str(), k);
					SetSignalColor(hGraph, (int)chan.channel_legend_color.Red(), (int)chan.channel_legend_color.Green(), (int)chan.channel_legend_color.Blue(), k);
				
					
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
	cSignalTable* sigt = sigt->getInstance();
	std::list<CHAN_LEGEND_STRUCT> chan_legend_struct_list = sigt->get_signal_table();

	// iterate until finding item
	int pos = 0;
	for (auto &chan : chan_legend_struct_list)
	{
		
		if (chan.type == type)
		{
			if (pos == position)
			{
				if (chan.channel_legend_name.compare("slot free") != 0)
				{
					// update
					chan.channel_legend_name = name;
					std::cout << "update_chan_name_to_gui->name: " << chan.channel_legend_name << "\n";
					sigt->set_signal_table(chan_legend_struct_list);
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
	cSignalTable* sigt = sigt->getInstance();
	std::list<CHAN_LEGEND_STRUCT> chan_legend_struct_list = sigt->get_signal_table();

	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_legend_struct_list.begin();

	// BUG index different de i et decalle 
	int index = 0;
	for (size_t i = 0; i < chan_legend_struct_list.size(); i++)
	{
		if (it->type != MEAS_TYPE::VOID_INSTR)
		{
			if (it->channel_legend_name.compare("slot free") != 0)
			{
				chan_info_btn_pool[index]->set_min(get_signal_min_value(it->type, index));
				chan_info_btn_pool[index]->set_average(get_signal_average_value(index));
				chan_info_btn_pool[index]->set_max(get_signal_max_value(index));
				index++;
			}
			it++;
		}
	}
}

int cPlot::GuiPositionToWingraphPosition(wxWindowID id)
{
	int first_signal = chan_info_btn_pool[0]->GetId();
	int signal_position = id - first_signal;

	if (signal_position < 0)
	{
		std::cout << "[!] GuiPositionToWingraphPosition signal_position is invalid \n";
		return -1;
	}

	cSignalTable* sigt = sigt->getInstance();
	std::list<CHAN_LEGEND_STRUCT> chan_legend_struct_list = sigt->get_signal_table();

	int struct_pos = 0;
	for (auto&& chan : chan_legend_struct_list)
	{
		if (chan.type != MEAS_TYPE::VOID_INSTR)
		{
			if (chan.channel_legend_name.compare("slot free") != 0)
			{
				std::string target_name = chan_info_btn_pool[signal_position]->get_name().ToStdString();
				if (target_name.compare(chan.channel_legend_name) == 0)
				{
					return struct_pos;
				}
				struct_pos++;
			}
		}
	}
	return -1;
	/*
	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_legend_struct_list.begin();

	// Iterate until and count valid signal
	int j = 0;
	for (int i = 0; i <= signal_position; i++)
	{
		if (it->channel_legend_name.compare("slot free") != 0)
		{
			j++;
		}
		it++; // inc iterator until the position 
	}
	return j;
	*/
}

void cPlot::update_chan_color(wxColor col, wxWindowID id)
{
	int index = GuiPositionToWingraphPosition(id);
	SetSignalColor(hGraph, col.Red(), col.Green(), col.Blue(), index);
}

void cPlot::reset_chan_statistic_labels(wxWindowID id)
{
	/*
	int first_signal = chan_info_btn_pool[0]->GetId();
	int signal_position =  id - first_signal;

	if (signal_position < 0)
	{
		std::cout << "[!] reset_chan_statistic_labels signal_position is invalid \n";
		return;
	}

	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_legend_struct_list.begin();

	// Iterate until and count valid signal
	int j = 0;
	for (int i = 0; i < signal_position; i++)
	{
		if (it->channel_legend_name.compare("slot free") != 0)
		{
			j++;
		}
		it++; // inc iterator until the position
	}
	signal_position = j;
	SignalResetStatisticValue(hGraph, signal_position);
	*/
	int signal_position = GuiPositionToWingraphPosition(id);
	SignalResetStatisticValue(hGraph, signal_position);
}

HGRAPH cPlot::get_graph_handle()
{
	return hGraph;
}

bool cPlot::get_graph_state()
{
	return GetGraphState(hGraph);
}

char* cPlot::get_graph_filename()
{
	return GetGraphFilename(hGraph);
}

int cPlot::get_graph_signal_count()
{
	return GetGraphSignalCount(hGraph);
}

void cPlot::set_signal_filter(FILTER_M isFiltering, int position)
{
	int index = GuiPositionToWingraphPosition(position);
	SetFilteringMode(hGraph, isFiltering, index);
}

void cPlot::set_signal_filter_threshold(float intensity, int position)
{
	int index = GuiPositionToWingraphPosition(position);
	SetFilteringThreshold(hGraph, intensity, index);
}

void cPlot::set_signal_name(std::string signame, int position)
{
	SetSignalLabel(hGraph, signame.c_str(), position);
}

void cPlot::start_graph(LOGGER_M ReccordingType, int SignalNumber, std::string opt_header)
{
	if (GetGraphState(hGraph) == FALSE)
	{
		if (GetGraphSignalCount(hGraph) != SignalNumber)
		{
			if (SetSignalCount(hGraph, SignalNumber) == FALSE)
			{
				std::cout << "[!] Start graph is canceled, imposible to set signals\n";
				return;
			}
		}

		SetRecordingMode(hGraph, ReccordingType);
		
		if(opt_header.size()>0)
			StartGraph(hGraph, opt_header.c_str());
		else
			StartGraph(hGraph, nullptr);
	}
}

void cPlot::stop_graph()
{
	if (TRUE == GetGraphState(hGraph))
	{
		//cMeasurementControler* meas_controler = nullptr;
		//meas_controler = meas_manager->get_measurement_controler();
		//meas_controler->stop();
		StopGraph(hGraph);
	}
}

void cPlot::graph_addpoint(const int signb, double val[])
{
	if (hGraph == nullptr)
	{
		std::cout << "cPlot::graph_addpoint() -> hGraph is null\n";
		return;
	}
	AddPoints(hGraph, val, signb);
}

double cPlot::get_signal_min_value(MEAS_TYPE type, int SignalNumber)
{
	cSignalTable* sigt = sigt->getInstance();
	std::list<CHAN_LEGEND_STRUCT> list = sigt->get_signal_table();

	for (auto& chan : list)
	{

	}
	return GetSignalMinValue(hGraph, SignalNumber);
}

double cPlot::get_signal_average_value(int SignalNumber)
{
	return GetSignalAverageValue(hGraph, SignalNumber);
}

double cPlot::get_signal_max_value(int SignalNumber)
{
	return GetSignalMaxValue(hGraph, SignalNumber);
}

void cPlot::set_signal_visible(bool bShow, int SignalNumber)
{
	int index = GuiPositionToWingraphPosition(SignalNumber);
	SetSignalVisible(hGraph, bShow, index);
}

void cPlot::set_autoscale(bool enable)
{
	SetAutoscaleMode(hGraph, enable);
}

void cPlot::set_zoom(int factor)
{
	SetZoomFactor(hGraph, factor);
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
	return GetZoomFactor(hGraph);
}

void cPlot::SizeGraph()
{
	if (inst_ != nullptr)
	{
		//wxRect r = plot_rightpanel_->GetClientSize();
		wxRect r = plot_rightpanel_->GetClientRect();
		ReshapeGraph(hGraph, r.GetLeft(), r.GetTop(), r.GetRight(), r.GetBottom());
	}
}

void cPlot::RenderGraph()
{
	/*
	static int counter = 0;
	std::cout << "Rendering..."<< counter <<"\n";
	counter++;
	*/
	Render(hGraph);
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

