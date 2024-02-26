#include "cTable.h"

#include"cPlot.h"

#include "cCycle.h"
#include "cTick.h"
#include <format>

class cConfig;
class cDevice;

cTable::cTable(wxWindow* inst, cConfig* m_config)
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

	pugin_number = unit_vec.size();
	

	// Create a wxGrid object
	grid = new wxGrid(table_rightpanel_, IDCTABLEGRID, wxDefaultPosition, inst->FromDIP(wxSize(600, 600)), wxSUNKEN_BORDER);

	grid->CreateGrid(LINE_NB, COL_NB + pugin_number);
	grid->SetDefaultRowSize(40, false);
	grid->SetDefaultCellOverflow(false);
	grid->SetDefaultCellFont(grid->GetFont().Scale(1));
	grid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
	//grid->SetUseNativeColLabels(false);
	//grid->UseNativeColHeader(true);

	int pos = 0;
	grid->SetColLabelValue(pos, "Pace 6000 (bar)");

	int j = 0;
	for (auto& unit : unit_vec)
	{
		pos++;
		std::string name = name_vec.at(j); j++;
		grid->SetColLabelValue(pos, name + std::string("(") + std::string(unit) + std::string(")"));
	}

	//grid->SetColLabelValue(1, "Voltage (V)");
	//grid->SetColLabelValue(2, "Flow (SCCM)");
	//grid->SetColLabelValue(3, "Temperature (°C)");
	//grid->SetColLabelValue(4, "Position (m)");
	//grid->SetColLabelValue(5, "Force set (N)");
	//grid->SetColLabelValue(1, "Triger H (V)");
	//grid->SetColLabelValue(2, "Triger L (V)");

	grid->SetColLabelValue(++pos, "Jump to (i)");
	grid->SetColLabelValue(++pos, "Jump count (n)");
	//grid->SetColLabelValue(5, "Front");
	grid->SetColLabelValue(++pos, "Duration (s)");

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


	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	hbox->Add(flexsizer, 0, wxALL | wxEXPAND, inst->FromDIP(10));

	wxBoxSizer* grid_vsizer = new wxBoxSizer(wxVERTICAL);
	grid_vsizer->Add(grid, 1, wxEXPAND);
	grid_vsizer->Add(hbox, 0, wxCENTER);

	table_leftpanel_->SetSizerAndFit(vleftsizer);
	table_rightpanel_->SetSizerAndFit(grid_vsizer);

	table_hsizer_ = new wxBoxSizer(wxHORIZONTAL);
	table_hsizer_->Add(table_leftpanel_, 0, wxEXPAND);
	table_hsizer_->Add(table_rightpanel_, 1, wxEXPAND);

	table_hsizer_->Show(false);
	//set_line_highlight(2);
}

cTable::~cTable()
{
	std::cout << "cTable dtor...\n";
	delete stat;
	// Save current row values
}

void cTable::destroy_cycle()
{
	std::cout << "cTable->destroy_cycle()\n";
	delete m_cycle;
	m_cycle = nullptr;

	// Stop stat
	stat->stop();
}

cCycle* cTable::load_cycle()
{
	std::cout << "cTable->load_cycle()\n";
	int j = get_last_active_line();

	if (j <= 0)
		return nullptr;

	std::cout << "new cCycle()\n";
	if(m_cycle != nullptr)
	{
		m_cycle = nullptr;
	}
	m_cycle = nullptr;
	m_cycle = new cCycle();
	m_cycle->create_cycle();

	wxString loopnumber = loop->GetValue();
	int iloopnumber = wxAtoi(loopnumber);
	if (iloopnumber <= 0)
		return nullptr;

	m_cycle->set_current_loop(iloopnumber);

	// For each table line
	for (int row = 0; row < j; row++)
	{
		// Inc step number in struct
		m_cycle->pcycle->total_step++;

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
			step.controler_vec.push_back( std::make_pair(name, value) );
			
		}
		// Add step to the std::vector
		m_cycle->pcycle->step_table.push_back(step);

		// extract table information (duration, jump to, ...)
		// to fill extra field needed
	}

	int index = 0;
	for (auto& item : m_cycle->pcycle->step_table)
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
		// Sanity check
		if (item.duration == 0.0)
		{
			MessageBox(GetFocus(), L"Step duration is too small", L"Be carefull", S_OK);
			delete m_cycle;
			return nullptr;
		}
	}

	/*
	for (int i = 0; i < j; i++)
	{
		double pressure = wxAtof(grid->GetCellValue(i, 0));
		double voltage = wxAtof(grid->GetCellValue(i, 0));
		double flow = wxAtof(grid->GetCellValue(i, 0));
		double temperature = wxAtof(grid->GetCellValue(i, 0));
		double position = wxAtof(grid->GetCellValue(i, 0));
		double force = wxAtof(grid->GetCellValue(i, 0));
		double htriger = wxAtof(grid->GetCellValue(i, 0));
		double ltriger = wxAtof(grid->GetCellValue(i, 0));
		int jumpto = wxAtoi(grid->GetCellValue(i, 1));
		int jumpcount = wxAtoi(grid->GetCellValue(i, 2));
		int frontshape = wxAtoi(grid->GetCellValue(i, 0));
		double duration = wxAtof(grid->GetCellValue(i, 3));

		m_cycle->add_step_back(pressure,
			voltage,
			flow,
			temperature,
			position,
			force,
			htriger,
			ltriger,
			jumpto,
			jumpcount,
			frontshape,
			duration
		);
	}
	*/

	stat->start(m_cycle, this);
	return m_cycle;
}

