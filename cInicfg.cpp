#include <wx/wx.h>


#include "cIniCfg.h"


#include <Windows.h>

#include"cObjectmanager.h"
#include"cTable.h"


cInicfg::cInicfg()
{
	std::cout << "[*] Ini config ctor\n";
	load_table();
}

void cInicfg::load_table(void)
{
	std::cout << "[*] cInicfg::read_table start...\n";
	cObjectmanager* obj_manager = obj_manager->getInstance();
	cTable* m_table = obj_manager->get_table();

	// Save in : // C:\Users\The Hive\AppData\Roaming

	wxString str;
	wxString index;

	// Scan until the MAX_TABLE_LINE !
	const int MAX_TABLE_LINE = 200;

	cfg = new wxFileConfig(wxEmptyString, wxEmptyString, configfile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto());
	cfg->SetPath(wxT("TABLE"));

	for (int i = 0; i < MAX_TABLE_LINE; i++)
	{
		index = "pressure"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 0, str); }
		index = "voltage"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 0, str); }
		index = "flow"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 0, str); }
		index = "temperature"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 0, str); }
		index = "position"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 0, str); }
		index = "force"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 0, str); }
		index = "htriger"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 0, str); }
		index = "ltriger"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 0, str); }
		index = "jumpto"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 1, str); }
		index = "jumpcount"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 2, str); }
		index = "frontshape"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 0, str); }
		index = "duration"; index.append(wxString::Format("%i", i));
		if (cfg->Read(index, &str)) { m_table->grid->SetCellValue(i, 3, str); }
	}
	cfg->DeleteAll();
	delete cfg;
}

void cInicfg::save_table(void)
{
	std::cout << "[*] cInicfg:save_table start...\n";
	cObjectmanager* obj_manager = obj_manager->getInstance();
	cTable* m_table = obj_manager->get_table();
	int j = m_table->get_last_active_line();

	cfg = new wxFileConfig(wxEmptyString, wxEmptyString, configfile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto());
	cfg->SetPath(wxT("TABLE"));
	//cfg->DeleteGroup("TABLE");
	//cfg->DeleteAll();

	wxString str;
	for (int i = 0; i < j; i++)
	{
		double pressure = wxAtof(m_table->grid->GetCellValue(i, 0));
		double voltage = wxAtof(m_table->grid->GetCellValue(i, 0));
		double flow = wxAtof(m_table->grid->GetCellValue(i, 0));
		double temperature = wxAtof(m_table->grid->GetCellValue(i, 0));
		double position = wxAtof(m_table->grid->GetCellValue(i, 0));
		double force = wxAtof(m_table->grid->GetCellValue(i, 0));
		double htriger = wxAtof(m_table->grid->GetCellValue(i, 0));
		double ltriger = wxAtof(m_table->grid->GetCellValue(i, 0));
		int jumpto = wxAtoi(m_table->grid->GetCellValue(i, 1));
		int jumpcount = wxAtoi(m_table->grid->GetCellValue(i, 2));
		int frontshape = wxAtoi(m_table->grid->GetCellValue(i, 0));
		double duration = wxAtof(m_table->grid->GetCellValue(i, 3));

		// Save in : // C:\Users\The Hive\AppData\Roaming

		str = "pressure"; str.append(wxString::Format("%i", i));
		cfg->Write(str, pressure);
		str = "voltage"; str.append(wxString::Format("%i", i));
		cfg->Write(str, voltage);
		str = "flow"; str.append(wxString::Format("%i", i));
		cfg->Write(str, flow);
		str = "temperature"; str.append(wxString::Format("%i", i));
		cfg->Write(str, temperature);
		str = "position"; str.append(wxString::Format("%i", i));
		cfg->Write(str, position);
		str = "force"; str.append(wxString::Format("%i", i));
		cfg->Write(str, force);
		str = "htriger"; str.append(wxString::Format("%i", i));
		cfg->Write(str, htriger);
		str = "ltriger"; str.append(wxString::Format("%i", i));
		cfg->Write(str, ltriger);
		str = "jumpto"; str.append(wxString::Format("%i", i));
		cfg->Write(str, jumpto);
		str = "jumpcount"; str.append(wxString::Format("%i", i));
		cfg->Write(str, jumpcount);
		str = "frontshape"; str.append(wxString::Format("%i", i));
		cfg->Write(str, frontshape);
		str = "duration"; str.append(wxString::Format("%i", i));
		cfg->Write(str, duration);
	}
	delete cfg;
}

cInicfg::~cInicfg()
{
	save_table();
	std::cout << "[*] Ini config dtor\n";

}