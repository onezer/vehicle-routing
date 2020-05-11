#pragma once

#include <string>
#include "Graph.hpp"

class Entity {
	
	std::string gene;

	Graph* _graph;
public:

	Entity(const std::string & gene, Graph* graph);
	Entity(std::string && gene, Graph* graph);
	float Cost() const;

	std::string Crossover(const std::string & other) const;
	std::string Mutation() const;

	const std::string & GetGene() const;

};