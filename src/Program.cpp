#include <iostream>
#include <fstream>

#include "Graph.hpp"
#include "gene.hpp"

int main() {

	int G[5][5] = { {0,1,0,3,10},
					{1,0,5,0,0},
					{0,5,0,2,1},
					{3,0,2,0,6},
					{10,0,1,6,0} };

	/*Graph g("D:/roadnetwork.json");

	g.OptimizeForCustom([](float p, float d) {return p + d; });

	g.Writeoutmatrices();*/

	Gene gene;

	gene.order.insert_or_assign('a', std::vector<int>({ 1,2,3,4,5,6 }));
	gene.order.insert_or_assign('b', std::vector<int>({ 10,54,8,48,84,98 }));
	gene.groups.push_back('a');
	gene.groups.push_back('b');

	std::vector<int> order = gene.order.at('a');

	std::swap(order[0], order[1]);



	Gene gene2(gene);

	gene2.order.insert_or_assign('a', std::vector<int>({ 15,65,84 }));

	for (int i = 0; i < gene.order.at('a').size(); ++i) {
		std::cout << order[i] << ' ';
		std::cout << '\n';
	}

	for (int i = 0; i < gene2.order.at('b').size(); ++i) {
		std::cout << gene2.order.at('b')[i] << ' ';
		std::cout << '\n';
	}

	return 0;
}