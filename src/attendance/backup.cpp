#include <attendance/backup.hpp>
#include <iostream>

void backupCreate(Database& db) {
	using namespace mstd;

	Size i = 0;

	std::string filepath;
	filepath.reserve(sizeof("backup1234.db2"));

	std::fstream backup;
	do {
		filepath = "backup" + std::to_string(i++) + ".db2";
		backup = std::fstream(filepath, std::ios::in | std::ios::out);
	} while(backup.good());

	db.write(filepath);
}

void backupRestore(Database& db) {
	using namespace mstd;

	Size i = 0;

	std::string filepath;
	filepath.reserve(sizeof("backup1234.db2"));

	std::fstream backup;
	do {
		filepath = "backup" + std::to_string(i++) + ".db2";
		backup = std::fstream(filepath, std::ios::in | std::ios::out);
	} while(backup.good());

	filepath = "backup" + std::to_string(i - 2) + ".db2";

	std::cout << "restored " << filepath << std::endl;

	db.read(filepath);
}
