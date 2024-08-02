#include "cConfig.h"

//#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
//#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS


#include <wx/dcbuffer.h>
#include <wx/combobox.h>
#include <wx/treectrl.h>


//#pragma comment (lib, "Plugin.lib")
//#include "..\Lab\Plugin\cDevice.h"
#include "..\Lab++\Plugin\cDevice.h"

#include "cTable.h"
#include "cPlot.h"

#include "cDeviceMonitor.h"
#include "cPressure.h"
#include "cVoltage.h"
#include "cVoltageRs.h"
#include "cOscope.h"

/*---------------- - Duplication-------------------------------- -
void cDevice::DisplayConfiguration()
{
	std::cout << "[*] Communication are configurated with: " << this->protocol->get_type() << "\n";
	return;
}

void cDevice::set_device_name(std::string name)
{
	device_name = name;
}

void cDevice::scpi_open(std::string addr)
{
	std::cout << "[*] SCPI open called.\n";
	this->protocol->open(addr);
	return;
}

void cDevice::scpi_write(std::string command)
{
	std::cout << "[*] SCPI write called.\n";
	this->protocol->send(command);
	return;
}

std::string cDevice::scpi_read()
{
	std::string response;
	this->protocol->recv(response);
	std::cout << "[*] SCPI read recv: " << response << "\n";
	return response;
}

void cDevice::scpi_close()
{
	std::cout << "[*] SCPI close called.\n";
	return;
}

std::string cDevice::get_device_name()
{
	return device_name;
}

int cDevice::get_device_access_type()
{
	return static_cast<int>(plugin_access_type);
}

std::string cDevice::get_plugin_name()
{
	return plugin_control_name;
}

std::string cDevice::plugin_unit()
{
	return plugin_control_unit;
}

void cDevice::OnPaint()
{
	return;
}
-----------------Duplication---------------------------------*/

