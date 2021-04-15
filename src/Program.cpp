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

	/*g.LabelNode(3, 'a');
	g.LabelNode(357, 'a');
	g.LabelNode(200, 'a');
	g.LabelNode(65, 'b');
	g.LabelNode(14, 'b');
	g.LabelNode(325, 'b');*/

	//g.SetStore(300);


	std::cout << "Calculating best routes...";
	g.CalculateDistances();
	std::cout << " done\n";
	
	Parameters params(300, 30, 150);

	Genetic genetic(params, &g);
	genetic.name = "test";

	std::cout << "Initializing population...";
	genetic.InitializePopulation();
	std::cout << " done\n";
	
	for (int i = 0; i < 10; ++i) {

		float cost = genetic.Iterate();

		if (i % 10 == 0) {
			std::cout << i << ". gen:  " << cost << std::endl;
		}
		
	}

	//std::cout << genetic.WriteRoute();

	genetic.WriteRouteToFile("D:/cities/route.json");

	Graph g("D:/cities/roadNetwork_big_600.json");

	g.ParseGroups("D:/cities/groups.json");

	return 0;
}