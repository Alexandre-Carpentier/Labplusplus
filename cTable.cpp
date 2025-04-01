/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cTable.h"

#include <Windows.h>
#include <string>
#include <memory>
#include <format>
#include <vector>

#include <filesystem>
#include "cSerialize.h"
#include "cSignalDisplay.h"

void cTable::serialize(std::string config_name)
{
	size_t col = 0;
	for (auto enable : col_enable)
	{
		if (enable)
		{
			wxString col_name = grid->GetColLabelValue(col);
			std::string fullpath = "Lab++Table";
			fullpath.append(col_name);
			fullpath.append(".ini");
			cSerialize configsaver(fullpath);
			
			for (size_t i = 0; i < LINE_NB; i++)
			{
				wxString row_value = grid->GetCellValue(i, col);
				if (row_value.size() > 0)
				{
					serialize_item(configsaver, std::to_string(i), row_value);
				}
			}
		}
		col++;
	}
}

void cTable::deserialize(std::string config_name)
{
	size_t col_num = grid->GetNumberCols();
	for (size_t col = 0; col < col_num; col++)
	{
		wxString col_name = grid->GetColLabelValue(col);
		std::string filename = "Lab++Table";
		filename.append(col_name);
		filename.append(".ini");

		auto path = std::filesystem::temp_directory_path().parent_path().parent_path().parent_path();
		path /= "Roaming\\";
		path += filename;

		if (std::filesystem::exists(path))
		{
			cSerialize configsaver(filename);

			for (size_t i = 0; i < LINE_NB; i++)
			{
				wxString value;
				deserialize_item(configsaver, std::to_string(i), value);
				if (value.size() > 0)
				{
					grid->SetCellValue(i, col, value);
				}
			}
		}
	}
}

void cTable::update(void* arg)
{
	/*
	CURRENT_VALUE_STRUCT *res = (CURRENT_VALUE_STRUCT*)arg;
	size_t c = 0;
	for (auto name : res->names_vec)
	{		
		indicator_vec1[c]->SetLabelText(name);
		//indicator_vec2[c]->SetLabelText(std::to_string(res->values_vec.at(c)));
		//indicator_vec2[c]->SetLabelText(std::format("{.2}",res->values_vec.at(c)));
		char number[200];
		std::sprintf(number, "%.2lf", res->values_vec.at(c));
		indicator_vec2[c]->SetLabelText(number);
		c++;
	}
	int pos = 0;
	for (auto indicator1 : indicator_vec1)
	{
		indicator1->Show();
		if (pos++ > res->names_vec.size()-1)
		{
			indicator1->Hide();
		}
	}
	pos = 0;
	for (auto indicator2 : indicator_vec2)
	{
		indicator2->Show();
		if (pos++ > res->names_vec.size()-1)
		{
			indicator2->Hide();
		}
	}
	*/


/*
	for (size_t j=c;j++;j<max_chan_number)
	{
		indicator_vec1[j]->Hide();
		indicator_vec2[j]->Hide();
	}
	*/
	//h_indicators_sizer->Layout();
}