cConfig::cConfig(wxWindow* inst, cTable* m_table, cDeviceMonitor* devmon)
{
	inst_ = inst; // wxFrame is the parent
	devmon_ = devmon;
	std::cout << "cConfig ctor...\n";

	config_leftpanel_ = new wxPanel(inst, IDC_CONFIG_LEFT_PAN, wxDefaultPosition, inst->FromDIP(wxSize(300, 600)));
	config_leftpanel_->SetBackgroundColour(wxColor(220, 220, 225));

	config_rightpanel_ = new wxPanel(inst, IDC_CONFIG_RIGHT_PAN, wxDefaultPosition, inst->FromDIP(wxSize(600, 600)));
	//config_rightpanel_->SetBackgroundColour(wxColor(255, 55, 55)); // To not overlap on control border must be paint with same color gradiant as in DAQ or plugin
	//config_rightpanel_->Connect(wxEVT_PAINT, wxPaintEventHandler(cConfig::OnPaint)); // Draw gradiant grey

	/////////////////////////////////////////////////////////////
	//
	//	create a book to store each instrument panel
	//

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	book = new wxSimplebook(config_rightpanel_, wxID_ANY);
	//book->SetEffectsTimeouts(1000, 1000);
	//book->SetEffect(wxSHOW_EFFECT_SLIDE_TO_RIGHT);
	book->SetFitToCurrentPage(true);


	/////////////////////////////////////////////////////////////
	//
	//	create new instrument as plugin here
	//

	// Add cDaqmx to plugin vec
	m_daqmx = new cDaqmx(book);
	m_daq_dev = new cDevice();
	m_daq_dev->set_access_type(ALL);
	m_daq_dev->set_device_name("NI-DAQ");
	m_daq_dev->set_measurement_unit("Volt");
	PLUGIN_DATA Daqmx_struct;
	Daqmx_struct.name = L"NI-DAQ.dll";
	Daqmx_struct.input_count = 32;
	Daqmx_struct.outputcount = 16;
	Daqmx_struct.signal_count = Daqmx_struct.input_count + Daqmx_struct.outputcount;
	Daqmx_struct.panel = m_daqmx->get_right_panel();
	Daqmx_struct.hInst = nullptr;
	Daqmx_struct.device = m_daq_dev;
	Daqmx_struct.Attach = nullptr;
	Daqmx_struct.Dettach = nullptr;
	Daqmx_struct.PStart = nullptr;
	Daqmx_struct.PStop = nullptr;
	plugin_vec.push_back(Daqmx_struct);

	// Add cPressure to plugin vec
	m_pressure = new cPressure(book, devmon_);
	cDevice* m_pressure_dev = new cDevice();
	m_pressure_dev->set_access_type(ALL);
	m_pressure_dev->set_device_name("PACE6000");
	m_pressure_dev->set_measurement_unit("Bar");
	PLUGIN_DATA Pressure_struct;
	Pressure_struct.name = L"Pace 6000.dll";
	Pressure_struct.input_count = 1;
	Pressure_struct.outputcount = 1;
	Pressure_struct.signal_count = Pressure_struct.input_count + Pressure_struct.outputcount;
	Pressure_struct.panel = m_pressure->get_right_panel();
	Pressure_struct.hInst = nullptr;
	Pressure_struct.device = m_pressure_dev;
	Pressure_struct.Attach = nullptr;
	Pressure_struct.Dettach = nullptr;
	Pressure_struct.PStart = nullptr;
	Pressure_struct.PStop = nullptr;
	plugin_vec.push_back(Pressure_struct);

	// Add cVoltage to plugin vec
	m_voltage = new cVoltage(book, devmon_);
	cDevice* m_voltage_dev = new cDevice();
	m_voltage_dev->set_access_type(ALL);
	m_voltage_dev->set_device_name("2280S");
	m_voltage_dev->set_measurement_unit("Volt");
	PLUGIN_DATA Voltage_struct;
	Voltage_struct.name = L"2280S.dll";
	Voltage_struct.input_count = 1;
	Voltage_struct.outputcount = 1;
	Voltage_struct.signal_count = Voltage_struct.input_count + Voltage_struct.outputcount;
	Voltage_struct.panel = m_voltage->get_right_panel();
	Voltage_struct.hInst = nullptr;
	Voltage_struct.device = m_voltage_dev;
	Voltage_struct.Attach = nullptr;
	Voltage_struct.Dettach = nullptr;
	Voltage_struct.PStart = nullptr;
	Voltage_struct.PStop = nullptr;
	plugin_vec.push_back(Voltage_struct);

	// Add cVoltage to plugin vec
	m_voltage_rs = new cVoltageRs(book, devmon_);
	cDevice* m_voltage_rs_dev = new cDevice();
	m_voltage_rs_dev->set_access_type(ALL);
	m_voltage_rs_dev->set_device_name("RS6005P");
	m_voltage_rs_dev->set_measurement_unit("Volt");
	PLUGIN_DATA VoltageRs_struct;
	VoltageRs_struct.name = L"RS6005P.dll";
	VoltageRs_struct.input_count = 1;
	VoltageRs_struct.outputcount = 1;
	VoltageRs_struct.signal_count = VoltageRs_struct.input_count + VoltageRs_struct.outputcount;
	VoltageRs_struct.panel = m_voltage_rs->get_right_panel();
	VoltageRs_struct.hInst = nullptr;
	VoltageRs_struct.device = m_voltage_rs_dev;
	VoltageRs_struct.Attach = nullptr;
	VoltageRs_struct.Dettach = nullptr;
	VoltageRs_struct.PStart = nullptr;
	VoltageRs_struct.PStop = nullptr;
	plugin_vec.push_back(VoltageRs_struct);

	// Add cOscope to plugin vec
	m_oscope = new cOscope(book, devmon_);
	cDevice* m_oscope_dev = new cDevice();
	m_oscope_dev->set_access_type(READ);
	m_oscope_dev->set_device_name("DSOX1202G");
	m_oscope_dev->set_measurement_unit("Volt");
	PLUGIN_DATA Oscope_struct;
	Oscope_struct.name = L"DSOX1202G.dll";
	Oscope_struct.input_count = 1;
	Oscope_struct.outputcount = 1;
	Oscope_struct.signal_count = Oscope_struct.input_count + Oscope_struct.outputcount;
	Oscope_struct.panel = m_oscope->get_right_panel();
	Oscope_struct.hInst = nullptr;
	Oscope_struct.device = m_oscope_dev;
	Oscope_struct.Attach = nullptr;
	Oscope_struct.Dettach = nullptr;
	Oscope_struct.PStart = nullptr;
	Oscope_struct.PStop = nullptr;
	plugin_vec.push_back(Oscope_struct);

	// Add cTension to plugin vec
	/*
	m_tension = new cTension(book);
	PLUGIN_DATA Tension_struct;
	Tension_struct.name = L"Keitley 2280S.dll";
	Tension_struct.panel = m_tension->get_right_panel();
	Tension_struct.hInst = nullptr;
	Tension_struct.device = nullptr;
	Tension_struct.Attach = nullptr;
	Daqmx_struct.Dettach = nullptr;
	Daqmx_struct.PStart = nullptr;
	Daqmx_struct.PStop = nullptr;
	plugin_vec.push_back(Tension_struct);
	*/
	cObjectmanager* manager = manager->getInstance();// Singleton...bad
	manager->set_daqmx(m_daqmx); // Singleton saver...bad
	manager->set_pressuredevice(m_pressure); // Singleton saver...bad
	manager->set_voltagedevice(m_voltage); // Singleton saver...bad
	manager->set_voltagersdevice(m_voltage_rs); // Singleton saver...bad
	manager->set_oscopedevice(m_oscope); // Singleton saver...bad

	/////////////////////////////////////////////////////
	// load and add dll instrument to plugin vec
	wchar_t lpszDir[MAX_PATH] = L"";
	DWORD dwLength = GetCurrentDirectory(MAX_PATH, lpszDir);
	std::wstring wDir;
	wDir.append(lpszDir);
	wDir.append(L"\\ADDON");
	std::cout << "[*] Try to load plugin in folder: " << wDir << "\n";
	load_plugins(book, wDir);

	mainSizer->Add(book, 1, wxEXPAND);
	config_rightpanel_->SetSizer(mainSizer);

	/////////////////////////////////////////////////////
	// Populate tree ctrl with 
	// loaded plugin
	//
	config_tree_ctrl = new wxTreeCtrl(config_leftpanel_, IDCCONFIGTREE, wxDefaultPosition, inst->FromDIP(wxSize(300, 600)), wxBORDER_DOUBLE);
	config_tree_ctrl->Bind(wxEVT_LEFT_DOWN, &cConfig::OnClickdrop, this, IDCCONFIGTREE);
	config_tree_ctrl->SetBackgroundColour(wxColor(210, 210, 212));

	wxTreeItemId config_root = config_tree_ctrl->AddRoot(L"Plugins");
	wxTreeItemId config_voltage_node = config_tree_ctrl->AppendItem(config_root, "Measurement modules");

	// Remove ".dll" in filename and add it to the tree
	for (auto& plugin : plugin_vec)
	{
		plugin.name.resize(plugin.name.size() - 4); // suppress .dll from name
		config_tree_ctrl->AppendItem(config_voltage_node, plugin.name);
		std::cout << "[*] Add to tree list view: " << plugin.name << "\n";
	}

	config_tree_ctrl->ExpandAll();

	wxBoxSizer* config_vtree_sizer_ = new wxBoxSizer(wxVERTICAL);
	config_vtree_sizer_->Add(config_tree_ctrl, 1, wxALL);
	config_leftpanel_->SetSizerAndFit(config_vtree_sizer_);

	 /////////////////////////////////////////////////////
	 // Add Dll plugin to the book
	 // 
	 //
	for (auto&& plugin : plugin_vec)
	{
		book->AddPage(plugin.panel, plugin.name.c_str());
		//plugin.panel->Connect(wxEVT_PAINT, wxPaintEventHandler(cConfig::OnPaint)); // Draw gradiant grey
	}

	config_hsizer_ = new wxBoxSizer(wxHORIZONTAL);
	config_hsizer_->Add(config_leftpanel_, 0, wxEXPAND);
	config_hsizer_->Add(config_rightpanel_, 1, wxEXPAND);

	book->SetSelection(0);

	config_hsizer_->Show(false);
}

