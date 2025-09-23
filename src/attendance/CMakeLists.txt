target_sources(
	attendance PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Database.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StudentSelector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backup.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AddStudent.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StudentSettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StudentSkills.cpp
)
