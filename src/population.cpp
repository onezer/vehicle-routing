#include "population.h"

#include <algorithm>

Population::Population() : parameters{ Parameters(0, 0, 0) }
{
}

Population::Population(int num, Parameters parameters, Graph* graph) : parameters{parameters}, _graph{graph}
{
	CreateEntities(num);
	SortGenePool();
}

void Population::CreateEntities(int num)
{
	for (int i = 0; i < num; ++i) {
		std::string currentGene;
		/*for (int j = 0; j < goal.size(); ++j) {
			int pos = rand() % tempGoal.size();

			currentGene.push_back(tempGoal[pos]);
			tempGoal.erase(pos, 1);
		}*/
		AddEntity(currentGene);
	}
}

int Population::Iterate()
{
	while (genePool.size() < parameters.max_population) {
		if (rand() % 2 == 1 && genePool.size() > 1) {
			int first = rand() % genePool.size();
			int second = rand() % genePool.size();

			while (first == second) {
				second = rand() % genePool.size();
			}

			AddEntity(genePool[first].second.Crossover(genePool[second].second.GetGene()));
		}
		else {
			AddEntity(genePool[rand() % genePool.size()].second.Mutation());
		}
		
	}

	SortGenePool();

	for (int i = 0; i < parameters.die_num; ++i) {
		if (genePool.size() == 1) {
			throw std::exception("Population is dead!");
		}
		genePool.pop_back();
	}

	return genePool[0].first;
}

std::string Population::BestGene()
{
	SortGenePool();
	return genePool[0].second.GetGene();
}

bool Compare(const std::pair<int, Entity> & first, const std::pair<int, Entity> & second)
{
	return (first.first < second.first);
}


void Population::AddEntity(const std::string & gene)
{
	if (genePool.size() == parameters.max_population) {
		return;
	}

	Entity currentEntity(gene, _graph);

	genePool.push_back(std::pair<int, Entity>(currentEntity.Cost(), currentEntity));
}

void Population::AddEntity(std::string && gene)
{
	if (genePool.size() == parameters.max_population) {
		return;
	}

	Entity currentEntity(gene, _graph);

	genePool.push_back(std::pair<int, Entity>(currentEntity.Cost(), currentEntity));
}

void Population::SortGenePool()
{
	std::sort(genePool.begin(), genePool.end(), Compare);
}