void cTable::set_line_highlight(const int line)
{
	for (int i = 0; i < COL_NB + pugin_number; i++)
	{
		grid->SetCellBackgroundColour(line - 1, i, *wxWHITE);
		grid->SetCellBackgroundColour(line, i, *wxLIGHT_GREY);
		grid->SetCellBackgroundColour(line + 1, i, *wxWHITE);
		grid->Refresh();
	}
}

void cTable::set_lines_white()
{
	for (int line = 0; line < m_cycle->get_total_step_number(); line++)
	{
		for (int i = 0; i < COL_NB + pugin_number; i++)
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
			//return i-1;
		}
	}
	return LINE_NB;
}

bool cTable::IsActiveLine(const int line)
{
	double cell = 0;
	cell = wxAtof(grid->GetCellValue(line, COL_NB + pugin_number - 1));
	if (cell > 0.0)
		return true;
	else
		return false;
}

void cTable::GridResize(wxGrid* grid)
{
	wxSize size = grid->GetSize();
	//int cell_width = ((size.x - size.x / (COL_NB + pugin_number)) / (COL_NB + pugin_number));

	int cell_width = (size.x / (COL_NB + pugin_number ) );
	cell_width = (grid->GetClientSize().x - wxSystemSettings::GetMetric(wxSYS_VSCROLL_X)) / 5;
	
	for (int i = 0; i < COL_NB + pugin_number; i++)
	{
		grid->SetColSize(i, cell_width);
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


void cDurationStatisticCtrl::start(cCycle* m_cycle, cTable* m_table)
{
	// Save references
	m_cycle_ = m_cycle;
	m_table_ = m_table;

	// Compute & display statistic timestamps
	double sec = 0.0;
	int j = m_table->get_last_active_line();
	for (int i = 0; i < j; i++)
	{
		sec += m_cycle->get_duration();
	}
	auto duration_s = sec * m_cycle->get_current_loop();
	auto step_number = j * m_cycle->get_current_loop();

	// Fill indicators
	wxString current_timestamp = "Started at :" + get_current_timestamp();
	started->SetLabelText(current_timestamp);

	wxString futur_timestamp = "End at: " + add_to_current_timestamp(duration_s);
	end->SetLabelText(futur_timestamp);

	wxString elapsed_timestamp = "Elapsed: " + add_to_current_timestamp(0);
	elapsed->SetLabelText(elapsed_timestamp);

	saved_total_cycle = m_cycle->get_current_loop();
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
	tick->reset_tick();
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
	auto current_cycle = saved_total_cycle - m_cycle_->get_current_loop();
	auto total_cycle = saved_total_cycle;
	wxString performed = wxString::Format("Performed: %Iu/%Iu", current_cycle, total_cycle); // Use %Iu for MSW %zu otherwize to print size_t number
	cycle_step_state->SetLabelText(performed);

	// Update % in status bar
	cObjectmanager* object_manager = object_manager->getInstance();
	wxStatusBar* statusbar = object_manager->get_status_bar();
	wxString statusstr = wxString::Format("%Ii %% performed...", (current_cycle * 100) / total_cycle);
	statusbar->SetLabelText(statusstr);
}