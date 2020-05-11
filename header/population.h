#pragma once

#include <string>
#include <memory>
#include <vector>

#include "parameters.hpp"
#include "entity.h"

class Population {

	std::vector<std::pair<float, Entity>> genePool;

	Graph* _graph;
	
	Parameters parameters;

public:
	Population();
	Population(int num, Parameters parameters, Graph* graph);

	void CreateEntities(int num);
	
	int Iterate(); //returns lowest cost
	std::string BestGene();

private:
	void AddEntity(const std::string &);
	void AddEntity(std::string &&);
	void SortGenePool();
};