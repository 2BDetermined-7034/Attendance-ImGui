#include <attendance/timer.hpp>

#include <chrono>
#include <thread>
#include <mstd/misc>

void timer(std::function<void()> run) {
	using namespace mstd;

	std::thread t([run]() {
		std::time_t timestamp;
		std::time(&timestamp);

		const tm* local = std::localtime(&timestamp);

		if (local->tm_hour == 0) {
			run();
			return;
		}

		while (1) {
			local = std::localtime(&timestamp);

			constexpr auto midnight = std::chrono::time_point<std::chrono::system_clock>();
	

			//std::this_thread::sleep_until(midnight);
		}

		run();

		return;
	});
	t.detach();
}