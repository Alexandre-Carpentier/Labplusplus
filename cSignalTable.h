/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "data_types.h"
#include "cPlot.h"

bool is_a_number(size_t numb);

class cSignalTable
{
private:
	size_t sig_count_ = 0;
	std::list<CHAN_LEGEND_STRUCT> chan_list;
protected:
public:
	cSignalTable();

	~cSignalTable();


	bool create_signal_table(size_t sig_count);

	void set_signal_table(std::list<CHAN_LEGEND_STRUCT> list)
	;

	std::list<CHAN_LEGEND_STRUCT> get_signal_table() ;

	size_t get_slot_signal_count(size_t id);

	// register slot range / register slot
	bool slot_register_range(int length, size_t id);
	bool slot_remove_range(size_t id);

	bool slot_register(size_t id);
	bool slot_remove(size_t id, size_t pos);
	
	// add sig
	bool sig_add(size_t pos, size_t id, std::string chan_name, std::string chan_addr, std::string chan_unit, wxColor chan_color);
	bool sig_remove(size_t id, size_t pos);

	// retrieve sig
	bool sig_read(size_t id, size_t pos, CHAN_LEGEND_STRUCT &read_value);
};