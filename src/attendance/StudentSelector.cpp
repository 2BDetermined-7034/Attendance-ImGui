#include <attendance/StudentSelector.hpp>

#include <imgui/imgui.h>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <ctime>

void StudentSelector::updateSearch() {
	using namespace mstd;

	students.resize(db.students.size());

	if (!ImGui::InputTextWithHint("Search", "Dean Kamen", search, sizeof(search))) {
		return;
	}

	if (search[0] == '\0') {
		for (auto& s : students) {
			s.searchResult = true;
		}
	} else {
		char lowercase[128];
		// Make the search result lowercase
		std::transform(search, search + sizeof(search), lowercase, tolower);

		// Go through the students and make their names lowercase to search for a match
		std::string name;
		for (Size i = 0; i < db.students.size(); ++i) {
			const std::string& first = db.firstNames[i];
			const std::string& last = db.lastNames[i];

			name.resize(first.size() + last.size() + 1);
			std::transform(first.begin(), first.end(), name.begin(), tolower);
			std::transform(last.begin(), last.end(), name.begin() + first.size() + 1, tolower);
			name[first.size()] = ' ';

			students[i].searchResult = name.find(lowercase) != std::string::npos;
		}
	}
}

void StudentSelector::render() {
	using namespace mstd;

	updateSearch();

	for (Size i = 0; i < students.size(); ++i) {
		if (!students[i].searchResult) continue;

		Database::Student& s = db.students[i];
		std::string& first = db.firstNames[i];
		std::string& last = db.lastNames[i];

		const std::string name = first + " " + last;

		constexpr ImU32 absentColor = ImGuiCol_Text;
		constexpr ImU32 presentColor = IM_COL32(0, 255, 0, 255);
		if (students[i].present) {
			ImGui::PushStyleColor(ImGuiCol_Text, presentColor);
			if (ImGui::Button(name.c_str(), {800, 0})) {
				toggleAttendance(i);
			}
			ImGui::PopStyleColor();
		} else {
			if (ImGui::Button(name.c_str(), {800, 0})) {
				toggleAttendance(i);
			}
		}
		continue;

		ImGui::PushID(name.c_str());

		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0.3, 0});
		if (ImGui::Button("Settings")) {
			settings.toggle(i);
		}
		ImGui::PopStyleVar();
		
		ImGui::SameLine();
		if (ImGui::Button("Skills")) {
			skills.toggle(i);
		}
		
		ImGui::PopID();
	}
}

void StudentSelector::toggleAttendance(mstd::Size student) {
	if (students[student].present) {
		signOut(student);
	} else {
		signIn(student);
	}
}

void StudentSelector::signIn(mstd::Size student) {
	using namespace mstd;

	if (students[student].present) return;

	students[student].signInTime = glfwGetTime();
	students[student].present = true;

	db.addDate();
	if (db.shifts.back()[student].in.hour == 0xFF) {
		std::time_t timestamp;
		std::time(&timestamp);

		const tm* local = std::localtime(&timestamp);

		Database::Timestamp in = {
			.hour = U8(local->tm_hour),
			.minute = U8(local->tm_min)
		};
		db.shifts.back()[student].in = in;
	}
}

void StudentSelector::signOut(mstd::Size student) {
	using namespace mstd;

	if (!students[student].present) return;

	db.students[student].labHours += (glfwGetTime() - students[student].signInTime) / 3600.0f;

	students[student].signInTime = 0.0f;
	students[student].present = false;

	std::time_t timestamp;
	std::time(&timestamp);

	const tm* local = std::localtime(&timestamp);

	Database::Timestamp out = {
		.hour = U8(local->tm_hour),
		.minute = U8(local->tm_min)
	};
	db.shifts.back()[student].out = out;
}

/*
 * Resets the global GLFW time.
 * I don't know if this is necessary, but it would preserve floating point precision
 * should this application be left on for a very long amount of time.
 *
 * I haven't taken enough care to ensure that every single nanosecond is accounted for
 * in this operation. Fortunately for me, I do not care!
 */
void StudentSelector::resetClock() {
	using namespace mstd;

	for (Size i = 0; i < students.size(); ++i) {
		if (!students[i].present) continue;

		db.students[i].labHours += (glfwGetTime() - students[i].signInTime) / 3600.0f;
	}

	glfwSetTime(0.0f);
	for (Size i = 0; i < students.size(); ++i) {
		students[i].signInTime = 0.0f;
	}
}