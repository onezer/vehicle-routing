#pragma once

#include "parameters.hpp"
#include "Graph.hpp"
#include "population.h"

#include <string>
class Genetic {
	Population population;
	Parameters parameters;
	Graph* _graph;

public:
	Genetic(std::string goal, Parameters parameters, Graph* graph);

	void InitializePopulation(int num);

	int Iterate(); //returns the lowest cost in current generation

	std::string GetBestGene();
};