#pragma once

#include <string>
#include "Graph.hpp"
#include "gene.hpp"

class Entity {
	
	Gene gene;

	Graph* _graph;
public:

	Entity(const Gene & gene, Graph* graph);
	Entity(Gene && gene, Graph* graph);
	float Cost() const;

	Gene Crossover(const Gene & other) const;
	Gene Mutation() const;

	const Gene & GetGene() const;

};