cTable::cTable(wxWindow* inst, cConfig *m_config)
{
	std::cout << "cTable ctor...\n";
	inst_ = inst;
	m_config_ = m_config;

	table_leftpanel_ = new wxPanel(inst, IDCTABLEPANELLEFT, wxDefaultPosition, inst->FromDIP(wxSize(300, 600)));
	table_leftpanel_->SetBackgroundColour(wxColor(240, 240, 240));

	wxButton* resetbtn = new wxButton(table_leftpanel_, IDCTABLEBTNRESET, "Clear grid", wxDefaultPosition, inst->FromDIP(wxDefaultSize), 0);
	inst->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cTable::clearButtonClicked, this, IDCTABLEBTNRESET);

	wxBoxSizer* vleftsizer = new wxBoxSizer(wxVERTICAL);
	vleftsizer->Add(resetbtn, 1, wxALL | wxEXPAND, inst->FromDIP(4));


	table_rightpanel_ = new wxPanel(inst, IDCTABLEPANELRIGHT, wxDefaultPosition, inst->FromDIP(wxSize(600, 600)), wxSUNKEN_BORDER);
	table_rightpanel_->SetBackgroundColour(wxColor(240, 240, 240));

	// Retrieve ADDON information
	std::vector<std::string>name_vec = m_config_->get_plugin_name_vec();
	std::vector<std::string>unit_vec = m_config_->get_plugin_unit_vec();
	std::vector<size_t>output_vec = m_config_->get_plugin_output_number();

	for (auto& outp : output_vec)
	{
		output_number += outp;
	}

	// Create a wxGrid object
	grid = new wxGrid(table_rightpanel_, IDCTABLEGRID, wxDefaultPosition, inst->FromDIP(wxSize(600, 600)), wxSUNKEN_BORDER);

	grid->CreateGrid(LINE_NB, COL_NB + output_number);
	grid->SetDefaultRowSize(40, false);
	grid->SetDefaultCellOverflow(false);
	grid->SetDefaultCellFont(grid->GetFont().Scale(1));
	grid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
	//grid->SetUseNativeColLabels(false);
	//grid->UseNativeColHeader(true);

	int pos = 0;
	//grid->SetColLabelValue(pos, "Pace 6000 (bar)");
	size_t elements = name_vec.size(); // total item

	int j = 0; // output_vec index
	for (auto& output : output_vec)
	{
		if (output == 0)
		{
			continue;
		}

		std::string name = name_vec.at(j); std::string unit = unit_vec.at(j);  
		
		for (int n = 0; n < output; n++) // Add each output from output_vec
		{			
			std::string col_name = name + std::format("_{}", n) + std::string("(") + unit + std::string(")");
			
			grid->SetColLabelValue(pos, col_name);
			grid->SetColSize(pos, 0); // Hide it
			col_enable.push_back(false);
			pos++; // col index
		}

		j++; // output_vec index
	}

	//grid->SetColLabelValue(1, "Voltage (V)");
	//grid->SetColLabelValue(2, "Flow (SCCM)");
	//grid->SetColLabelValue(3, "Temperature (°C)");
	//grid->SetColLabelValue(4, "Position (m)");
	//grid->SetColLabelValue(5, "Force set (N)");
	//grid->SetColLabelValue(1, "Triger H (V)");
	//grid->SetColLabelValue(2, "Triger L (V)");

	grid->SetColLabelValue(pos, "Jump to (i)");
	pos++;
	col_enable.push_back(true);
	grid->SetColLabelValue(pos, "Jump count (n)");
	pos++;
	col_enable.push_back(true);
	grid->SetColLabelValue(pos, "Duration (s)");
	pos++;
	col_enable.push_back(true);

	wxFlexGridSizer* flexsizer = new wxFlexGridSizer(2, 2, 10, 50);
	wxStaticText* staticloop = new wxStaticText(table_rightpanel_, IDCSTATICLOOP, L"Total iteration:");
	staticloop->SetFont(staticloop->GetFont().Scale(2));
	staticloop->SetBackgroundColour(wxColor(240, 240, 240));
	loop = new wxTextCtrl(table_rightpanel_, IDCLOOP, L"100", wxDefaultPosition, inst->FromDIP(wxDefaultSize), wxSUNKEN_BORDER);
	loop->SetFont(loop->GetFont().Scale(2));

	stat = new cDurationStatisticCtrl(table_rightpanel_);

	flexsizer->Add(staticloop);
	flexsizer->Add(loop);
	flexsizer->Add(stat);

	///////////////////////////////////////////////////////
	// Create indicators
	///////////////////////////////////////////////////////
	for (size_t i = 0; i < max_chan_number; i ++ )
	{
		indicator_vec1[i] = new wxStaticText(table_rightpanel_, IDCINDICATORNAME0+i, L"Signal:");
		//indicator_vec1[i]->SetFont(indicator_vec1[i]->GetFont().Scale(1));
		indicator_vec1[i]->SetBackgroundColour(wxColor(102, 178, 255));
		//indicator_vec1[i]->Hide();
		indicator_vec2[i] = new wxStaticText(table_rightpanel_, IDCINDICATORVALUE0 +i, L"00.00");
		indicator_vec2[i]->SetFont(indicator_vec2[i]->GetFont().Scale(2));
		//indicator_vec2[i]->SetBackgroundColour(wxColor(112, 112, 111));
		//indicator_vec2[i]->Hide();
	}
	h_indicators_sizer = new wxBoxSizer(wxHORIZONTAL);
	for (size_t i = 0; i < max_chan_number; i++)
	{
		h_indicators_sizer->Add(indicator_vec1[i], 0, wxALL, inst->FromDIP(10));
		h_indicators_sizer->Add(indicator_vec2[i], 0, wxALL, inst->FromDIP(10));
		indicator_vec1[i]->Hide();
		indicator_vec2[i]->Hide();
	}

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->Add(flexsizer, 0, wxALL | wxEXPAND, inst->FromDIP(10));

	wxBoxSizer* grid_vsizer = new wxBoxSizer(wxVERTICAL);
	grid_vsizer->Add(h_indicators_sizer, 0, wxEXPAND);
	grid_vsizer->Add(grid, 1, wxEXPAND);
	grid_vsizer->Add(hbox, 0, wxCENTER);


	table_leftpanel_->SetSizerAndFit(vleftsizer);
	table_rightpanel_->SetSizerAndFit(grid_vsizer);

	table_hsizer_ = new wxBoxSizer(wxHORIZONTAL);
	table_hsizer_->Add(table_leftpanel_, 0, wxEXPAND);
	table_hsizer_->Add(table_rightpanel_, 1, wxEXPAND);

	table_hsizer_->Show(false);
	//set_line_highlight(2);

	m_config_->set_table(this); // share cTable with plugin

	// Deserialize
	for (auto name : name_vec)
	{
		deserialize(name);
	}
}

