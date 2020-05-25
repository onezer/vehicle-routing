#pragma once

#include <string>
#include <memory>
#include <vector>

#include "parameters.hpp"
#include "entity.h"
#include "gene.hpp"

class Population {

	std::vector<std::pair<float, Entity>> genePool;

	Graph* _graph;
	
	Parameters parameters;

public:
	Population();
	Population(int num, Parameters parameters, Graph* graph);

	void CreateEntities(int num);
	
	float Iterate(); //returns lowest cost
	Gene BestGene();

private:
	void AddEntity(const Gene &);
	void AddEntity(Gene &&);
	void SortGenePool();
};