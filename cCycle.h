#pragma once
#include <vector>
#include <iostream>

typedef struct {
	double pressure;
	double voltage;
	double flow;
	double temperature;
	double position;
	double force;
	double htriger;
	double ltriger;
	int jumpto;
	int jumpcount;
	int frontshape;
	double duration;

}STEPSTRUCT, * PSTEPSTRUCT;

class cCycle
{
	typedef struct {
		int current_step;
		int total_step;
		int total_loop;
		std::vector<PSTEPSTRUCT> step_table;

	}CYCLESTRUCT, * PCYCLESTRUCT;

public:
	PCYCLESTRUCT pcycle = nullptr;

	cCycle()
	{
		std::cout << "cCycle ctor...\n";
	}

	~cCycle()
	{
		std::cout << "cCycle dtor...\n";
		if (pcycle)
		{
			std::cout << "Removing steps in pcycle\n";
			delete_steps();
			std::cout << "delete pcycle\n";
			delete pcycle;
		}
	}

	PCYCLESTRUCT create_cycle()
	{
		if (pcycle == nullptr)
		{
			std::cout << "new pcycle\n";
			pcycle = new CYCLESTRUCT;
			if (!pcycle)
			{
				return nullptr;
			}
			pcycle->current_step = 0;
			pcycle->total_step = 0;
			pcycle->total_loop = 0;
		}
		return pcycle;
	}

	PCYCLESTRUCT get_cycle()
	{
		if (pcycle != nullptr)
		{
			return pcycle;
		}
	}

	void add_step_back(double pressure,
		double voltage,
		double flow,
		double temperature,
		double position,
		double force,
		double htriger,
		double ltriger,
		int jumpto,
		int jumpcount,
		int frontshape,
		double duration
	)
	{
		if (pcycle == nullptr)
		{
			std::cout << "pcycle == nullptr\n";
			return;
		}

		std::cout << "new p_items in cCycle\n";
		PSTEPSTRUCT p_items = nullptr; p_items = new STEPSTRUCT;
		if (!p_items)
		{
			return;
		}
		std::cout << "pressure: " << pressure;
		std::cout << "voltage: " << voltage;
		std::cout << "flow: " << flow;
		std::cout << "temperature: " << temperature;
		std::cout << "position: " << position;
		std::cout << "force: " << force;
		std::cout << "htriger: " << htriger;
		std::cout << "ltriger: " << ltriger;
		std::cout << "jumpto: " << jumpto;
		std::cout << "jumpcount: " << jumpcount;
		std::cout << "frontshape: " << frontshape;
		std::cout << "duration: " << duration;
		std::cout << "\n";

		p_items->pressure = pressure;
		p_items->voltage = voltage;
		p_items->flow = flow;
		p_items->temperature = temperature;
		p_items->position = position;
		p_items->force = force;
		p_items->htriger = htriger;
		p_items->ltriger = ltriger;
		p_items->jumpto = jumpto;
		p_items->jumpcount = jumpcount;
		p_items->frontshape = frontshape;
		p_items->duration = duration;

		pcycle->step_table.push_back(p_items);
		pcycle->total_step++;
		return;
	}

	int get_current_loop()
	{
		if (pcycle != nullptr)
		{
			return pcycle->total_loop;
		}
		return -1;
	}

	void set_current_loop(int count)
	{
		if (pcycle != nullptr)
		{
			pcycle->total_loop = count;
		}
	}

	int get_current_step()
	{
		if (pcycle != nullptr)
		{
			return pcycle->current_step;
		}
		return -1;
	}

	void set_current_step(int count)
	{
		if (pcycle != nullptr)
		{
			pcycle->current_step = count;
		}
	}

	int get_total_step_number()
	{
		if (pcycle != nullptr)
		{
			return pcycle->total_step;
		}
		return -1;
	}

	void set_total_step_number(int count)
	{
		if (pcycle != nullptr)
		{
			pcycle->total_step = count;
		}
	}

	void delete_steps()
	{
		if (pcycle == nullptr)
		{
			std::cout << "cCycle->delete_steps: pcycle == nullptr()\n";
			return;
		}
		std::cout << "cCycle->delete_steps: step_table.clear()...\n";

		for (int x = 0; x < pcycle->step_table.size(); x++)
		{
			delete pcycle->step_table.at(x);
			pcycle->step_table.at(x) = nullptr;
		}
		pcycle->step_table.clear(); // Delete raw pointers before clearing vector to prevent memory leaks
		pcycle->current_step = 0;
		pcycle->total_step = 0;
	}

public:
	void previous() { pcycle->current_step--; }
	void next() { pcycle->current_step++; }

