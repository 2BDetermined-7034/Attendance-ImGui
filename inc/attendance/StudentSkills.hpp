#ifndef STUDENTSKILLS_HPP
#define STUDENTSKILLS_HPP

#include <attendance/Database.hpp>
#include <vector>

class StudentSkills {
public:
	StudentSkills(Database& db) : db(db) {
		enabled = false;
		hasAdmin = false;
	}

	void render();

	void toggle(mstd::Size studentIndex) {
		this->studentIndex = studentIndex;
		enabled ^= 1;

		name = db.firstNames[studentIndex] + " " + db.lastNames[studentIndex];

		const Database::Student& s = db.students[studentIndex];
	}

	mstd::Bool hasAdmin;

private:
	struct Category {
		const char* name;
		std::vector<const char*> nodes;
	};

	static std::vector<Category> software;
	static std::vector<Category> business;
	void renderSubteams(std::function<void(std::vector<Category>&, mstd::U8*)> f);
	void renderSubteamStudent(std::vector<Category>& categories, mstd::U8* data);
	void renderSubteamAdmin(std::vector<Category>& categories, mstd::U8* data);

	mstd::Size studentIndex;
	mstd::Bool enabled;

	std::string name;
	Database& db;
};

#endif
