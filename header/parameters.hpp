#pragma once

class Parameters {
public:
	Parameters(int max_population, int new_entities, int die_num):
		max_population{ max_population },
		new_entities{ new_entities },
		die_num{die_num}
	{}

	int max_population; //maximum number of entities in a population
	int new_entities; //new random entities created in each generation
	int die_num; //least fit entities to be removed
};