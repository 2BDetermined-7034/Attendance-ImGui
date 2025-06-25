cmake_minimum_required(VERSION 3.28)

project(
	Attendance VERSION 1.0.0
	DESCRIPTION "FRC 7034 Attendance application"
	HOMEPAGE_URL ""
)

include(ExternalProject)
include(FetchContent)

set(INSTALL_LOCATION ${CMAKE_BINARY_DIR}/deps)

FetchContent_Declare(
	mstd
	GIT_REPOSITORY https://github.com/mopo3ob-jack/mstd.git
	EXCLUDE_FROM_ALL
	SYSTEM
)
FetchContent_MakeAvailable(mstd)

FetchContent_Declare(
	SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 3.0.1
	EXCLUDE_FROM_ALL
	SYSTEM
)
FetchContent_MakeAvailable(SFML)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "inc" "*.hpp" "*.h")

add_executable(attendance)
add_subdirectory(src/attendance)
target_include_directories(attendance PUBLIC inc/)
target_link_libraries(attendance PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Network mstd)
set_target_properties(attendance PROPERTIES CXX_STANDARD 20)
