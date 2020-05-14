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
	newGene.groups = gene.groups;
	if (other.groups.size() != gene.groups.size()) throw std::exception("Illegal crossover!");

	for (int i = 0; i < gene.groups.size(); ++i) {
		char label = gene.groups[i];

		int order_size = gene.order.at(label).size();

		bool* choose = new bool[order_size];

		//inicialize choose bool array with random values
		for (int j = 0; j < order_size; ++j) {
			choose[j] = rand() % 100 < 50;
		}

		std::vector<int> temp;
		std::vector<int> order;

		//mixing the two order vectors
		for (int j = 0; j < order_size; ++j) {
			temp.push_back(gene.order.at(label)[j]);
			temp.push_back(other.order.at(label)[j]);
		}

		int k = 0;
		std::unordered_map<int, bool> visited;

		//removing duplicates
		for(int j = 0; j < temp.size(); ++j){
			if (visited.find(temp[j]) == visited.end()) {
				if (choose[k]) {
					order.push_back(temp[j]);
					++k;
				}

				visited.insert({temp[j], true});
			}
			else {
				if (choose[k] == false) {
					order.push_back(temp[j]);
					++k;
				}
			}

		}

		newGene.order.insert({ label, order });
	}

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
