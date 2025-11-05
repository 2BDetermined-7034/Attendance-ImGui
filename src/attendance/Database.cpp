#include <attendance/Database.hpp>
#include <fstream>
#include <cstring>
#include <iostream>
#include <functional>
#include <mstd/memory>
#include <GLFW/glfw3.h>
#include <ctime>

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

	std::ifstream file(filepath, std::ios::binary);
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

	std::ofstream file(filepath, std::ios::trunc | std::ios::binary);
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
		U32 dateCount;
	} subHeader;
	subHeader.studentCount = students.size();
	subHeader.dateCount = dates.size();

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
	file.write((C8*)dates.data(), dates.size() * sizeof(Date));

	for (Size s = 0; s < shifts.size(); ++s) {
		file.write((C8*)shifts[s].data(), shifts[s].size() * sizeof(Shift));
	}

	return 0;
}

mstd::Status Database::version1(std::ifstream& file, mstd::U16 revision) {
	using namespace mstd;

	struct SubHeader {
		U32 studentCount;
		U32 stringChunkSize;
		U32 dateCount;
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

	dates.resize(subHeader.dateCount);
	shifts.resize(subHeader.dateCount);
	file.read((C8*)dates.data(), dates.size() * sizeof(Date));

	for (Size s = 0; s < shifts.size(); ++s) {
		shifts[s].resize(subHeader.studentCount);
		file.read((C8*)shifts[s].data(), shifts[s].size() * sizeof(Shift));
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
	source.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	while (!file.eof()) {
		C8 c;
		file.read(&c, 1);
		
		if (c != '\r') {
			source.push_back(c);
		}
	}

	if (source.back() != '\n') {
		source.push_back('\n');
	}

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

		END = 6,
	};

	enum {
		SOLVE,
		CAMP,
		OTHER,
	} eventType;

	U32 state = TIMESTAMP;

	C8* start = source.data() + dataStartOffset;
	C8* end;

	std::vector<std::string> names;
	names.resize(students.size());

	for (Size i = 0; i < students.size(); ++i) {
		names[i] = firstNames[i] + " " + lastNames[i];
		students[i].solveHours = 0.0f;
		students[i].campHours = 0.0f;
		students[i].otherHours = 0.0f;

		students[i].solveEvents = 0;
		students[i].camps = 0;
		students[i].other = 0;
	}

	Student* currentStudent = nullptr;
	for (Size i = dataStartOffset; i < source.size(); ++i) {
		if (source[i] == ',' || source[i] == '\n') {
			end = source.data() + i;

			std::string_view token = std::string_view(start + 1, end - 1);
			std::cout << token << std::endl;

			switch (state) {
			case NAME:
				std::cout << "^^^ THIS IS A NAME\n";
				currentStudent = nullptr;
				for (Size s = 0; s < names.size(); ++s) {
					if (token == names[s]) {
						currentStudent = students.data() + s;
						break;
					}
				}
				break;
			case ACTIVITY:
				if (!currentStudent) {
					break;
				}
				if (token == "Solve Event") {
					++currentStudent->solveEvents;
					eventType = SOLVE;
				} else if (token == "Camp") {
					++currentStudent->camps;
					eventType = CAMP;
				} else {
					++currentStudent->other;
					eventType = OTHER;
				}
				break;
			case HOURS:
				if (!currentStudent) {
					break;
				}
				if (eventType == SOLVE) {
					currentStudent->solveHours += std::stof(std::string(token));
				} else if (eventType == CAMP) {
					currentStudent->campHours += std::stof(std::string(token));
				} else {
					currentStudent->otherHours += std::stof(std::string(token));
				}
				break;
			default:
				break;
			}

			start = end + 1;
			state = (state + 1) % END;
		}
	}

	return 0;
}

mstd::Status Database::exportCSV(const std::string& filepath) {
	using namespace mstd;

	std::ofstream file(filepath, std::ios::trunc);
	if (!file.is_open()) {
		logError(filepath, "");
		return 1;
	}

	file << ",";
	for (auto& d : dates) {
		file << "\"" << U32(d.month) << "/" << U32(d.day) << "/" << U32(d.year) << " IN\",";
		file << "\"" << U32(d.month) << "/" << U32(d.day) << "/" << U32(d.year) << " OUT\",";
	}
	file << "\n";

	for (Size s = 0; s < students.size(); ++s) {
		file << firstNames[s] << " " << lastNames[s] << ",";

		for (Size d = 0; d < shifts.size(); ++d) {
			Shift& shift = shifts[d][s];
			if (shift.in.hour == 255) {
				file << "ABSENT,ABSENT,";
				continue;
			}

			file << U32(shift.in.hour) << ":" << U32(shift.in.minute) << ",";
			file << U32(shift.out.hour) << ":" << U32(shift.out.minute) << ",";
		}

		file << "\n";
	}

	return 0;
}

mstd::Status Database::importNames(const std::string& filepath) {
	using namespace mstd;

	std::ifstream file(filepath);
	if (!file.is_open()) {
		logError(filepath, "");
		return 1;
	}

	std::string source;
	file.seekg(0, std::ios::end);
	source.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	while (!file.eof()) {
		C8 c;
		file.read(&c, 1);
		
		if (c != '\r') {
			source.push_back(c);
		}
	}

	if (source.back() != '\n') {
		source.push_back('\n');
	}

	enum State : U32 {
		FIRST = 0,
		LAST = 1,

		END = 2
	};

	U32 state = FIRST;
	C8* start = source.data();
	C8* end;

	for (Size i = 0; i < source.size(); ++i) {
		if (source[i] == ',' || source[i] == '\n') {
			end = source.data() + i;

			std::string_view token = std::string_view(start, end);
			if (state == FIRST) {
				firstNames.push_back(std::string(token));
			} else {
				lastNames.push_back(std::string(token));
				students.push_back({});

				for (Size s = 0; s < shifts.size(); ++s) {
					shifts[s].push_back({0xFF, 0});
				}
			}

			start = end + 1;
			state = (state + 1) % END;
		}
	}

	return 0;
}

void Database::addDate() {
	using namespace mstd;

	Date today;

	std::time_t timestamp;
	std::time(&timestamp);

	const tm* local = std::localtime(&timestamp);

	today.year = local->tm_year + 1900;
	today.month = local->tm_mon + 1;
	today.day = local->tm_mday;

	if (dates.size()) {
		Date last = dates.back();
		if (std::memcmp(&last, &today, sizeof(Date)) == 0) {
			return;
		}
	}

	dates.push_back(today);
	shifts.push_back({});
	shifts.back().resize(students.size());
}


void Database::printShifts() const {
	using namespace mstd;

	for (Size i = 0; i < dates.size(); ++i) {
		std::cout << (U32)dates[i].year << "/" << (U32)dates[i].month << "/" << (U32)dates[i].day << " ";

		for (Size j = 0; j < students.size(); ++j) {
			Shift temp = shifts[i][j];
			std::cout
				<< (U32)temp.in.hour << ":" << (U32)temp.in.minute << " "
				<< (U32)temp.out.hour << ":" << (U32)temp.out.minute << ";";
		}

		std::cout << std::endl;
	}

}