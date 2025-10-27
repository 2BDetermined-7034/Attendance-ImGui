#include <attendance/AddStudent.hpp>

#include <imgui/imgui.h>

void AddStudent::render() {
	using namespace mstd;
	
	ImGui::InputText("First", first, sizeof(first));
	ImGui::InputText("Last", last, sizeof(last));
	if (ImGui::Button("Add")) {
		db.students.push_back({});
		db.firstNames.push_back(first);
		db.lastNames.push_back(last);

		std::fill(first, first + sizeof(first), '\0');
		std::fill(last, last + sizeof(last), '\0');

		for (Size s = 0; s < db.shifts.size(); ++s) {
			db.shifts[s].push_back({0xFF, 0});
		}
	}
}