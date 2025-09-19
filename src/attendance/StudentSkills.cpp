#include "imgui/imgui_internal.h"
#include <attendance/StudentSkills.hpp>
#include <imgui/imgui.h>
#include <iostream>

std::vector<StudentSkills::Category> StudentSkills::electrical = {
	{
		.name = "Tools",
		.nodes = {
			"Learning names",
			"Electrical Safety",
			"Using a multimeter",
			"Battery capacity tester",
		},
	},
	{
		.name = "FRC Processes",
		.nodes = {
			"Learning the names of FRC components",
			"Wire a motor to Bobert (spaghetti bot)",
			"Robot schematic/wiring table",
			"Troubleshooting exercise in a controlled environment",
			"FRC Electrical Inspection",
		},
	},
	{
		.name = "Theoretical Knowledge",
		.nodes = {
			"Ohm’s law",
			"Power law",
			"Kirchoff’s Voltage and Current Laws",
			"DC vs. AC Power",
			"Learn about logic gates",
		},
	},
	{
		.name = "Electrial Engineering",
		.nodes = {
			"Lighting an LED with arduino",
			"Motor operation and theory",
			"PCB Design and Applications",
			"Passive Components",
			"Resistors, Resistivity, Color Codes, etc.",
			"Capacitors, Capacitance, Dielectric, etc",
			"Inductors, Inductance, Magnetism, etc. ",
			"Diodes!",
			"Control Theory",
			"Semiconductors",
		}
	},
};

std::vector<StudentSkills::Category> StudentSkills::software = {
	{
		.name = "General Skills",
		.nodes = {
			"Driver Station", 
			"Phoenix Tuner", 
			"Camera Calibration", 
			"Radio Flashing",
		},
	},
	{
		.name = "Control Theory",
		.nodes = {
			"PID Loops", 
			"Feedforward Types", 
			"Trapezoidal Motion",
		},
	},
	{
		.name = "Programming",
		.nodes = {
			"Basic Java", 
			"Command-based & Lambdas",
		},
	},
};

std::vector<StudentSkills::Category> StudentSkills::outreach = {
	{
		.name = "General Skills",
		.nodes = {
			"Learn the whole spiel to give to literally everyone at all times :)",
			"General email etiquette (sign offs, being formal, etc.)",
			"General Google Sheets skills",
			"Basic photography",
		}
	},
	{
		.name = "Camps",
		.nodes = {
			"Choosing dates",
			"Google forms + email notifications for forms",
			"Creating planning sheets",
			"Choosing lessons + organizing teachers/lessons",
			"Notifying the team + camper parents (help marketing media release)",
			"Put people in groups WITHOUT parents yelling at you",
			"Basic Canva - posters, nametags, etc.",
		}
	},
	{
		.name = "Impact",
		.nodes = {
			"Brainstorming/creating a theme",
			"Outlining - creating flow, choosing what events to mention, etc.",
			"Actually writing the essay - word choice, writing clearly and persuasively",
			"Editing!",
			"Presentation/video - conveying ideas in an engaging way",
			"Memorizing scripts",
			"Video filming",
			"Video editing",
			"Answering judge’s questions",
		}
	},
	{
		.name = "Other Events",
		.nodes = {
			"Choosing dates",
			"Google forms",
			"Creating planning sheets",
			"Organizing staff",
			"Notifying the team",
			"Finding materials etc.",
		}
	}
};

std::vector<StudentSkills::Category> StudentSkills::business = {
	{
		.name = "Sponsorships/Grants",
		.nodes = {
			"Scout potential sponsors online",
			"Send sponsor emails",
			"Visit in-person",
			"Create sponsorship paraphernalia",
			"Find grants",
			"Fill out grants",
		},
	},
	{
		.name = "Event Management",
		.nodes = {
			"Plan a dine-out",
			"Cashbox management",
			"Plan LB, OTF, or DARD",
		},
	},
	{
		.name = "Business Duties",
		.nodes = {
			"Receipt categorizing",
			"Quickbooks categorizing",
			"Build a budget",
			"SWOT analysis",
			"Make a business plan",
			"Business summary",
		},
	},
};

void StudentSkills::render() {
	using namespace mstd;

	if (!enabled) return;
	
	ImGui::SetNextWindowSizeConstraints({400, 300}, {FLT_MAX, FLT_MAX});
	if (ImGui::Begin(name.c_str(), &enabled)) {
		Database::Student& s = db.students[studentIndex];
		if (hasAdmin) {
			renderSubteams(
				[&](std::vector<Category> c, U8* d) {
					renderSubteamAdmin(c, d);
				}
			);
		} else {
			renderSubteams(
				[&](std::vector<Category> c, U8* d) {
					renderSubteamStudent(c, d);
				}
			);
		}
	}
	ImGui::End();
}

void StudentSkills::renderSubteams(std::function<void(std::vector<Category>&, mstd::U8*)> f) {
	using namespace mstd;

	Database::Student& s = db.students[studentIndex];

	if (ImGui::BeginTabBar("Subteams")) {
		if (ImGui::BeginTabItem("Electrical")) {
			f(electrical, s.electrical);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Software")) {
			f(software, s.software);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Outreach")) {
			f(outreach, s.outreach);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Business")) {
			f(business, s.business);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void StudentSkills::renderSubteamStudent(std::vector<Category>& categories, mstd::U8* data) {
	using namespace mstd;

	Size n = 0;
	for (Size i = 0; i < categories.size(); ++i) {
		const Category& c = categories[i];
		ImGui::Text("%s", c.name);

		for (Size n = 0; n < c.nodes.size(); ++n) {
			Bool t = true;
			Bool f = false;

			ImGui::Checkbox(c.nodes[n], data[i] > n ? &t : &f);
		}

		ImGui::Separator();
	}
}

void StudentSkills::renderSubteamAdmin(std::vector<Category>& categories, mstd::U8* data) {
	using namespace mstd;

	Size n = 0;
	for (Size i = 0; i < categories.size(); ++i) {
		const Category& c = categories[i];
		ImGui::Text("%s", c.name);

		for (Size n = 0; n < c.nodes.size(); ++n) {
			Bool t = true;
			Bool f = false;

			Bool lower = true;
			Bool upper = false;

			if (data[i] - 1 == n) {
				ImGui::Checkbox(c.nodes[n], &lower);
			} else if (data[i] > n) {
				ImGui::Checkbox(c.nodes[n], &t);
			} else if (data[i] == n) {
				ImGui::Checkbox(c.nodes[n], &upper);
			} else {
				ImGui::Checkbox(c.nodes[n], &f);
			}

			if (!lower) {
				data[i] = (U8)n;
			} else if (upper) {
				data[i] = (U8)n + 1;
			}
		}

		ImGui::Separator();
	}
}
