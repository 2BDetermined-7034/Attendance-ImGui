#include <attendance/StudentSkills.hpp>
#include <imgui/imgui.h>
#include <iostream>

void StudentSkills::render() {
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

void StudentSkills::renderStudent() {
	using namespace mstd;
	if (ImGui::BeginTabBar("Subteams")) {
		if (ImGui::BeginTabItem("Software")) {
			Size n = 0;
			for (Size i = 0; i < 3; ++i) {
				ImGui::Text("%s", softwareCategories[i]);
				while (softwareNodes[n] != nullptr) {
					ImGui::Checkbox(softwareNodes[n], (mstd::Bool*)completed.data() + n);
					++n;
				}
				++n;
				ImGui::Separator();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void StudentSkills::renderAdmin() {
}
