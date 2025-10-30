/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <vector>
#include <iostream>

struct CURRENT_VALUE_STRUCT {
	void add_values(std::string name, double value)
	{
		names_vec.push_back(name);
		values_vec.push_back(value);
	}
	void clear()
	{
		names_vec.clear();
		values_vec.clear();
	}

	std::vector<std::string> names_vec;
	std::vector<double> values_vec;
};

class  observer;

class observed {
public:
	virtual ~observed() {};
	virtual void subscribe(observer* obs) = 0;
	virtual void notify(void* data) = 0;
};

class observer {
public:
	virtual ~observer() {};
	virtual void update(void* arg) = 0;
};

class results_observer :observer {
public:
	virtual ~results_observer() {};
	virtual void update(void* arg) = 0;
};


////////////////////////////////////////////////////////
// Update the current values for every instrument
////////////////////////////////////////////////////////

class currentValueObserved :public observed {
	std::vector<observer*> obs_vec;
	
public:
	virtual ~currentValueObserved() {};

	virtual void subscribe(observer* obs) override
	{
		obs_vec.push_back(obs);
	};
	virtual void notify(void* data) override
	{
		CURRENT_VALUE_STRUCT* res = (CURRENT_VALUE_STRUCT*)data;

		for (auto o : obs_vec)
		{
			o->update(res);
		}
	};
};
