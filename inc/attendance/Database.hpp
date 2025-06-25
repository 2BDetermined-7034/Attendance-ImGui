#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <mstd/misc>
#include <string>
#include <vector>
#include <variant>
#include <functional>

class Database {
public:
	~Database() {}

	mstd::Status read(const std::string& filepath);
	mstd::Status write(const std::string& filepath);

	struct Student {
	private:
		mstd::U32 firstName;
		mstd::U32 lastName;

		friend class Database;

	public:
		mstd::F32 labHours;
		mstd::F32 solveHours;
		mstd::F32 campHours;
		mstd::F32 otherHours;

		mstd::U32 solveEvents;
		mstd::U32 camps;
		mstd::U32 other;

		/*
		 * TODO: Add skill tree data here.
		 * I think it could also be a couple U8s if the trees within each sub-team don't
		 * intersect themselves.
		 * Or a bitmask.
		 *
		 * e.g.
		 *
		 * struct {
		 * U8 generalSkills;
		 * U8 controlTheory;
		 * U8 programming;
		 * } software;
		 *
		 * --or--
		 *
		 *  enum class softwareMask;
		 */
	};

	std::vector<Student> students;
	std::vector<std::string> firstNames, lastNames;

private:
	mstd::Status version1(std::ifstream& file, mstd::U16 revision);

	static constexpr std::array versions = {
		&Database::version1
	};
};

#endif
