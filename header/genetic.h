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
	Genetic(Parameters parameters, Graph* graph);

	void InitializePopulation(int num = 0);

	float Iterate(); //returns the lowest cost in current generation

	void WriteRoute();

	Gene GetBestGene();
};