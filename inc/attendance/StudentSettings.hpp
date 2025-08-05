#ifndef STUDENTSETTINGS_HPP
#define STUDENTSETTINGS_HPP

#include <attendance/Database.hpp>

class StudentSettings {
public:
	StudentSettings(Database& db) : db(db) {
		enabled = false;
		attemptingToDelete = false;
		hasAdmin = false;
		std::fill(deleteQuery, deleteQuery + sizeof(deleteQuery), '\0');
	}

	void render();

	void toggle(mstd::Size studentIndex) {
		this->studentIndex = studentIndex;
		enabled ^= 1;

		name = db.firstNames[studentIndex] + " " + db.lastNames[studentIndex];
	}

	mstd::Bool hasAdmin;

private:
	void renderAdmin();
	void renderStudent();

	mstd::Size studentIndex;
	mstd::Bool attemptingToDelete;
	mstd::Bool enabled;

	Database& db;
	std::string name;
	char deleteQuery[128];
};

#endif
