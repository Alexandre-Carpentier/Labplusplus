/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cSignalTable.h"

bool is_a_number(size_t numb)
{
	if (numb != numb)
	{
		std::cout << "[*] Nan\n";
		return false;
	}
	return true;
}

cSignalTable::cSignalTable()
{
	std::cout << "[*] cSignalTable ctor called.\n";
	create_signal_table(64);
};

cSignalTable::~cSignalTable() 
{ 
	std::cout << "[*] cSignalTable dtor called.\n"; 
};

bool cSignalTable::create_signal_table(size_t sig_count) {
	if (!is_a_number(sig_count))
	{
		std::cout << "[!] Error at create_signal_table(). sig_count is not a number\n";
		return false;
	}

	std::cout << "[*] Allocate " << sig_count << " signals in create_signal_table().\n";

	// allocate all slot
	for (size_t i = 0; i < sig_count; i++)
	{
		chan_list.push_back({ 0, "slot free", "/voidaddr", "MyUnit", 0.0, 0.0, 0.0, wxColor(90, 90, 90) });
		sig_count_++;
	}

	std::cout << "[*] Allocate success.\n";
	return true;
};

void cSignalTable::set_signal_table(std::list<CHAN_LEGEND_STRUCT> list)
{
	chan_list = list;
};

std::list<CHAN_LEGEND_STRUCT> cSignalTable::get_signal_table() {
	return chan_list;
};

size_t cSignalTable::get_slot_signal_count(size_t id)
{
	size_t count = 0;
	for (auto& chan : chan_list)
	{
		if (chan.id == id)
		{
			count++;
		}
	}
	return count;
}

bool cSignalTable::slot_register_range(int length, size_t id)
{
	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_list.begin();

	size_t counter = 0;
	while (it->id != MEAS_TYPE::VOID_INSTR)
	{
		if (counter == sig_count_)
		{
			std::cout << "[!] Error at slot_register_range() max signal is reached\n";
			return false;
		}
		if (it->id == id) // count only 
		{
			counter++;
		}

		it++;
	}

	if ((counter + length) > sig_count_)
	{
		std::cout << "[!] Error at slot_register_range() can't allocate more than max signal available \"" << sig_count_ << "\".\n";
		length = sig_count_;
	}

	// Reserve sig space here
	for (auto& chan : chan_list)
	{
		if (chan.id == MEAS_TYPE::VOID_INSTR)
		{
			if (length > 0)
			{
				chan.id = id;
				chan.channel_legend_addr = std::string("slot free");
				chan.channel_legend_addr = "/addr";
				chan.channel_legend_unit = "Volt";
				chan.channel_legend_color = wxColor(90, 90, 90);
				chan.channel_legend_max_value = 0.0;
				chan.channel_legend_min_value = 0.0;
				chan.channel_legend_average_value = 0.0;
				length--;
			}
		}
	}
	/*
	for (int i = 0; i < length; i++)
	{
		*it = { type, std::string("slot free"), "/addr", "Volt", 0.0, 0.0, 0.0, wxColor(90, 90, 90) };
		it++;
	}
	*/
	return true;
}

bool cSignalTable::slot_remove_range(size_t id)
{
	
	for (auto& chan : chan_list)
	{
		if (chan.id == id)
		{
			chan.id = MEAS_TYPE::VOID_INSTR;
			chan.channel_legend_addr = std::string("slot free");
			chan.channel_legend_addr = "/addr";
			chan.channel_legend_unit = "Volt";
			chan.channel_legend_color = wxColor(90, 90, 90);
			chan.channel_legend_max_value = 0.0;
			chan.channel_legend_min_value = 0.0;
			chan.channel_legend_average_value = 0.0;
		}
	}
	return true;
}

