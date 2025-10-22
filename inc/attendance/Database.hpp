#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <mstd/misc>
#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <chrono>

class Database {
public:
	~Database() {}

	mstd::Status read(const std::string& filepath);
	mstd::Status write(const std::string& filepath);

	mstd::Status import(const std::string& filepath);

	struct Student {
	private:
		mstd::U32 firstName;
		mstd::U32 lastName;

		friend class Database;

	public:
		mstd::F32 labHours = 0.0f;
		mstd::F32 solveHours = 0.0f;
		mstd::F32 campHours = 0.0f;
		mstd::F32 otherHours = 0.0f;

		mstd::U32 solveEvents = 0.0f;
		mstd::U32 camps = 0.0f;
		mstd::U32 other = 0.0f;

		mstd::U8 design[6] = {0};
		mstd::U8 manufacturing[16] = {0};
		mstd::U8 mechanical[4] = {0};
		mstd::U8 electrical[5] = {0};
		mstd::U8 software[4] = {0};
		mstd::U8 marketing[8] = {0};
		mstd::U8 outreach[5] = {0};
		mstd::U8 business[4] = {0};
		mstd::U8 safety[4] = {0};
		mstd::U8 strategy[2] = {0};
	};

	std::vector<Student> students;
	std::vector<std::string> firstNames, lastNames;

private:
	mstd::Status version1(std::ifstream& file, mstd::U16 revision);

	using V = std::function<mstd::Status(Database*, std::ifstream&, mstd::U16)>;

	static std::vector<V> versions;
};

#endif