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

	for (int i = 0; i < gene.groups.size(); ++i) {
		char label = gene.groups[i];

		cost += _graph->GetDistanceBetween(_graph->GetStore(), gene.order.at(label)[0]);

		for (int j = 0; j < gene.order.at(label).size(); ++j) {
			if (j == gene.order.at(label).size() - 1) {
				cost += _graph->GetDistanceBetween(_graph->GetStore(), gene.order.at(label)[j]);
			}
			else {
				cost += _graph->GetDistanceBetween(gene.order.at(label)[j], gene.order.at(label)[j+1]);
			}
		}
	}

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

		std::vector<int> temp;
		std::vector<int> order;

		//mixing the two order vectors
		for (int j = 0; j < order_size; ++j) {
			if (rand() % 100 < 50) {
				temp.push_back(gene.order.at(label)[j]);
				temp.push_back(other.order.at(label)[j]);
			}
			else {
				temp.push_back(other.order.at(label)[j]);
				temp.push_back(gene.order.at(label)[j]);
			}
			
		}

		std::unordered_map<int, char> choose;

		//inicialize choose random values
		//a = first pick, unvisited
		//b = second pick, unvisited
		//c = first pick, visited
		//d = second pick, visited
		for (int j = 0; j < order_size; ++j) {
			char decision = rand() % 100 < 50 ? 'a' : 'b';

			choose.insert({gene.order.at(label)[j], decision});
		}

		//removing duplicates
		for(int j = 0; j < temp.size(); ++j){
			if (choose.at(temp[j]) == 'a') {
				order.push_back(temp[j]);
				choose.insert_or_assign(temp[j], 'c');
			}
			else if(choose.at(temp[j]) == 'b'){
				choose.insert_or_assign(temp[j], 'd');
			}
			else if (choose.at(temp[j]) == 'd') {
				order.push_back(temp[j]);
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
