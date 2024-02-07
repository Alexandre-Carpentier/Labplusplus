#pragma once

#include "data_types.h"

bool is_a_number(size_t numb);

class cSignalTable;

// Singleton
static cSignalTable* singleton_;

class cSignalTable
{
protected:
	// Singleton protect contructor and destructor
	cSignalTable() {}; 
	~cSignalTable() {};
public:
	static cSignalTable *getInstance()
	{
		if (singleton_ == nullptr)
		{
			singleton_ = new cSignalTable;
			std::cout << "creating cSignalTable singleton.\n";
			// Create a signal table
			// to share between components
			cSignalTable* sigt = sigt->getInstance();
			sigt->create_signal_table(64);
			std::cout << "add 64 signals to cSignalTable.\n";
		}
		else
		{
			//std::cout << "cSignalTable singleton already created.\n";
		}
		return (singleton_);
	}

	static void kill()
	{
		if (nullptr != singleton_)
		{
			delete singleton_;
			singleton_ = nullptr;
		}
	}

	bool create_signal_table(size_t sig_count) {
		if (!is_a_number(sig_count))
		{
			std::cout << "[*] Exiting cSignalTable Constructor\n\n";
			return false;
		}
		// allocate all slot
		for (size_t i = 0; i < sig_count; i++)
		{
			chan_list.push_back({ MEAS_TYPE::VOID_INSTR, "slot free", "/voidaddr", "MyUnit", 0.0, 0.0, 0.0, wxColor(90, 90, 90) });
			sig_count_++;
		}
		return true;
	};

	std::list<CHAN_LEGEND_STRUCT> get_signal_table() {
		return chan_list;
	};
	size_t get_slot_signal_count(MEAS_TYPE type);

	// register slot range / register slot
	bool slot_register_range(int length, MEAS_TYPE type);
	bool slot_register(MEAS_TYPE type);
	
	// add sig
	bool sig_add(size_t pos, MEAS_TYPE type, std::string chan_name, std::string chan_addr, std::string chan_unit, wxColor chan_color);
	bool sig_remove(MEAS_TYPE type, size_t pos);

	// retrieve sig
	bool sig_read(MEAS_TYPE type, size_t pos, CHAN_LEGEND_STRUCT &read_value);

private:
	
	size_t sig_count_ = 0;
	std::list<CHAN_LEGEND_STRUCT> chan_list;
};