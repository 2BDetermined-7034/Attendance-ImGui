#ifndef NODELINK_HPP
#define NODELINK_HPP

#include <attendance/NodeBubble.hpp>
#include <SFML/Graphics.hpp>

class NodeLink {
public:
	static constexpr mstd::F32 LINK_ACCEL = 100.0f;

	NodeLink() {}

	NodeLink(NodeBubble* a, NodeBubble* b);

	void update();

	constexpr operator sf::RectangleShape&() {
		return shape;
	}

private:
	sf::RectangleShape shape;
	NodeBubble* a;
	NodeBubble* b;
	mstd::F32 targetLength;
};

#endif
