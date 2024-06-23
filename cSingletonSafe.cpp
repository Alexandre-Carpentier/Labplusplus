#include "cSingletonSafe.h"

cSingletonSafe *cSingletonSafe::getInstance()
{
	std::lock_guard<std::mutex> lock(mut);
	if (single_ == nullptr)
	{
		return new cSingletonSafe;
	}
	else
	{
		return single_;
	}
}

// Instanciate static member outside

cSingletonSafe* cSingletonSafe::single_ = nullptr;
std::mutex cSingletonSafe::mut;
