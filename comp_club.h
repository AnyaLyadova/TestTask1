#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <queue>
#include <algorithm>

class Comp_Club
{
public:
	Comp_Club(std::string file) {
		file_name = file;
		count = 0;
	}
	void parse_file();

private:
	std::string file_name;
	tm start_time;
	tm end_time;
	int num_table;
	int price;

	struct Table {
		bool is_free=true;
		tm start = { };
		double time=0;
		int sum=0;
		std::string client;
	};

	std::vector <Table> tables;
	std::vector <std::string> clients;
	std::queue <std::string> cl_wait;

	void parse_string(std::string line);
	void came(tm time, std::string name);
	void sit(tm time, std::string name, int table, int code);
	void free_sit(tm time, std::string name, int table);
	void wait(tm time, std::string name);
	void leave(tm time, std::string name);
	void error(std::string message, tm time);
	tm parse_time(std::string str_time);
	int count;
	bool search_client(std::string name);
	void end_leave();
	std::string convert_time(tm* time);


};

