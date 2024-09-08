#pragma once
#include <string>
#include <wx/fileconf.h>

class cObjectmanager;
class cTable;

class cInicfg
{
private:
	// C:\Users\The Hive\AppData\Roaming
	std::string configfile = "Lab++TableConfig.ini";
	wxFileConfig* cfg = nullptr;
	void load_table(void);
	void save_table(void);
public:
	cInicfg();
	~cInicfg();
};

