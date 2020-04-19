#pragma once

#include"Node.hpp"


class Edge {
	int id;

	std::pair<Node, Node> nodes;

public:

	int GetId() const { return id; }
	std::pair<Node, Node> GetNodes() const { return nodes; }
};