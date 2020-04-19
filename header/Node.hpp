#pragma once

#include "Edge.hpp"
#include<vector>


class Node {
	int id;

	std::vector<Edge> edges;

public:

	Node(int id): id(id) {}
	Node(int id, Edge e) : id(id) {
		edges.push_back(e);
	}

	int GetId() const { return id; }
	std::vector<Edge> GetEdges() const { return edges; }
	void AddEdge(Edge e) {
	}
};