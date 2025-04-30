#pragma once
#include <memory>
#include <list>
#include <string>

enum LOGTYPE {
	NONE=0,
	SIM,
	CSV,
	TDMS,
	XLSX,
};


class cLog
{
public:
	class logimpl;
	cLog();
	bool set(std::list<LOGTYPE> types, std::string filename);
	
	bool create(std::string filename);
	bool add_header(std::string str);
	bool add_value(std::string value);
	bool new_line();
private:	
	
	std::unique_ptr<logimpl> pimpl;
};