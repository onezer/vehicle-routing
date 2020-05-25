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

std::string Genetic::WriteRoute()
{
	Gene gene = population.BestGene();
	const Graph *graph = population.GetGraph();
	
	std::string route = "";

	int store = graph->GetStore();

	std::vector<int> tmpRoute;

	for (char label : gene.groups) {
		int prev = store;

		for (int gNode : gene.order.at(label)) {
			tmpRoute = graph->GetRouteBetween(prev, gNode);

			for (int node : tmpRoute) {
				route += std::to_string(node);
				route += "-->";
			}

			prev = gNode;
		}

		tmpRoute = graph->GetRouteBetween(prev, store);

		for (int node : tmpRoute) {
			route += std::to_string(node);
			route += "-->";
		}
	}
	route += std::to_string(store);

	return route;
}

Gene Genetic::GetBestGene()
{
	return population.BestGene();
}
