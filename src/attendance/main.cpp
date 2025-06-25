#include <SFML/Graphics.hpp>
#include <attendance/Database.hpp>
#include <iostream>

int main() {
	using namespace mstd;

	sf::RenderWindow window(sf::VideoMode({800, 600}), "Attendance", sf::Style::Default);
	window.setVerticalSyncEnabled(true);

	sf::Clock clock;
	F32 previousTime = 0.0f;
	while (window.isOpen()) {
		F32 time = clock.getElapsedTime().asSeconds();
		F32 deltaTime = time - previousTime;
		previousTime = time;
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.display();
	}

	return 0;
}