cTable::~cTable()
{
	std::cout << "cTable dtor...\n";

	// Serialize
	std::vector<std::string>name_vec = m_config_->get_plugin_name_vec();
	for (auto name : name_vec)
	{
		serialize(name);
	}

	delete stat; // TODO wxwidget delete stat automatically ?
	stat = nullptr;
	// Save current row values
}

int cTable::get_step_number()
{
	return get_last_active_line();
}

int cTable::get_loop_number()
{
	wxString loopnumber = loop->GetValue();
	int iloopnumber = wxAtoi(loopnumber);
	if (iloopnumber < 0)
		return 0;

	return iloopnumber;
}

double cTable::get_total_step_duration()
{
	double sec = 0.0;
	for (int row = 0; row < get_last_active_line(); row++)
	{
		sec += wxAtof(grid->GetCellValue(row, get_last_active_col()));
	}
	return sec;
}

std::vector<STEPSTRUCT> cTable::get_step_table()
{
	std::vector<STEPSTRUCT> step_table;

	// For each table line
	for (int row = 0; row < get_last_active_line(); row++)
	{

		// Fill the step struc for each line
		STEPSTRUCT step;
		step.duration = 0.0;
		step.jumpcount = 0.0;
		step.jumpto = 0.0;
		step.controler_vec.clear();

		// For each table collumn
		int col_numb = 0;
		col_numb = grid->GetNumberCols();
		for (int col = 0; col < col_numb; col++)
		{
			// populate the std::pair with col name and col value
			// put all std::pair in a vector
			std::string name = grid->GetColLabelValue(col).ToStdString();
			double value = wxAtof(grid->GetCellValue(row, col));
			if (grid->GetColSize(col) != 0)
			{
				step.controler_vec.push_back(std::make_pair(name, value));
			}
		}
		// Add step to the std::vector
		step_table.push_back(step);

		// extract table information (duration, jump to, ...)
		// to fill extra field needed
	}

	int index = 0;
	for (auto& item : step_table)
	{
		for (auto& controler : item.controler_vec)
		{
			if (controler.first.compare(std::string("Duration (s)")) == 0)
			{
				item.duration = controler.second;
			}
			if (controler.first.compare("Jump to (i)") == 0)
			{
				item.jumpto = controler.second;
			}
			if (controler.first.compare("Jump count (n)") == 0)
			{
				item.jumpcount = controler.second;
			}
			index++;
		}
	}

	return step_table;
}

