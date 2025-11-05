#ifndef ADDSTUDENT_HPP
#define ADDSTUDENT_HPP

#include <attendance/Database.hpp>
#include <algorithm>

class AddStudent {
public:
	AddStudent(Database& db) : db(db) {
		std::fill(first, first + sizeof(first), '\0');
		std::fill(last, last + sizeof(last), '\0');
	}

	void render();

	char first[128];
	char last[128];

private:
	Database& db;
};

#endif