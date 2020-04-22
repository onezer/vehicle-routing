#pragma once

#include <vector>
#include <unordered_map>
#include <fstream>
#include"json.hpp"

using json = nlohmann::json;

template <>
struct std::tr1::hash<std::pair<int, int> > {
public:
	size_t operator()(std::pair<int, int> x) const throw() {
		size_t h = (x.first ^ x.second) * x.first + x.second;
		return h;
	}
};

class Graph {

	int vertex_num;

	float** _weight_matrix = nullptr;
	float** _population = nullptr;
	float** _distance = nullptr;


	std::vector<int> storages;
	std::vector<int> addresses;
	std::vector<int> nodes_of_interest; //storages+addresses

	//distances and routes variables only contain the distances and routes for the nodes of interest
	std::unordered_map<std::pair<int, int>, float> _weight_distance; //_distance[{2,5}] gives the minimum distance between the node 2 and 5
	std::unordered_map<std::pair<int, int>, std::vector<int>> _route; //route[{3,4}] gives a vector that contains the route nodes from node 3 to node 4 in order


	float** AllocateMatrix(int vertex_num) const {
		float** matrix = new float*[vertex_num];
		for (int i = 0; i < vertex_num; ++i) {
			matrix[i] = new float[vertex_num];
		}

		for (int i = 0; i < vertex_num; ++i) {
			for (int j = 0; j < vertex_num; ++j) {
				matrix[i][j] = 0;
			}
		}

		return matrix;
	}

	void DeleteMatrix(float**& matrix, unsigned int size) const {
		for (unsigned int i = 0; i < size; ++i) {
			delete[] matrix[i];
		}
		delete[] matrix;
	}

	void OverwriteMatrix(float** from, float**& to, int sizeFrom, int sizeTo = 0) const {
		if (sizeTo == 0) sizeTo = sizeFrom;

		if (to != nullptr) DeleteMatrix(to,sizeTo);

		to = AllocateMatrix(sizeFrom);

		for (int i = 0; i < sizeFrom; ++i) {
			for (int j = 0; j < sizeFrom; ++j) {
				to[i][j] = from[i][j];
			}
		}
	}

	void dijkstra(int startnode) {
		int n = vertex_num;
		float** cost;
		int *pred, *visited;
		float *distance, mindistance;

		cost = AllocateMatrix(vertex_num);
		distance = new float[vertex_num]();
		pred = new int[vertex_num]();
		visited = new int[vertex_num]();

		int count, nextnode, i, j;

		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				if (_weight_matrix[i][j] == 0)
					cost[i][j] = INFINITY;
				else
					cost[i][j] = _weight_matrix[i][j];

		for (i = 0; i < n; i++) {
			distance[i] = cost[startnode][i];
			pred[i] = startnode;
			visited[i] = 0;
		}

		distance[startnode] = 0;
		visited[startnode] = 1;
		count = 1;

		while (count < n - 1) {
			mindistance = INFINITY;
			for (i = 0; i < n; i++)
				if (distance[i] < mindistance && !visited[i]) {
					mindistance = distance[i];
					nextnode = i;
				}
			visited[nextnode] = 1;
			for (i = 0; i < n; i++)
				if (!visited[i])
					if (mindistance + cost[nextnode][i] < distance[i]) {
						distance[i] = mindistance + cost[nextnode][i];
						pred[i] = nextnode;
					}
			count++;
		}

		for(int i : nodes_of_interest) {
			if (i != startnode) {
				_weight_distance.insert_or_assign({ startnode, i }, distance[i]);

				std::vector<int> route;

				j = i;
				do {
					j = pred[j];
					route.push_back(j);
				} while (j != startnode);

				std::reverse(route.begin(), route.end());
				_route.insert_or_assign({ startnode, i }, route);
			}
		}

		delete[] distance, pred, visited;

		DeleteMatrix(cost, vertex_num);
	}

public:

	typedef float(*optimizeCB)(float, float);

	Graph(const Graph& other) noexcept {
		*this = other;
	}

	Graph(Graph&& other) noexcept {
		*this = other;
	}

	Graph(int v_num , float** matrix) noexcept : vertex_num(v_num) {
		OverwriteMatrix(matrix, _weight_matrix, vertex_num, vertex_num);
	}

