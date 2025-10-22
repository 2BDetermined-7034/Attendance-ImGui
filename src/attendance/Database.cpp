#include <attendance/Database.hpp>
#include <fstream>
#include <cstring>
#include <iostream>
#include <functional>
#include <mstd/memory>
#include <GLFW/glfw3.h>

static void logError(const std::string& filepath, const std::string& error) {
	std::cerr << errorText << "Could not open file \"" << filepath << "\" " << error << std::endl;
}

std::vector<Database::V> Database::versions = {
	&Database::version1
};

/*
 * Adding new versions:
 * First, decide if your changes are significant enough that they constitute a new function.
 * If so, create a new function named versionXXX with the same signature as version1.
 * Then, add it to the function array.
 * To modify an existing function, make sure to check for your revision number using >=
 * so that those changes may persist into future revisions.
 *
 * Be sure to edit the write function to the latest version.
 */

static constexpr mstd::U16 latestRevision = 0;

mstd::Status Database::read(const std::string& filepath) {
	using namespace mstd;

	struct FileHeader {
		C8 type[4]; // Always equal to "DB2", our team name backwards
		U16 version; // Major version
		U16 revision; // Revision
	} header;

	std::ifstream file(filepath);
	if (!file.is_open()) {
		logError(filepath, "");
		return 1;
	}

	file.read((C8*)&header, sizeof(FileHeader));

	if (strcmp(header.type, "DB2")) {
		logError(filepath, "Invalid database file");
		return 1;
	}

	if (header.version > versions.size()) {
		logError(filepath, "Invalid version number");
		return 1;
	}

	auto versionFunc = versions[header.version - 1];
	if ((versionFunc)(this, file, header.revision)) {
		return 1;
	}

	return 0;
}

mstd::Status Database::write(const std::string& filepath) {
	using namespace mstd;

	std::ofstream file(filepath, std::ios::trunc);
	if (!file.is_open()) {
		logError(filepath, "");
		return 1;
	}

	struct FileHeader {
		C8 type[4] = "DB2";
		U16 version = versions.size();
		U16 revision = latestRevision;
	} header;

	struct SubHeader {
		U32 studentCount;
		U32 stringChunkSize;
	} subHeader;
	subHeader.studentCount = students.size();

	/*
	 * What is going on here?
	 * To start, a large, uninitialized chunk of memory is reserved using the Arena class.
	 * Then, each of the students' names are added to the arena. Keep in mind the size() + 1
	 * also ensures that the null character '\0' is added at the end.
	 *
	 * We subtract the first pointer from the result of arena.append(...) so that we recieve
	 * the offset from that first element as opposed to a pointer. Remember, pointers are not
	 * valid in files so we use array indices instead.
	 */
	Arena arena(1L << 24);
	Size first = (Size)arena.tell<C8>(); // Request the memory address of the arena chunk.
	for (Size i = 0; i < firstNames.size(); ++i) {
		students[i].firstName =
			(Size)arena.append(firstNames[i].size() + 1, firstNames[i].data()) - first;

		students[i].lastName =
			(Size)arena.append(lastNames[i].size() + 1, lastNames[i].data()) - first;
	}

	subHeader.stringChunkSize = (Size)arena.tell<C8>() - first;

	file.write((C8*)&header, sizeof(header));
	file.write((C8*)&subHeader, sizeof(subHeader));
	file.write((C8*)students.data(), students.size() * sizeof(Student));
	file.write((C8*)first, subHeader.stringChunkSize);

	return 0;
}

mstd::Status Database::version1(std::ifstream& file, mstd::U16 revision) {
	using namespace mstd;

	struct SubHeader {
		U32 studentCount;
		U32 stringChunkSize;
	} subHeader;

	file.read((C8*)&subHeader, sizeof(SubHeader));

	students.resize(subHeader.studentCount);
	file.read((C8*)students.data(), students.size() * sizeof(Student));

	firstNames.clear();
	lastNames.clear();
	firstNames.reserve(students.size());
	lastNames.reserve(students.size());

	Arena arena(subHeader.stringChunkSize);
	C8* stringChunk = arena.tell<C8>();
	file.read(arena.reserve<C8>(subHeader.stringChunkSize), subHeader.stringChunkSize);

	for (Size i = 0; i < students.size(); ++i) {
		Student& s = students[i];
		firstNames.push_back(std::string(stringChunk + s.firstName));
		lastNames.push_back(std::string(stringChunk + s.lastName));
	}

	return 0;
}

mstd::Status Database::import(const std::string& filepath) {
	using namespace mstd;

	std::ifstream file(filepath);
	if (!file.is_open()) {
		logError(filepath, "");
		return 1;
	}

	std::string source;
	file.seekg(0, std::ios::end);
	source.resize(file.tellg());
	file.seekg(0, std::ios::beg);

	file.read(source.data(), source.size());

	Size dataStartOffset;
	for (Size i = 0; i < source.size(); ++i) {
		if (source[i] == '\n') {
			dataStartOffset = i + 1;
			break;
		}
	}

	enum State : U32 {
		TIMESTAMP = 0,
		NAME = 1,
		ACTIVITY = 2,
		HOURS = 3,
		DATE = 4,
		DETAILS = 5,
	};

	U32 state = TIMESTAMP;

	C8* start = source.data() + dataStartOffset;
	C8* end;

	std::vector<std::string> names;
	names.resize(students.size());

	for (Size i = 0; i < students.size(); ++i) {
		std::cout << students[i].lastName << std::endl;
		std::cout << firstNames[students[i].firstName] + " " + lastNames[students[i].lastName] << std::endl;
	}
	return 0;

	Student* currentStudent;
	for (Size i = dataStartOffset; i < source.size(); ++i) {
		Bool controlCharacter = false;
		if (source[i] == '\n') {
			state = TIMESTAMP;
			controlCharacter = true;
		} else if (source[i] == ',') {
			++state;
			controlCharacter = true;
		}
		if (controlCharacter) {
			++state;
			end = source.data() + i;

			std::string_view token = std::string_view(start + 1, end - 1);

			switch (state) {
			case NAME:
				currentStudent = nullptr;
				for (Size s = 0; s < names.size(); ++s) {
					if (token == names[s]) {
						currentStudent = students.data() + s;
						break;
					}
				}
			case TIMESTAMP:
			default:
				break;
			}

			if (currentStudent) {
				std::cout << firstNames[currentStudent->firstName] << std::endl;
			} else {
				std::cout << "NO STUDENT!!" << std::endl;
			}

			start = end + 1;
		}
	}

	return 0;
}