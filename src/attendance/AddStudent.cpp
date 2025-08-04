#include <attendance/AddStudent.hpp>

#include <imgui/imgui.h>

void AddStudent::render() {
	ImGui::InputText("First", first, sizeof(first));
	ImGui::InputText("Last", last, sizeof(last));
	if (ImGui::Button("Add")) {
		db.students.push_back({});
		db.firstNames.push_back(first);
		db.lastNames.push_back(last);

		std::fill(first, first + sizeof(first), '\0');
		std::fill(last, last + sizeof(last), '\0');
	}
}
