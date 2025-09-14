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
	}

	mstd::Bool hasAdmin;

private:

	static constexpr const char* softwareCategories[] = {
		"General Skills",
		"Control Theory",
		"Programming"
	};
	
	// nullptr is a delimiter in this array
	static constexpr const char* softwareNodes[] = {
		"Driver Station", "Phoenix Tuner", "Camera Calibration", "Radio Flashing", nullptr,
		"PID Loops", "Feedforward Types", "Trapezoidal Motion", nullptr,
	};

	void renderAdmin();
	void renderStudent();

	mstd::Size studentIndex;
	mstd::Bool enabled;

	std::array<mstd::Bool, sizeof(softwareNodes) / sizeof(softwareNodes[0])> completed;

	std::string name;
	Database& db;
};

#endif
