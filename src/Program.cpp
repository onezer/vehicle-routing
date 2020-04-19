#include <iostream>

#include"Graph.hpp"

int main() {

	int G[5][5] = { {0,1,0,3,10},
					{1,0,5,0,0},
					{0,5,0,2,1},
					{3,0,2,0,6},
					{10,0,1,6,0} };

	Graph test(G);

	test.dijkstra(0);

	return 0;
}