#ifndef _TYPES_H_
#define _TYPES_H_
#include <string>
#include <memory>

enum SCPIMODE { COM, TCP, USB, DAQMX, VISA };
enum CONTROLTYPE { BUT, TXT, TXTFIELD, SPINBUT };

/*
struct DATAS
{
	size_t buffer_size;
	std::unique_ptr<double> buffer;
	std::string unit;
};
*/
#endif
