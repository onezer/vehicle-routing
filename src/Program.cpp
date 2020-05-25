#include <iostream>
#include <fstream>

#include "Graph.hpp"
#include "gene.hpp"
#include "genetic.h"
#include "parameters.hpp"

int main() {

	srand(6546);

	int G[5][5] = { {0,1,0,3,10},
					{1,0,5,0,0},
					{0,5,0,2,1},
					{3,0,2,0,6},
					{10,0,1,6,0} };
	
	std::cout << "Loading the file...";
	Graph g("D:/cities/roadNetwork_big_600.json");
	std::cout << " done\n";

	//Graph g(G);

	g.OptimizeForCustom([](float p, float d) {return p + d; });

	//g.Writeoutmatrices();

	g.LabelNode(3, 'a');
	g.LabelNode(4, 'a');
	g.LabelNode(0, 'a');
	g.LabelNode(112, 'a');
	g.LabelNode(255, 'a');
	g.LabelNode(458, 'a');
	g.LabelNode(26, 'a');
	g.LabelNode(287, 'a');
	g.LabelNode(302, 'a');
	g.LabelNode(445, 'a');
	g.LabelNode(89, 'a');
	g.LabelNode(65, 'a');
	g.LabelNode(14, 'a');
	g.LabelNode(325, 'a');
	g.LabelNode(514, 'a');
	g.LabelNode(87, 'a');
	g.LabelNode(296, 'a');
	g.LabelNode(311, 'a');
	g.LabelNode(357, 'a');
	g.LabelNode(200, 'a');

	g.SetStore(300);


	std::cout << "Calculating best routes...";
	g.CalculateDistances();
	std::cout << " done\n";
	
	Parameters params(3000, 300, 1500);

	Genetic genetic(params, &g);

	std::cout << "Initializing population...";
	genetic.InitializePopulation();
	std::cout << " done\n";
	
	for (int i = 0; i < 200; ++i) {
		/*if (i > 0) {
			std::cout << "\n";
		}*/

		float cost = genetic.Iterate();

		if (i % 10 == 0) {
			std::cout << i << ". gen:  " << cost << std::endl;
		}

		
	}

	return 0;
}