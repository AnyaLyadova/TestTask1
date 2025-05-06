#include "comp_club.h"

void Comp_Club::parse_file()
{
	std::ifstream fin;
	fin.open(file_name);
	if (fin.is_open()){
		std::string line;
		fin >> line;
		num_table = stoi(line);
		for (int i = 0; i < num_table; ++i) {
			tables.push_back(Table());

		}
		fin >> line;
		start_time = parse_time(line);
		fin >> line;
		end_time = parse_time(line);
		fin >> line;
		price = stoi(line);
		printf("%02d:%02d\n", start_time.tm_hour, start_time.tm_min);

		while (!fin.eof()) {
			fin >> line;
			tm time = parse_time(line);
			int event=0;
			fin >> event;
			std::string name;
			fin >> name;
			int tab_num = 0;
			if (event == 2) {
				fin >> tab_num;
			}
			switch (event) {
			case 1:
				came(time, name);
				break;
			case 2:
				sit(time, name, tab_num,2);
				break;
			case 3:
				wait(time, name);
				break;
			case 4:
				leave(time, name);

			}
		}
		end_leave();

		fin.close();
		printf("%02d:%02d\n", end_time.tm_hour, end_time.tm_min);
		for (int i = 0; i < tables.size(); ++i) {
			int hours = tables[i].time/3600;
			int minutes = (tables[i].time -hours*3600)/ 60;
			tm sum_time;
			sum_time.tm_hour = hours;
			sum_time.tm_min = minutes;
			if (minutes != 0) {
				tables[i].sum = (hours + 1) * price;
			}
			else {
				tables[i].sum = hours * price;
			}
			std::cout << i + 1 <<" "<< tables[i].sum <<" ";
			//<< tables[i].time << std::endl
			printf("%02d:%02d\n", sum_time.tm_hour, sum_time.tm_min);
		}
	}
	else {
		std::cout << "Ïðîáëåìà ñ ôàéëîì" << std::endl;
	}

}

void Comp_Club::parse_string(std::string line)
{
}

void Comp_Club::came(tm time, std::string name)
{
	std::cout << time.tm_hour << ":" << time.tm_min << " 1 " << name << " " << std::endl;
	if (time.tm_hour < start_time.tm_hour||
		(time.tm_hour == start_time.tm_hour&&time.tm_min<start_time.tm_min)) {
		error("NotOpenYet",time);
	}

	else if (search_client(name)) {
		error("YouShallNotPass",time);
	}
	else {
		cl_wait.push(name);
	}
}

void Comp_Club::sit(tm time, std::string name, int table, int code)
{
	printf("%02d:%02d", time.tm_hour, time.tm_min);
	std::cout << " " << code << " " << name << " " << table << std::endl;

	if (tables[table - 1].is_free) {
		if (search_client(name) || (!cl_wait.empty() && cl_wait.front() == name)) {
			cl_wait.pop();
			tables[table - 1].is_free = false;
			tables[table - 1].client = name;
			clients.push_back(name);
			tables[table - 1].start = time;
		}
		else {
			error("ClientUnknown", time);
		}
	}
	else {
		error("PlaceIsBusy", time);
	}
}



void Comp_Club::wait(tm time, std::string name)
{
	printf("%02d:%02d", time.tm_hour, time.tm_min);
	std::cout << " 3 " << name << std::endl;
	if (cl_wait.size() > num_table) {
		leave(time, name);
	}
	else {
		for (int i = 0; i < tables.size(); ++i) {
			if (tables[i].is_free) {
				error("ICanWaitNoLonger!", time);
				break;
			}
		}
	}
}

void Comp_Club::leave(tm time, std::string name)
{
	printf("%02d:%02d", time.tm_hour, time.tm_min);
	std::cout << " 4 " << name << std::endl;
	if (search_client(name)||cl_wait.front()==name) {
		int free_table=-1;
		for (int i = 0; i < tables.size(); ++i) {
			if (tables[i].client == name) {
				tables[i].is_free = true;
				time_t mytime = std::time(NULL);
				tm st_t;
				localtime_s(&st_t, &mytime);
				tm t_t;
				localtime_s(&t_t,&mytime);
				st_t.tm_hour = tables[i].start.tm_hour;
				st_t.tm_min = tables[i].start.tm_min;
				t_t.tm_hour = time.tm_hour;
				t_t.tm_min = time.tm_min;
				std::time_t t = std::mktime(&t_t);
				std::time_t s = std::mktime(&st_t);
				tables[i].time += difftime(t,s);
				free_table = i+1;
				break;
			}
		}
			for (int i = 0; i < clients.size(); ++i) {
				if (clients[i] == name) {
					clients.erase(clients.begin() + i);
					break;
				}
		}
		if (!cl_wait.empty()) {
			sit(time, cl_wait.front(),free_table,12);
			//cl_wait.pop();
		}
	}
	else {
		error("ClientUnknown", time);
	}
}

void Comp_Club::error(std::string message, tm time)
{
	printf("%02d:%02d", time.tm_hour, time.tm_min);
	std::cout <<" 13 "<< message << std::endl;
}

tm Comp_Club::parse_time(std::string str_time)
{
	tm time;
	std::string temp;
	temp += str_time[0];
	temp += str_time[1];
	time.tm_hour = stoi(temp);
	temp = "";
	temp += str_time[3];
	temp += str_time[4];
	time.tm_min = stoi(temp);
	return time;
}




bool Comp_Club::search_client(std::string name)
{
	for (int i = 0; i < clients.size(); ++i) {
		if (clients[i] == name) {
			return true;
		}
	}
	return false;
}

void Comp_Club::end_leave()
{
	if (!clients.empty()) {
		std::sort(begin(clients), end(clients));
		for (int i = 0; i < tables.size(); ++i) {
			if (!tables[i].is_free) {
				printf("%02d:%02d", end_time.tm_hour, end_time.tm_min);
				std::cout << " 11 " << tables[i].client << std::endl;
				time_t mytime = std::time(NULL);
				tm st_t;
				localtime_s(&st_t, &mytime);
				tm t_t;
				localtime_s(&t_t, &mytime);
				st_t.tm_hour = tables[i].start.tm_hour;
				st_t.tm_min = tables[i].start.tm_min;
				t_t.tm_hour = end_time.tm_hour;
				t_t.tm_min = end_time.tm_min;
				std::time_t t = std::mktime(&t_t);
				std::time_t s = std::mktime(&st_t);
				tables[i].time = difftime(t, s);
			}
		}
	}
}

std::string Comp_Club::convert_time(tm* time)
{
	char str[5];

	strftime(str, 5, "%I:%M%", time);
	std::string res;
	for (int i = 0; i < 5; ++i)
		res += str[i];
	return res;
}


