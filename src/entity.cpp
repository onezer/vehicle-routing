#include "entity.h"
#include "Graph.hpp"
#include <math.h>
#include <stdlib.h>
#include <unordered_map>


Entity::Entity(const std::string & gene, Graph* graph) : gene{ gene }, _graph{graph}
{
}

Entity::Entity(std::string && gene, Graph* graph) : gene { gene }, _graph{ graph }
{
}

float Entity::Cost() const
{
	float cost = 0;

	return cost;
}

std::string Entity::Crossover(const std::string & other) const
{
	std::string newGene;

	for (int i = 0; i < (int)gene.size(); ++i) {

		char c = rand() % 2 == 1 ? gene[i] : other[i];

		newGene.push_back(c);
	}

	return newGene;
}

std::string Entity::Mutation() const
{
	std::string newGene(gene);

	int mutation_num = rand() % 4 + 1;

	for (int i = 0; i < mutation_num; ++i) {
		int first = rand() % (int)gene.size();
		int second = rand() % (int)gene.size();
		while (first == second) {
			second = rand() % (int)gene.size();
		}

		std::swap(newGene[first], newGene[second]);
	}

	return newGene;
}

const std::string & Entity::GetGene() const
{
	return gene;
}
