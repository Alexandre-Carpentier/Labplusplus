#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <mutex>

typedef struct {
	std::vector<std::pair<std::string,double>> controler_vec; // an agregation of instrument name and setpoints
	int jumpto;
	int jumpcount;
	double duration;
}STEPSTRUCT;

class cCycle
{
	std::mutex step_mutex; // protect current_step from data race

	typedef struct {
		std::atomic<int> current_step;
		std::atomic<int> total_step;
		std::atomic<int> current_loop;
		std::atomic<int> total_loop;
		std::vector<STEPSTRUCT> step_table;

	}CYCLESTRUCT;

public:
	std::shared_ptr<CYCLESTRUCT> cycle = nullptr;

	cCycle();

	~cCycle();

	// clear old cycles 
	void clear_cycles();

	std::shared_ptr<CYCLESTRUCT> get_cycles();

	void set_step_table(std::vector<STEPSTRUCT> step_table);
	std::vector<STEPSTRUCT>get_step_table();

	int get_current_loop();

	void set_current_loop(int count);

	int get_current_step();

	void set_current_step(int count);

	int get_total_step_number();

	void set_total_step_number(int count);

	int get_total_loop_number();

	void set_total_loop_number(int count);

public:
	void previous();
	void next();

	int get_jumpto();
	int get_jumpto(const int index);
	int get_jumpcount();
	int get_jumpcount(const int index);

	double get_duration();
	double get_duration(const int index);

	void set_jumpto(const int jt);
	void set_jumpto(const int jt, const int index);
	void set_jumpcount(const int jc);
	void set_jumpcount(const int jc, const int index);

	void set_duration(const double d);
	void set_duration(const double d, const int index);
};