void cTable::start_statistic(std::shared_ptr<cCycle> m_cycle)
{
	// if stat already created, reset it
	if (stat->isTick())
	{
		stat->reset();
	}

	stat->start(m_cycle);
}

void cTable::stop_statistic()
{
	stat->stop();
}

void cTable::enable_device_col(std::string name)
{
	// Find col by name and update col_enable
	int sz = grid->GetNumberCols();
	for (int i = 0; i < sz; i++)
	{
		std::string table_name = grid->GetColLabelValue(i).ToStdString();
		if (name.compare(table_name) == 0)
		{
			std::cout << "[*] Found col in cTable at: " << name << "\n";
			col_enable.at(i) = true;
		}
	}
}

void cTable::disable_device_col(std::string name)
{
	// Find col by name and update col_enable
	int sz = grid->GetNumberCols();
	for (int i = 0; i < sz; i++)
	{
		if (name.compare(grid->GetColLabelValue(i).ToStdString()) == 0)
		{
			std::cout << "[*] Found col in cTable at: " << name << "\n";
			col_enable.at(i) = false;
			grid->SetColSize(i, 0); // Hide it
		}
	}
}

void cTable::set_line_highlight(const int line)
{
	for (int i = 0; i < COL_NB + output_number; i++)
	{
		grid->SetCellBackgroundColour(line - 1, i, *wxWHITE);
		grid->SetCellBackgroundColour(line, i, *wxLIGHT_GREY);
		grid->SetCellBackgroundColour(line + 1, i, *wxWHITE);
		grid->Refresh();
	}
}

void cTable::set_lines_white()
{
	for (int line = 0; line < get_last_active_line(); line++)
	{
		for (int i = 0; i < COL_NB + output_number; i++)
		{
			grid->SetCellBackgroundColour(line, i, *wxWHITE);
			grid->Refresh();
		}
	}
}

int cTable::get_last_active_line()
{
	for (int i = 0; i < LINE_NB; i++)
	{
		if (!IsActiveLine(i))
		{
			return i;
		}
	}
	return 0;
}

int cTable::get_last_active_col()
{
	int col_num = grid->GetNumberCols();

	double duration = wxAtof(grid->GetCellValue(0, col_num));

	assert(duration > 0.0);
	return duration;
}

bool cTable::IsActiveLine(const int line)
{
	double cell = 0;
	cell = wxAtof(grid->GetCellValue(line, COL_NB + output_number - 1)); // look at duration
	if (cell > 0.0)
		return true;
	else
		return false;
}

void cTable::GridResize(wxGrid* grid)
{
	wxSize size = grid->GetSize();
	//int cell_width = ((size.x - size.x / (COL_NB + pugin_number)) / (COL_NB + pugin_number));

	// Count visible col
	size_t displayed = 0;
	for (auto item : col_enable)
	{
		if(item==true)
			displayed++;
	}
	int cell_width = (size.x / (displayed));
	cell_width = (grid->GetClientSize().x - wxSystemSettings::GetMetric(wxSYS_VSCROLL_X)) / 5;

	for (int i = 0; i < COL_NB + output_number; i++)
	{
		if (col_enable.at(i) == true)
		{
			grid->SetColSize(i, ((size.x - size.x / (displayed)) / (displayed)));
		}
	}
}

void cTable::set_loop_count(int count)
{
	wxString str = std::to_string(count);
	loop->SetValue(str);
}

wxPanel* cTable::Getleftpan()
{
	return table_leftpanel_;
}

wxPanel* cTable::Getrightpan()
{
	return table_rightpanel_;
}

wxBoxSizer* cTable::Get_hsizer()
{
	return table_hsizer_;
}

