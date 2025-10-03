#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <attendance/Database.hpp>
#include <attendance/StudentSelector.hpp>
#include <attendance/backup.hpp>
#include <attendance/AddStudent.hpp>
#include <attendance/StudentSettings.hpp>
#include <attendance/StudentSkills.hpp>

#include <iostream>
#include <functional>

mstd::I32 focused = 1;
void focusCallback(GLFWwindow* window, mstd::I32 f) {
	focused = f;
}

int main() {
	using namespace mstd;

	if (!glfwInit()) {
		std::cerr << errorText << "Could not initialize GLFW" << std::endl;
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Attendance", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << errorText << "Could not initialize GLAD" << std::endl;
		glfwTerminate();
		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(1.5f);
	style.FontScaleDpi = 1.5f; 

	style.FontSizeBase = 15.0f;
	if (!io.Fonts->AddFontFromFileTTF("resources/font.ttf")) {
		std::cout << errorText << "Could not load font" << std::endl;
		return 0;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Enable V-Sync
	glfwSwapInterval(1);

	Database db;
	db.read("database.db2");

	AddStudent addStudent(db);
	StudentSettings settings(db);
	StudentSkills skills(db);
	StudentSelector selector(db, settings, skills);

	glfwSetWindowFocusCallback(window, focusCallback);
	
	I32 width, height;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (!focused) {
			continue;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Save")) {
				db.write("database.db2");
			}
			if (ImGui::MenuItem("Backup")) {
				backupCreate(db);
			}
			if (ImGui::MenuItem("Restore")) {
				backupRestore(db);
			}
			if (ImGui::MenuItem("Exit")) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			ImGui::EndMenu();
		}
		if (settings.hasAdmin) {
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::BeginMenu("Add Student")) {
					addStudent.render();
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Sign All Out")) {
					for (Size i = 0; i < db.students.size(); ++i) {
						selector.signOut(i);
					}
				}
				ImGui::EndMenu();
			}
		}
		if (ImGui::BeginMenu("Admin")) {
			if (ImGui::BeginMenu("Sign In")) {
				if (
					char password[64] = "";
					ImGui::InputText(
						"Password", password, 64,
						ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue
					)
				) {
					if (password == std::string("7034")) {
						settings.hasAdmin = true;
						skills.hasAdmin = true;
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Sign Out")) {
				settings.hasAdmin = false;
				skills.hasAdmin = false;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		Bool isOpen;
		ImGui::Begin("Main", &isOpen,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBringToFrontOnFocus
		);

		ImGui::SetWindowPos({0, 32});
		ImGui::SetWindowSize(io.DisplaySize);

		selector.render();
		settings.render();
		skills.render();

		ImGui::End();

		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/*
		 * If the application is left on for over an hour, reset the clock and autosave.
		 * TODO Move this logic to execute after signing everyone out at ~9-10 automatically
		 */
		if (glfwGetTime() > 3600.0f) {
			selector.resetClock();
			db.write("database.db2");
		}

		glfwSwapBuffers(window);
	}

	for (Size i = 0; i < db.students.size(); ++i) {
		selector.signOut(i);
	}

	db.write("database.db2");

	glfwTerminate();

	return 0;
}