cConfig::~cConfig()
{
	std::cout << "cConfig dtor...\n";
	//unload_plugins(); // Error occured when wxWidget use the garbage collector after
	delete m_daqmx;
	delete m_daq_dev;
	delete m_pressure;
	//delete m_pressure_dev;
	delete m_voltage;
	delete m_voltage_rs;
	m_daqmx = nullptr;
	m_pressure = nullptr;
	m_voltage = nullptr;
	m_voltage_rs = nullptr;
}

void cConfig::unload_plugins()
{
	for (auto& plugin : plugin_vec)
	{
		if (plugin.hInst != nullptr)
		{
			plugin.Dettach();
			FreeLibrary(plugin.hInst);
			plugin.hInst = nullptr;
		}
	}
}

void cConfig::load_plugins(wxWindow* parent, std::wstring folder_path)
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Find the first file in the directory.
	std::wstring searchpath = folder_path;
	searchpath.append(L"\\*");
	hFind = FindFirstFile(searchpath.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{

	}

	// List all the files in the directory with some info about them.
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);

			//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring dllfilepath = folder_path;
			dllfilepath.append(L"\\");
			dllfilepath.append(ffd.cFileName);

			HINSTANCE hModule = nullptr;
			//std::cout << "[*] Found plugin module to load: " << dllfilepath << "\n";
			hModule = LoadLibrary(dllfilepath.c_str());
			if (hModule)
			{
				std::cout << "[*] Loading...\n";
				Attach = nullptr;
				Attach = (PLUGIN_ATTACH)GetProcAddress(hModule, "PLUGIN_Attach");
				if (!Attach)
				{
					std::cout << "[!] Failed to load function Attach() with GetProcAddress(). \n";
					FreeLibrary(hModule);
					continue;
					//break;
				}
				Dettach = nullptr;
				Dettach = (PLUGIN_DETTACH)GetProcAddress(hModule, "PLUGIN_Dettach");
				if (!Dettach)
				{
					std::cout << "[!] Failed to load function Dettach() with GetProcAddress(). \n";
					FreeLibrary(hModule);
					continue;
				}
				PStart = nullptr;
				PStart = (PLUGIN_START)GetProcAddress(hModule, "PLUGIN_Start");
				if (!PStart)
				{
					std::cout << "[!] Failed to load function PStart() with GetProcAddress(). \n";
					FreeLibrary(hModule);
					continue;
				}
				PStop = nullptr;
				PStop = (PLUGIN_STOP)GetProcAddress(hModule, "PLUGIN_Stop");
				if (!PStop)
				{
					std::cout << "[!] Failed to load function PStop() with GetProcAddress(). \n";
					FreeLibrary(hModule);
					continue;
				}
				std::cout << "[*] Found plugin module to load: " << dllfilepath << "\n";
				std::cout << "[*] Loading success. \n";		

				// Populate vector of PLUGIN_DATA
				std::wstring plugin_name = ffd.cFileName;

				// Attach the plugin DLL to the core system here
				cDevice* dev = nullptr;
				dev = Attach(parent);
				assert(dev != nullptr);

				wxPanel* plugin_panel = nullptr;
				if (dev != nullptr)
				{
					plugin_panel = dev->panel;
				}
				
				// If new device is a writer: 
				// add a new column in cTable with expected name and unit
				// 0 == READ
				// 1 == WRITE
				// 2 == ALL

				PLUGIN_DATA plugin_data;
				plugin_data.name = plugin_name;
				plugin_data.input_count = dev->get_input_number();
				plugin_data.outputcount = dev->get_output_number();
				plugin_data.signal_count = plugin_data.input_count + plugin_data.outputcount;
				plugin_data.panel = plugin_panel;
				plugin_data.device = dev;
				plugin_data.hInst = hModule;
				plugin_data.Attach = Attach;
				plugin_data.Dettach = Dettach;
				plugin_data.PStart = PStart;
				plugin_data.PStop = PStop;
				plugin_vec.push_back(plugin_data); // Add struct
			}
			else
			{
				dwError = GetLastError();
				std::cout << "[!] LoadLibrary() failed with code: 0x" << dwError << "\n";
			}
			//dwError = GetLastError();
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{

	}
	FindClose(hFind);
	return;	// return vector of wide string
}

