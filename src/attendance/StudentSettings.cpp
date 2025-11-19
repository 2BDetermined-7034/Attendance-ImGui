#include <attendance/StudentSettings.hpp>
#include <iostream>

#include <imgui/imgui.h>

void StudentSettings::render() {
	using namespace mstd;

	if (!enabled) return;
	
	ImGui::SetNextWindowSizeConstraints({400, 300}, {FLT_MAX, FLT_MAX});
	if (ImGui::Begin(name.c_str(), &enabled)) {
		if (hasAdmin) {
			renderAdmin();
		} else {
			renderStudent();
		}
	}
	ImGui::End();
}

void StudentSettings::renderAdmin() {
	using namespace mstd;

	ImGui::SeparatorText("Lab");
	ImGui::InputFloat("Lab hours", &db.students[studentIndex].labHours, 3.0f);
	ImGui::SeparatorText("Service");
	ImGui::InputFloat("SOLVE hours", &db.students[studentIndex].solveHours, 3.0f);
	ImGui::InputFloat("Camp hours", &db.students[studentIndex].campHours, 1.0f);
	ImGui::InputFloat("Other hours", &db.students[studentIndex].otherHours, 1.0f);
	ImGui::Text(
		"TOTAL = %f",
		db.students[studentIndex].solveHours + 
		db.students[studentIndex].campHours + 
		db.students[studentIndex].otherHours
	);
	ImGui::InputInt("Solve events", (I32*)&db.students[studentIndex].solveEvents);
	ImGui::InputInt("Camps", (I32*)&db.students[studentIndex].camps);
	ImGui::InputInt("Other", (I32*)&db.students[studentIndex].other);

	ImGui::PushStyleColor(ImGuiCol_Separator, {0.875f, 0.125f, 0.125f, 0.25f});
	ImGui::PushStyleColor(ImGuiCol_Text, {0.875f, 0.125f, 0.125f, 1.0f});
	ImGui::SeparatorText("Danger Zone");
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	if (ImGui::Button("Rename")) {
		attemptingToRename ^= 1;
	}
	if (attemptingToRename) {
		std::string& firstName = db.firstNames[studentIndex];
		std::string& lastName = db.lastNames[studentIndex];

		/*
		* Copy from the string beginning to the string end + 1
		* in order to also encapsulate the null byte '\0'.
		* We do this so that we trick ImGui to think that the query
		* buffers have been properly cleared when in fact they were not.
		*/
		std::copy(firstName.begin(), firstName.end() + 1, firstQuery);
		std::copy(lastName.begin(), lastName.end() + 1, lastQuery);
		ImGui::InputText("First ", firstQuery, 64);
		ImGui::InputText("Last ", lastQuery, 64);
		if (ImGui::Button("Ok")) {
			firstName.resize(std::strlen(firstQuery));
			lastName.resize(std::strlen(lastQuery));

			std::copy(firstQuery, firstQuery + firstName.size(), firstName.begin());
			std::copy(lastQuery, lastQuery + lastName.size(), lastName.begin());
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			attemptingToDelete = 0;
		}
	}
	if (ImGui::Button("Delete")) {
		attemptingToDelete ^= 1;
	}
	if (attemptingToDelete) {
		std::string deleteText = "Delete " + name;
		ImGui::InputText(
			(("Type \"") + deleteText + ("\" to continue")).c_str(),
			deleteQuery,
			128
		);
		if (ImGui::Button("Ok")) {
			if (deleteText == deleteQuery) {
				db.students.erase(db.students.begin() + studentIndex);
				db.firstNames.erase(db.firstNames.begin() + studentIndex);
				db.lastNames.erase(db.lastNames.begin() +  studentIndex);

				for (Size d = 0; d < db.shifts.size(); ++d) {
					db.shifts[d].erase(db.shifts[d].begin() + studentIndex);
				}

				attemptingToDelete = 0;
				enabled = 0;
				std::fill(deleteQuery, deleteQuery + sizeof(deleteQuery), '\0');
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			attemptingToDelete = 0;
		}
	}
}

void StudentSettings::renderStudent() {
	using namespace mstd;

	ImGui::SeparatorText("Lab");
	ImGui::Text("Lab hours = %f", db.students[studentIndex].labHours);
	ImGui::SeparatorText("Service");
	ImGui::Text("SOLVE hours = %f", db.students[studentIndex].solveHours);
	ImGui::Text("Camp hours = %f", db.students[studentIndex].campHours);
	ImGui::Text("Other hours = %f", db.students[studentIndex].otherHours);
	ImGui::Text(
		"TOTAL = %f",
		db.students[studentIndex].solveHours + 
		db.students[studentIndex].campHours + 
		db.students[studentIndex].otherHours
	);
	ImGui::Text("Solve events = %d", db.students[studentIndex].solveEvents);
	ImGui::Text("Camps = %d", db.students[studentIndex].camps);
	ImGui::Text("Other = %d", db.students[studentIndex].other);
}