void cTable::clearButtonClicked(wxCommandEvent& evt)
{
	std::cout << "cTable->clearButtonClicked()\n";
	int last = get_last_active_line();
	std::string s = std::to_string(last + 1);
	grid->SetCellValue(0, 0, s);

	INT iResult = MessageBox(GetFocus(), L"Are you sure to clear all cell values from table ?", L"Clear ? ", MB_ICONQUESTION | MB_YESNO | MB_SYSTEMMODAL);
	if (iResult == IDYES)
	{
		grid->ClearGrid();
	}
	evt.Skip();
}

cDurationStatisticCtrl::cDurationStatisticCtrl(wxWindow* parent) :
	wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize), wxTimer()
{
	wxBoxSizer* vBox = new wxBoxSizer(wxVERTICAL);

	started = new wxStaticText(this, wxID_ANY, "Started at: /", wxDefaultPosition, this->FromDIP(wxSize(200, 20)), wxALIGN_CENTER);
	end = new wxStaticText(this, wxID_ANY, "End at: /", wxDefaultPosition, this->FromDIP(wxSize(200, 20)), wxALIGN_CENTER);
	elapsed = new wxStaticText(this, wxID_ANY, "Elapsed: /", wxDefaultPosition, this->FromDIP(wxSize(200, 20)), wxALIGN_CENTER);
	cycle_step_state = new wxStaticText(this, wxID_ANY, "Performed: / cyles", wxDefaultPosition, this->FromDIP(wxSize(200, 20)), wxALIGN_CENTER);

	vBox->Add(started);
	vBox->Add(end);
	vBox->Add(elapsed);
	vBox->Add(cycle_step_state);

	this->SetSizer(vBox);
}

cDurationStatisticCtrl::~cDurationStatisticCtrl()
{
	delete tick;
}


void cDurationStatisticCtrl::start(std::shared_ptr<cCycle> m_cycle)
{
	// Save references
	m_cycle_ = m_cycle;

	// Compute & display statistic timestamps
	double sec = 0.0;
	auto cycle_obj = m_cycle_->get_cycles();
	auto step_table = cycle_obj->step_table;
	for (auto& step : step_table)
	{
		sec += step.duration;
	}
	assert(sec > 0);

	//todo: loop count must be in step table

	auto duration_s = sec * m_cycle_->get_total_loop_number();
	//auto step_number = j * m_cycle_->get_current_loop();

	// Fill indicators
	wxString current_timestamp = "Started at :" + get_current_timestamp();
	started->SetLabelText(current_timestamp);

	wxString futur_timestamp = "End at: " + add_to_current_timestamp(duration_s);
	end->SetLabelText(futur_timestamp);

	wxString elapsed_timestamp = "Elapsed: 0.0 s";// +add_to_current_timestamp(0.0);
	elapsed->SetLabelText(elapsed_timestamp);

	saved_total_cycle = m_cycle->get_current_loop();
	
	int j = m_cycle->get_total_step_number();
	saved_total_step = j;
	wxString cycle_status = std::format("Performed: 0/{} cyles", saved_total_cycle);
	cycle_step_state->SetLabelText(cycle_status);

	// Update timestamp every 0.5s with a timer event
	tick = new cTick;
	wxTimer::Start(500);
	tick->start_tick();
}

void cDurationStatisticCtrl::stop()
{
	// stop stat
	wxTimer::Stop();
}

void cDurationStatisticCtrl::reset()
{
	started->SetLabelText("Started at: /");

	end->SetLabelText("End at: /");

	elapsed->SetLabelText("Elapsed: /");

	cycle_step_state->SetLabelText("Performed: / cyles");

	assert(tick != nullptr);
	tick->reset_tick();

	savedst = { 0 };
	savedft = { 0 };
	saved_total_cycle = 0;
	saved_total_step = 0;
}

bool cDurationStatisticCtrl::isTick()
{
	if (tick)
		return true;
	else
		return false;
}



/*
* Called by the system of by wxWidgets when the panel needs
* to be redrawn. You can also trigger this call by
* calling Refresh()/Update().
*/

