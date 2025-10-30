#pragma once
#include <memory>
#include <list>
#include <string>

enum LOGTYPE {
	NONE=0,
	SIM,
	TSV,
	CSV,
	TDMS,
	XLSX,
};


class cLog
{
public:
	///////////////////////////////////////////////////////////////////////////////////
	// Setup the logger
	cLog(); // Must call set() after
	cLog(LOGTYPE type, std::string filename);
	~cLog();
	
	bool set(LOGTYPE type, std::string filename);	
	bool isReady();

	///////////////////////////////////////////////////////////////////////////////////
	// Use the logger
	bool create();
	bool add_header(std::string str);
	bool add_value(std::string value);
	bool new_line();

	std::string get_filename();
	LOGTYPE get_reccording_mode();

	///////////////////////////////////////////////////////////////////////////////////
	// Clean logger
	bool close();

private:	
	class logimpl;
	std::unique_ptr<logimpl> pimpl;
};