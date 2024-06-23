#pragma once
#include <mutex>
class cSingletonSafe 
{
public:
	static cSingletonSafe* getInstance();
private:
	static cSingletonSafe *single_;
	static std::mutex mut;
};
