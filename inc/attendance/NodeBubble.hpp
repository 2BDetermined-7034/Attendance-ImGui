#ifndef NODEBUBBLE_HPP
#define NODEBUBBLE_HPP

#include <mstd/geometry>
#include <SFML/Graphics.hpp>

class NodeBubble {
public:
	static constexpr mstd::F32 FRICTION_ACCEL = 500.0f;
	static constexpr mstd::F32 RESTORING_ACCEL = 100.0f;
	static constexpr mstd::F32 MAX_VELOCITY = 2000.0f;

	void update(mstd::F32 deltaTime);

	constexpr mstd::Bool atRest() {
		return velocity == mstd::Vector2f(0.0f);
	}

	constexpr operator sf::Shape&() {
		return shape;
	}

	mstd::Vector2f position, targetPosition, velocity, acceleration;
	sf::Shape& shape;
};

#endif
