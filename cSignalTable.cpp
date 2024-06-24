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

size_t cSignalTable::get_slot_signal_count(MEAS_TYPE type)
{
	size_t count = 0;
	for (auto& chan : chan_list)
	{
		if (chan.type == type)
		{
			count++;
		}
	}
	return count;
}

bool cSignalTable::slot_register_range(int length, MEAS_TYPE type)
{
	std::list<CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_list.begin();

	size_t counter = 0;
	while (it->type != MEAS_TYPE::VOID_INSTR)
	{
		if (counter == sig_count_)
		{
			std::cout << "[!] Error at slot_register_range() max signal is reached\n";
			return false;
		}
		if (it->type == type) // count only 
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
		if (chan.type == MEAS_TYPE::VOID_INSTR)
		{
			if (length > 0)
			{
				chan.type = type;
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

bool cSignalTable::slot_remove_range(MEAS_TYPE type)
{
	
	for (auto& chan : chan_list)
	{
		if (chan.type == type)
		{
			chan.type = MEAS_TYPE::VOID_INSTR;
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

bool cSignalTable::slot_register(MEAS_TYPE type)
{
	for (auto& chan : chan_list)
	{
		if (chan.type == MEAS_TYPE::VOID_INSTR)
		{
			// register the new signal
			chan.type = type;
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

bool cSignalTable::slot_remove(MEAS_TYPE type, size_t pos)
{
	int currentpos = 0;
	for (auto& chan : chan_list)
	{
		if (chan.type == type)
		{
			if (currentpos = pos)
			{
				chan.type = MEAS_TYPE::VOID_INSTR;
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

bool cSignalTable::sig_add(size_t pos, MEAS_TYPE type, std::string chan_name, std::string chan_addr, std::string chan_unit, wxColor chan_color)
{
	for (auto&& chan : chan_list)
	{
		//std::cout << chan.type << "\n";
	}

	std::list< CHAN_LEGEND_STRUCT>::iterator it;
	it = chan_list.begin();

	size_t counter = 0;
	while (it->type != type)
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
	while (it->type == type)
	{
		if (item == pos)
		{
			*it = { type, chan_name, chan_addr, chan_unit, 0.0, 0.0, 0.0, chan_color };
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

bool cSignalTable::sig_remove(MEAS_TYPE type, size_t pos)
{
	
		// iterate until type is found

	size_t counter = 0;
	size_t item = 0;

	for (auto& chan : chan_list)
	{
		if (chan.type != type) // Count dead signals
		{
			if (counter == sig_count_)
			{
				std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
				return false;
			}
			counter++;
		}
		if (chan.type == type) // Signal type found
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

bool cSignalTable::sig_read(MEAS_TYPE type, size_t pos, CHAN_LEGEND_STRUCT &read_value)
{
	size_t counter = 0;
	size_t item = 0;

	for (auto& chan : chan_list)
	{
		if (chan.type != type)
		{
			if (counter == sig_count_)
			{
				std::cout << "[!] Error at slot_register_range() max signal is reached\"" << sig_count_ << "\".\n";
				return false;
			}
			counter++;
		}
		if(chan.type == type)
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