wxPanel* cConfig::Getleftpan()
{
	return config_leftpanel_;
}

wxPanel* cConfig::Getrightpan()
{
	return config_rightpanel_;
}

wxBoxSizer* cConfig::Get_hsizer()
{
	return config_hsizer_;
}


cDaqmx* cConfig::get_nidaq()
{
	return m_daqmx;
}

std::vector<cDevice*> cConfig::get_device_vec()
{
	std::vector<cDevice*> vec;
	for (auto& plugin : plugin_vec)
	{
		if (plugin.device)
			vec.push_back(plugin.device);
	}
	return vec;
}

std::vector<std::string> cConfig::get_plugin_name_vec()
{
	std::vector<std::string> vec;
	for (auto& plugin : plugin_vec)
	{
		if (plugin.device)
			vec.push_back(plugin.device->get_device_name());
	}
	return vec;
}

std::vector<std::string> cConfig::get_plugin_unit_vec()
{
	std::vector<std::string> vec;
	for (auto& plugin : plugin_vec)
	{
		if(plugin.outputcount>0)
			vec.push_back(plugin.device->get_measurement_unit());
	}
	return vec;
}

std::vector<size_t> cConfig::get_plugin_output_number()
{
	std::vector<size_t> vec;
	for (auto& plugin : plugin_vec)
	{
			vec.push_back(plugin.outputcount);
	}
	return vec;
}

