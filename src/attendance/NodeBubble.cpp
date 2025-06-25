#include <attendance/NodeBubble.hpp>
#include <iostream>

void NodeBubble::update(mstd::F32 deltaTime) {
	using namespace mstd;

	Vector2f displacement = targetPosition - position;
	acceleration += displacement * RESTORING_ACCEL;
	velocity += acceleration * deltaTime;

	F32 friction = FRICTION_ACCEL * deltaTime;
	if (magnitude(velocity) > friction) {
		velocity -= normalize(velocity) * friction;
		if (magnitude(velocity) > MAX_VELOCITY) {
			velocity = normalize(velocity) * MAX_VELOCITY;
		}
		position += velocity * deltaTime;
	} else {
		velocity = Vector2f(0.0f);
		if (magnitude(displacement) < FRICTION_ACCEL * deltaTime) {
			position = targetPosition;
		}
	}

	shape.setPosition(*(sf::Vector2f*)&position);
}
