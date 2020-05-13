#include "genetic.h"

Genetic::Genetic(Parameters parameters, Graph* graph) : parameters{parameters}, _graph{graph}
{
}

void Genetic::InitializePopulation(int num)
{
	population = Population(num, parameters, _graph);
}

int Genetic::Iterate()
{
	population.CreateEntities(parameters.new_entities);

	int cost = population.Iterate();

	return cost;
}

Gene Genetic::GetBestGene()
{
	return population.BestGene();
}