cPressure* cConfig::get_pressuredevice()
{
	return m_pressure;
}

cVoltage* cConfig::get_voltagedevice()
{
	return m_voltage;
}

cOscope* cConfig::get_oscopedevice()
{
	return m_oscope;
}

void cConfig::set_graph(cPlot* m_plot)
{
	m_plot_ = m_plot;
}

void cConfig::set_table(cTable* m_table)
{
	// Save cTable to add or remove colomn afterward
	m_table_ = m_table;
	m_daqmx->set_table(m_table_);
	m_pressure->set_table(m_table_);
	m_voltage->set_table(m_table_);
	m_voltage_rs->set_table(m_table_);
	m_oscope->set_table(m_table_);
}


void cConfig::OnClickdrop(wxMouseEvent& evt)
{

	wxPoint mouse_position = evt.GetPosition();
	int temp_num = wxTREE_HITTEST_ONITEMLABEL;

	wxTreeItemId id = nullptr;
	id = config_tree_ctrl->HitTest(mouse_position, temp_num);
	if (id == nullptr) // sanity check -> id
	{
		evt.Skip();
		return;
	}

	// If id exist handle panels
	if (id && config_tree_ctrl->ItemHasChildren(id))
	{

		// Root tree

	}
	else
	{
		// Children
		int index = 0;
		for (auto&& plug : plugin_vec)
		{
			// Compare with item(id) name and if match load the right panel
			wxString str = config_tree_ctrl->GetItemText(id);

			if (plug.name.compare(str) == 0)
			{

				// Hide all
				for (auto&& p : plugin_vec)
				{
					p.panel->Show(false);
				}

				// Show selected panel
				plug.panel->Show(true); // TODO maybe to delete as book->SetSelection already do the job just after

				// Show book page
				book->SetSelection(index);

				inst_->Layout();
			}
			index++;
		}
	}
	evt.Skip();
}

void cConfig::StopTest(bool stop)
{
	// Send click on STOP btn
	wxCommandEvent evt = wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, IDC_STARTBTN);
	wxPostEvent(inst_, evt);
}

void cConfig::OnPaint(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this, wxBUFFER_CLIENT_AREA);


	wxRect size = this->GetRect();

	size.x = 0;
	//dc.GradientFillLinear(size, wxColor(105, 105, 105), wxColor(255, 255, 255), wxUP);

	// change the brush to fill the whole client rectangle
	wxBrushList my_brush_list;
	wxBrush* my_brush = my_brush_list.FindOrCreateBrush(wxColour(255, 20, 20), wxBRUSHSTYLE_SOLID);
	dc.SetBrush(*my_brush); // custom filling
	dc.DrawRectangle(size);

	event.Skip();
}


