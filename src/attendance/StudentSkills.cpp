#include <imgui/imgui.h>
#include <attendance/StudentSkills.hpp>
#include <imgui/imgui.h>
#include <iostream>

std::vector<StudentSkills::Category> StudentSkills::design = {
	{
		.name = "Navigate Fusion",
		.nodes = {
			"Have a Fusion account and access to the team",
			"Navigate the Fusion team",
			"Use mouse functions + effectively view and navigate a fusion model",
			"Measure and build robot subsystems using a reference model",
			"Use read only for me, save, and use versions and version history",
			"Have completed the CAD Standardization lesson",
		},
	},
	{
		.name = "Use Fusion as a tool",
		.nodes = {
			"Have completed Fusion 101-102 lesson",
			"Know how to change units",
			"Create and edit sketches",
			"Use constraints, projections, etc",
			"Use extrusions and direct 3D objects",
			"Use correct file types and download files for 3D printing and laser cutting",
			"Download and import components from vendors",
			"Use tolerances for different materials",
		}
	},
	{
		.name = "CAD Parts",
		.nodes = {
			"Create drawings from designs",
			"Create bill of materials",
			"Use and properly constrain joints",
			"Use external components",
			"Derive components and bodies",
			"Use mirrors and patterns",
			"Use fillets and chamfers",
			"Use splits and combines",
			"Use assembly contexts",
		}
	},
	{
		.name = "CAD Complex Parts",
		.nodes = {
			"Create bodies out of sheet metal: flanges, bends, and flat patterns",
			"View and edit sheet metal rules",
			"Use parameters",
			"Construct planes and points",
			"Use section analysis",
			"Copy/paste CAM",
			"Post process existing CAM files and print setup sheets",
		},
	},
	{
		.name = "CAD Subsystems",
		.nodes = {
			"System integrate",
			"Display center of mass, mass, volume, moment of inertia",
			"Change densities, physical materials, and appearances",
			"Render designs",
			"Create and analyze static stress simulations",
			"CAM for the Camaster: bores and 2D contour",
		}
	}
};

std::vector<StudentSkills::Category> StudentSkills::manufacturing = {
	{
		.name = "Horizontal Bandsaw Safety",
		.nodes = {
			"Blade guard",
			"Listen for noises",
			"Push stick",
		}
	},
	{
		.name = "Horizontal Bandsaw Running",
		.nodes = {
			"Push stick",
			"Push assists",
		}
	},
	{
		.name = "Horizontal Bandsaw Maintenance",
		.nodes = {
			"Blade cleaning",
			"Blade changing",
		}
	},
	{
		.name = "Vertical Bandsaw Running",
		.nodes = {
			"Coolant",
			"Jack stand",
			"Speed",
			"Clamp all the way",
		}
	},
	{
		.name = "Vertical Bandsaw Maintenance",
		.nodes = {
			"Fill coolant",
			"Change blade",
		}
	},
	{
		.name = "Lathe Safety",
		.nodes = {
			"TAKE THE KEY OUT",
		}
	},
	{
		.name = "Lathe Running",
		.nodes = {
			"Speeds and feeds",
			"Tools",
			"Types of operations",
		}
	},
	{
		.name = "Lathe Maintenance",
		.nodes = {
			"Clean steel regularly (keep dry and rust free)",
			"Ensure bits on tools are sharp",
		}
	},
	{
		.name = "CAMaster Safety",
		.nodes = {
			"E-stop",
			"Screw in correctly",
		}
	},
	{
		.name = "CAMaster Running",
		.nodes = {
			"Change tools",
			"Touch top ",
			"Tubes",
			"Sheets",
			"Run machine",
			"With supervision",
			"Without supervision but checks",
			"Completely free"
		}
	},
	{
		.name = "CAMaster Maintenance",
		.nodes = {
			"Coolant filling",
			"Getting new tools",
			"Turn off all the way safely",
		}
	},
	{
		.name = "CAMaster CAM",
		.nodes = {
			"With help",
			"Without help",
		}
	},
	{
		.name = "Tormach Safety",
		.nodes = {
			"Running",
			"With supervision",
			"Without supervision but checks",
			"Completely free",
		}
	},
	{
		.name = "Tormach Maintenance",
		.nodes = {
			"Oil skimmer",
			"Cleaning aluminum off",
			"Changing tools in tool holders",
		}
	},
	{
		.name = "Tormach CAM",
		.nodes = {
			"With help",
			"Without help",
		}
	}
};

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

