#pragma once
#include <vector>
#include <iostream>

/*
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
*/

typedef struct {
	std::vector<std::pair<std::string,double>> controler_vec; // an agregation of instrument name and setpoints
	int jumpto;
	int jumpcount;
	double duration;
}STEPSTRUCT, * PSTEPSTRUCT;

class cCycle
{
	typedef struct {
		int current_step;
		int total_step;
		int total_loop;
		std::vector<STEPSTRUCT> step_table;

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

	/*
	void add_step_back(
		std::pair<std::string, double> instruction,

		int jumpto,
		int jumpcount,
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

		std::cout << "jumpto: " << jumpto;
		std::cout << "jumpcount: " << jumpcount;
		std::cout << "duration: " << duration;
		std::cout << "\n";

		p_items->jumpto = jumpto;
		p_items->jumpcount = jumpcount;
		p_items->duration = duration;

		pcycle->step_table.push_back(p_items);
		pcycle->total_step++;
		return;
	}
	*/
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
			//delete pcycle->step_table.at(x);
			//pcycle->step_table.at(x) = nullptr;
		}
		pcycle->step_table.clear(); // Delete raw pointers before clearing vector to prevent memory leaks
		pcycle->current_step = 0;
		pcycle->total_step = 0;
	}

public:
	void previous() { pcycle->current_step--; }
	void next() { 
		pcycle->current_step++; 
	}

	int get_jumpto() { return pcycle->step_table.at(pcycle->current_step).jumpto; }
	int get_jumpto(const int index) { return pcycle->step_table.at(index).jumpto; }
	int get_jumpcount() { return pcycle->step_table.at(pcycle->current_step).jumpcount; }
	int get_jumpcount(const int index) { return pcycle->step_table.at(index).jumpcount; }

	double get_duration() { return pcycle->step_table.at(pcycle->current_step).duration; }
	double get_duration(const int index) { return pcycle->step_table.at(index).duration; }

	void set_jumpto(const int jt) { pcycle->step_table.at(pcycle->current_step).jumpto = jt; }
	void set_jumpto(const int jt, const int index) { pcycle->step_table.at(index).jumpto = jt; }
	void set_jumpcount(const int jc) { pcycle->step_table.at(pcycle->current_step).jumpcount = jc; }
	void set_jumpcount(const int jc, const int index) { pcycle->step_table.at(index).jumpcount = jc; }

	void set_duration(const double d) { pcycle->step_table.at(pcycle->current_step).duration = d; }
	void set_duration(const double d, const int index) { pcycle->step_table.at(index).duration = d; }
};