	double get_pressure() { return pcycle->step_table[pcycle->current_step]->pressure; }
	double get_pressure(const int index) { return pcycle->step_table[index]->pressure; }
	double get_voltage() { return pcycle->step_table[pcycle->current_step]->voltage; }
	double get_voltage(const int index) { return pcycle->step_table[index]->voltage; }
	double get_flow() { return pcycle->step_table[pcycle->current_step]->flow; }
	double get_flow(const int index) { return pcycle->step_table[index]->flow; }
	double get_temperature() { return pcycle->step_table[pcycle->current_step]->temperature; }
	double get_temperature(const int index) { return pcycle->step_table[index]->temperature; }
	double get_position() { return pcycle->step_table[pcycle->current_step]->position; }
	double get_position(const int index) { return pcycle->step_table[index]->position; }
	double get_force() { return pcycle->step_table[pcycle->current_step]->force; }
	double get_force(const int index) { return pcycle->step_table[index]->force; }
	double get_htriger() { return pcycle->step_table[pcycle->current_step]->htriger; }
	double get_htriger(const int index) { return pcycle->step_table[index]->htriger; }
	double get_ltriger() { return pcycle->step_table[pcycle->current_step]->ltriger; }
	double get_ltriger(const int index) { return pcycle->step_table[index]->ltriger; }
	int get_jumpto() { return pcycle->step_table[pcycle->current_step]->jumpto; }
	int get_jumpto(const int index) { return pcycle->step_table[index]->jumpto; }
	int get_jumpcount() { return pcycle->step_table[pcycle->current_step]->jumpcount; }
	int get_jumpcount(const int index) { return pcycle->step_table[index]->jumpcount; }
	int get_frontshape() { return pcycle->step_table[pcycle->current_step]->frontshape; }
	int get_frontshape(const int index) { return pcycle->step_table[index]->frontshape; }
	double get_duration() { return pcycle->step_table[pcycle->current_step]->duration; }
	double get_duration(const int index) { return pcycle->step_table[index]->duration; }

	void set_pressure(const double p) { pcycle->step_table[pcycle->current_step]->pressure = p; }
	void set_pressure(const double p, const int index) { pcycle->step_table[index]->pressure = p; }
	void set_voltage(const double v) { pcycle->step_table[pcycle->current_step]->voltage = v; }
	void set_voltage(const double v, const int index) { pcycle->step_table[index]->voltage = v; }
	void set_flow(const double f) { pcycle->step_table[pcycle->current_step]->flow = f; }
	void set_flow(const double f, const int index) { pcycle->step_table[index]->flow = f; }
	void set_temperature(const double t) { pcycle->step_table[pcycle->current_step]->temperature = t; }
	void set_temperature(const double t, const int index) { pcycle->step_table[index]->temperature = t; }
	void set_position(const double pos) { pcycle->step_table[pcycle->current_step]->position = pos; }
	void set_position(const double pos, const int index) { pcycle->step_table[index]->position = pos; }
	void set_force(const double force) { pcycle->step_table[pcycle->current_step]->force = force; }
	void set_force(const double force, const int index) { pcycle->step_table[index]->force = force; }
	void set_htriger(const double ht) { pcycle->step_table[pcycle->current_step]->htriger = ht; }
	void set_htriger(const double ht, const int index) { pcycle->step_table[index]->htriger = ht; }
	void set_ltriger(const double lt) { pcycle->step_table[pcycle->current_step]->ltriger = lt; }
	void set_ltriger(const double lt, const int index) { pcycle->step_table[index]->ltriger = lt; }
	void set_jumpto(const int jt) { pcycle->step_table[pcycle->current_step]->jumpto; }
	void set_jumpto(const int jt, const int index) { pcycle->step_table[index]->jumpto; }
	void set_jumpcount(const int jc) { pcycle->step_table[pcycle->current_step]->jumpcount; }
	void set_jumpcount(const int jc, const int index) { pcycle->step_table[index]->jumpcount; }
	void set_frontshape(const int fs) { pcycle->step_table[pcycle->current_step]->frontshape; }
	void set_frontshape(const int fs, const int index) { pcycle->step_table[index]->frontshape; }
	void set_duration(const double d) { pcycle->step_table[pcycle->current_step]->duration; }
	void set_duration(const double d, const int index) { pcycle->step_table[index]->duration; }
};

