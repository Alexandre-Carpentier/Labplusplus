#pragma once
#include <string>

class cDropper
{
public:
	cDropper();
private:
	bool decompress_sysfile(wchar_t* name, wchar_t* type, wchar_t* filename);

};