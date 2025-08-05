#ifndef STUDENTSELECTOR_HPP
#define STUDENTSELECTOR_HPP

#include <attendance/Database.hpp>
#include <attendance/StudentSettings.hpp>

class StudentSelector {
public:
	StudentSelector(Database& db, StudentSettings& settings) : db(db), settings(settings) {
		students.resize(db.students.size());
		std::fill(search, search + sizeof(search), '\0');
	}

	void render();

	void toggleAttendance(mstd::Size student);
	void signIn(mstd::Size student);
	void signOut(mstd::Size student);
	void resetClock();

private:
	void updateSearch();

	struct StudentRuntimeData {
		StudentRuntimeData() {
			signInTime = 0.0f;
			searchResult = true;
			present = false;
		}

		mstd::F64 signInTime;
		mstd::U8 searchResult : 1;
		mstd::U8 present : 1;
	};

	Database& db;
	StudentSettings& settings;
	std::vector<StudentRuntimeData> students;
	mstd::C8 search[128];
};

#endif