std::vector<StudentSkills::Category> StudentSkills::marketing = {
	{
		.name = "Canva",
		.nodes = {
			"Making small designs (i.e; Lion Burgers tickets, dine out cards, etc;)",
			"Design posters for camps/social media",
			"Design promotional posters for competitions",
		}
	},
	{
		.name = "Adobe Illustrator",
		.nodes = {
			"Trace a basic design",
			"Sponsor logo magnets and stickers",
			"T-shirt design",
		}
	},
	{
		.name = "Printing/Cutting",
		.nodes = {
			"Use a printer",
			"Make a button using the button machine",
			"Use the cricut machine to print stickers",
			"Print magnets using digital arts room printer",
		}
	},
	{
		.name = "Facebook/Tik Tok/Instagram	",
		.nodes = {
			"Write a prompt for social media",
			"Post on social media",
		}
	},
	{
		.name = "GoDaddy",
		.nodes = {
			"Update the main page of the website",
			"Edit different sections of the website",
		}
	},
	{
		.name = "Newsletter (Team & ROAR)",
		.nodes = {
			"Write a prompt for the live and written ROAR",
			"Submit a live and written ROAR blurb via gmail",
			"Update The Hub",
			"Update the newsletter",
			"Send out the newsletter email",
		}
	},
	{
		.name = "Media Release and competition invite",
		.nodes = {
			"Write a competition invite prompt",
			"Send a competition invitation",
			"Write a media release prompt",
			"Send a media release email",
		}
	}
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

std::vector<StudentSkills::Category> StudentSkills::safety = {
	{
		.name = "General",
		.nodes = {
			"Fill out MIFS",
			"General Safety Training",
			"Safety Audit",
			"Enter MIFs into sheet",
		}
	},
	{
		.name = "Emergancy",
		.nodes = {
			"Emergency safety training",
			"Check first aid kit",
			"Knows all of the lab procedures",
			"First Aid trained",
			"CPR trained",
		}
	},
	{
		.name = "Competition",
		.nodes = {
			"Pack safety bin",
			"Is able to talk to other teams about safety",
			"Know all of our procedures to talk in detail to other teams",
			"Is the designated safety captain",
		}
	},
	{
		.name = "Organization",
		.nodes = {
			"Organize the safety cubbies",
			"Knowing the inventory",
			"Order supplies",
			"Teaching trainings",
			"Keep all materials updated",
			"Write safety audits",
			"Keep track of injuries recurring or not",
		}
	}
};

std::vector<StudentSkills::Category> StudentSkills::strategy = {
	{
		.name = "Scouting and Strategy",
		.nodes = {
			"Use scouting website",
			"Accurately scouts qualitatively",
			"Compiles and Analyzes data into match strategy",
			"Analyzes games for strategy and priorities",
			"Makes data-driven decisions"
		}
	}
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
		if (ImGui::BeginTabItem("Design")) {
			f(design, s.design);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Manufacturing")) {
			f(manufacturing, s.manufacturing);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Electrical")) {
			f(electrical, s.electrical);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Software")) {
			f(software, s.software);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Marketing")) {
			f(marketing, s.marketing);
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
		if (ImGui::BeginTabItem("Safety")) {
			f(safety, s.safety);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Strategy")) {
			f(strategy, s.strategy);
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