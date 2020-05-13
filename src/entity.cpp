#include "entity.h"
#include "Graph.hpp"
#include "genetic.h"
#include <math.h>
#include <stdlib.h>
#include <unordered_map>


Entity::Entity(const Gene & gene, Graph* graph) : gene{ gene }, _graph{graph}
{
}

Entity::Entity(Gene && gene, Graph* graph) : gene { gene }, _graph{ graph }
{
}

float Entity::Cost() const
{
	float cost = 0;

	return cost;
}

Gene Entity::Crossover(const Gene & other) const
{
	Gene newGene;


	return newGene;
}

Gene Entity::Mutation() const
{
	Gene newGene(gene);

	int mutation_num = rand() % 4 + 1;

	int label_num = gene.groups.size();

	for (int i = 0; i < label_num; ++i) {
		char label = gene.groups[i];

		std::vector<int> order = gene.order.at(label);
		int order_size = order.size();
		
		for (int j = 0; j < mutation_num; ++j) {
			int first = rand() % order_size;
			int second = rand() % order_size;

			while (first != second) {
				int first = rand() % order_size;
				int second = rand() % order_size;
			}

			std::swap(order[first], order[second]);
		}
	}

	return newGene;
}

const Gene & Entity::GetGene() const
{
	return gene;
}