bool cSignalTable::slot_register(size_t id)
{
	for (auto& chan : chan_list)
	{
		if (chan.id == MEAS_TYPE::VOID_INSTR)
		{
			// register the new signal
			chan.id = id;
			chan.channel_legend_addr = std::string("slot free");
			chan.channel_legend_addr = "/addr";
			chan.channel_legend_unit = "Volt";
			chan.channel_legend_color = wxColor(90, 90, 90);
			chan.channel_legend_max_value = 0.0;
			chan.channel_legend_min_value = 0.0;
			chan.channel_legend_average_value = 0.0;		
			return true;
		}
	}
	return false;

	///////////////////////////////////////////////////////////////////
	/*
	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_list.begin();

	size_t counter = 0;
	while (it->type == MEAS_TYPE::VOID_INSTR)
	{
		if (counter == sig_count_)
		{
			std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
			return false;
		}
		counter++;
		it++;
	}

	// Reserve sig space here
	it->type = type;

	return true;
	*/
}

bool cSignalTable::slot_remove(size_t id, size_t pos)
{
	int currentpos = 0;
	for (auto& chan : chan_list)
	{
		if (chan.id == id)
		{
			if (currentpos = pos)
			{
				chan.id = MEAS_TYPE::VOID_INSTR;
				chan.channel_legend_addr = std::string("slot free");
				chan.channel_legend_addr = "/addr";
				chan.channel_legend_unit = "Volt";
				chan.channel_legend_color = wxColor(90, 90, 90);
				chan.channel_legend_max_value = 0.0;
				chan.channel_legend_min_value = 0.0;
				chan.channel_legend_average_value = 0.0;
			}
		}
	}
	return true;
}

bool cSignalTable::sig_add(size_t pos, size_t id, std::string chan_name, std::string chan_addr, std::string chan_unit, wxColor chan_color)
{
	for (auto&& chan : chan_list)
	{
		//std::cout << chan.type << "\n";
	}

	std::list< CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_list.begin();

	size_t counter = 0;
	while (it->id != id)
	{
		if (counter == sig_count_-1)
		{
			std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
			return false;
		}
		counter++;
		it++;
	}

	size_t item = 0;
	while (it->id == id)
	{
		if (item == pos)
		{
			*it = { id, chan_name, chan_addr, chan_unit, 0.0, 0.0, 0.0, chan_color };
			break;
		}
		if (counter == sig_count_-1)
		{
			std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
			return false;
		}
		item++;
		it++;
	}
	return true;
}

bool cSignalTable::sig_remove(size_t id, size_t pos)
{
	
		// iterate until type is found

	size_t counter = 0;
	size_t item = 0;

	for (auto& chan : chan_list)
	{
		if (chan.id != id) // Count dead signals
		{
			if (counter == sig_count_)
			{
				std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
				return false;
			}
			counter++;
		}
		if (chan.id == id) // Signal type found
		{
			if (item == pos)
			{
				chan.channel_legend_name = "slot free";
				break;
			}
			if (counter == sig_count_)
			{
				std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
				return false;
			}
			item++;
		}
	}

	/*
	
	size_t counter = 0;
	std::list< CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_list.begin();

	
	while (it->type != type)
	{
		if (counter == sig_count_)
		{
			std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
			return false;
		}
		counter++;
		it++;
	}
	
	size_t item = 0;
	while (it->type == type)
	{
		if (item == pos)
		{
			it->channel_legend_name = "slot free";
			break;
		}
		if (counter == sig_count_)
		{
			std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
			return false;
		}
		item++;
		it++;
	}

	*/
	return true;
}

bool cSignalTable::sig_read(size_t id, size_t pos, CHAN_LEGEND_STRUCT &read_value)
{
	size_t counter = 0;
	size_t item = 0;

	for (auto& chan : chan_list)
	{
		if (chan.id != id)
		{
			if (counter == sig_count_)
			{
				std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
				return false;
			}
			counter++;
		}
		if(chan.id == id)
		{
			if (counter == sig_count_)
			{
				std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
				return false;
			}
			item++;
		}
	}
	/*
	std::list< CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_list.begin();

	size_t counter = 0;
	while (it->type != type)
	{
		if (counter == sig_count_)
		{
			std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
			return false;
		}
		counter++;
		it++;
	}

	size_t item = 0;
	while (it->type == type)
	{
		if (item == pos)
		{
			read_value = *it;
			break;
		}
		if (counter == sig_count_)
		{
			std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
			return false;
		}
		item++;
		it++;
	}
	*/
	return true;
}