	Graph& operator=(const Graph& other) {
		OverwriteMatrix(other._weight_matrix, _weight_matrix, other.vertex_num, vertex_num);
		OverwriteMatrix(other._population, _population, other.vertex_num, vertex_num);
		OverwriteMatrix(other._distance, _distance, other.vertex_num, vertex_num);

		_route = other._route;
		_weight_distance = other._weight_distance;
		nodes_of_interest = other.nodes_of_interest;
		storages = other.storages;
		addresses = other.addresses;
		vertex_num = other.vertex_num;
	}

	Graph& operator=(Graph&& other) {
		_weight_matrix = other._weight_matrix;
		other._weight_matrix = nullptr;

		_route = std::move(other._route);
		_weight_distance = std::move(other._weight_distance);
		nodes_of_interest = std::move(other.nodes_of_interest);
		storages = std::move(other.storages);
		addresses = std::move(other.addresses);
	}

	Graph(std::string filename) {
		ParseJSON(filename);
	}

	//for testing purposes
	Graph(int matrix[5][5]): vertex_num(5) {
		_weight_matrix = AllocateMatrix(vertex_num);
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				_weight_matrix[i][j] = (float)matrix[i][j];
			}
		}
	}



	void CalculateDistances() {
		if (_weight_matrix == nullptr) return;

		//TODO:support multithreading
		for (int node : nodes_of_interest) {
			dijkstra(node);
		}
	}

	float GetDistanceBetween(int x, int y) {
		//TODO: check for validity of input
		return _weight_distance[{x, y}];
	}

	std::vector<int> GetRouteBetween(int x, int y) {
		//TODO: check for validity of input
		return _route[{x, y}];
	}

	void ParseJSON(std::string filename) {
		json j;

		std::ifstream ifs(filename, std::ifstream::in);
		if (ifs.good() == false) return;
		j << ifs;
		ifs.close();

		if (_distance != nullptr) {
			DeleteMatrix(_distance, vertex_num);
		}

		if (_population != nullptr) {
			DeleteMatrix(_population, vertex_num);
		}

		if (_weight_matrix != nullptr) {
			DeleteMatrix(_weight_matrix, vertex_num);
		}

		vertex_num = j.at("intersections").size();
		
		_distance = AllocateMatrix(vertex_num);
		_population = AllocateMatrix(vertex_num);
		_weight_matrix = AllocateMatrix(vertex_num);

		for (auto e : j.at("roadSegments")) {
			int x = e.at("NextIntersection").at("Id");
			int y = e.at("PrevIntersection").at("Id");

			_distance[x][y] = e.at("Length");
			_distance[y][x] = e.at("Length");		//every road is two way
			_population[x][y] = e.at("Population");
			_population[y][x] = e.at("Population");
		}
	}

	//for testing purposes
	void Writeoutmatrices() {
		std::cout << "Population: \n";
		for (int i = 0; i < vertex_num; ++i) {
			for (int j = 0; j < vertex_num; ++j) {
				std::cout << i << "," << j << ": " << _population[i][j] << "\n";
			}
		}

		std::cout << "\n Distance: \n";
		for (int i = 0; i < vertex_num; ++i) {
			for (int j = 0; j < vertex_num; ++j) {
				std::cout << i << "," << j << ": " << _distance[i][j] << "\n";
			}
		}

		std::cout << "\n Weight: \n";
		for (int i = 0; i < vertex_num; ++i) {
			for (int j = 0; j < vertex_num; ++j) {
				std::cout << i << "," << j << ": " << _weight_matrix[i][j] << "\n";
			}
		}
	}

	void OptimizeForPopulation() {
		OverwriteMatrix(_population, _weight_matrix, vertex_num);
	}

	void OptimizeForDistance() {
		OverwriteMatrix(_distance, _weight_matrix, vertex_num);
	}

	void OptimizeForCustom(optimizeCB CB) {
		for (int i = 0; i < vertex_num; ++i) {
			for (int j = 0; j < vertex_num; ++j) {
				_weight_matrix[i][j] = CB(_population[i][j], _distance[i][j]);
			}
		}
	}

	~Graph() {
		DeleteMatrix(_weight_matrix, vertex_num);
	}

	constexpr unsigned int GetVertexNum() const { return vertex_num; }
};