void cDurationStatisticCtrl::paintEvent(wxPaintEvent& evt)
{
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC dc(this);
	render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void cDurationStatisticCtrl::paintNow()
{
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC dc(this);
	render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void cDurationStatisticCtrl::render(wxDC& dc)
{
	//if (pressedDown)
		//dc.SetBrush(*wxLIGHT_GREY_BRUSH);
	//else
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	//dc.SetBrush(*wxGREY_BRUSH);


//dc.DrawRectangle(0, 0, buttonWidth, buttonHeight);

}

std::string cDurationStatisticCtrl::get_current_timestamp()
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };

	GetLocalTime(&st);

	wprintf(L"%02d/%02d/%04d %02d:%02d:%02d\n",
		st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

	std::string timestamp = std::format("{}/{}/{} {}hr{}min{}s", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
	return timestamp;
}

std::string cDurationStatisticCtrl::add_to_current_timestamp(size_t duration_s)
{
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };

	GetLocalTime(&st);

	//wprintf(L"%02d/%02d/%04d %02d:%02d:%02d\n",
		//st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

	SystemTimeToFileTime(&st, &ft);

	ULARGE_INTEGER u = { 0 };

	memcpy(&u, &ft, sizeof(u));
	u.QuadPart += duration_s * 10000000LLU;
	memcpy(&ft, &u, sizeof(ft));

	FileTimeToSystemTime(&ft, &st);

	//wprintf(L"%02d/%02d/%04d %02d:%02d:%02d\n",
	//	st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

	std::string new_timestamp = std::format("{}/{}/{} {}hr{}min{}s", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
	return new_timestamp;
}

std::string cDurationStatisticCtrl::compute_elapsed_timestamp()
{
	// Runonce
	if (savedst.wYear == 0)
	{
		GetLocalTime(&savedst);
		SystemTimeToFileTime(&savedst, &savedft);
	}

	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };

	GetLocalTime(&st);

	//wprintf(L"%02d/%02d/%04d %02d:%02d:%02d\n",
		//st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

	SystemTimeToFileTime(&st, &ft);

	ULARGE_INTEGER u0 = { 0 };
	ULARGE_INTEGER u = { 0 };

	memcpy(&u0, &savedft, sizeof(u0)); //initial start tick
	memcpy(&u, &ft, sizeof(u)); //now

	u.QuadPart -= u0.QuadPart;

	//u.QuadPart -= 2*elapsed * 10000000LLU;
	memcpy(&ft, &u, sizeof(ft));

	FileTimeToSystemTime(&ft, &st);

	//wprintf(L"%02d/%02d/%04d %02d:%02d:%02d\n",
	//	st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

	std::string new_timestamp = std::format(" {}hr{}min{}s", st.wHour, st.wMinute, st.wSecond);
	return new_timestamp;
}

void cDurationStatisticCtrl::Notify()
{
	// Compute elapsed time
	wxString elapsed_timestamp = "Elapsed: " + compute_elapsed_timestamp();
	elapsed->SetLabelText(elapsed_timestamp);

	// Compute number of cycle performed
	size_t current_cycle = m_cycle_->get_total_loop_number()- (m_cycle_->get_current_loop());
	size_t total_cycle = saved_total_cycle;

	// zu or Iu to print size_t integer value -> after MSVC2013 we can use zu, before it was not ANSI compatible and is %Iu formater.
	//wxString performed = wxString::Format("Performed: %Iu/%Iu", current_cycle, total_cycle); // Use %Iu for MSW %zu otherwize to print size_t number
	wxString performed = wxString::Format("Performed: %zu/%zu", current_cycle, total_cycle); // Use %Iu for MSW %zu otherwize to print size_t number
	cycle_step_state->SetLabelText(performed);

	// Update % in status bar
	cObjectmanager* object_manager = object_manager->getInstance();
	wxStatusBar* statusbar = object_manager->get_status_bar();
	wxString statusstr = wxString::Format("%zu %% performed...", (current_cycle * 100) / total_cycle);
	statusbar->SetLabelText(statusstr);
}