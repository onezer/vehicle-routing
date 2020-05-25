#include "genetic.h"
#include "gene.hpp"

Genetic::Genetic(Parameters parameters, Graph* graph) : parameters{parameters}, _graph{graph}
{
}

void Genetic::InitializePopulation(int num)
{
	if (num == 0)
		num = parameters.max_population;
	population = Population(num, parameters, _graph);
}

float Genetic::Iterate()
{
	population.CreateEntities(parameters.new_entities);

	float cost = population.Iterate();

	return cost;
}

void Genetic::WriteRoute()
{
	Gene gene = population.BestGene();
}

Gene Genetic::GetBestGene()
{
	return population.BestGene();
}
