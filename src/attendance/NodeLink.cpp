#include <attendance/NodeLink.hpp>

NodeLink::NodeLink(NodeBubble* a, NodeBubble* b) {
	targetLength = mstd::magnitude(b->targetPosition - a->targetPosition);

	this->a = a;
	this->b = b;
}

void NodeLink::update() {
	using namespace mstd;

	Vector2f direction = b->position - a->position;
	shape.setPosition({direction.x / 2 + a->position.x, direction.y / 2 + a->position.y});
	shape.setSize({magnitude(direction), 40.0f});
	shape.setRotation(sf::radians(std::atan(direction.y / direction.x)));

	F32 displacement = targetLength - magnitude(direction);
	direction = normalize(direction);

	a->acceleration = -direction * displacement * LINK_ACCEL;
	b->acceleration = direction * displacement * LINK_ACCEL;
}
