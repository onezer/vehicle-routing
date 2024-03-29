#pragma once

#include <iostream>
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

	int _vertex_num;

	float** _weight_matrix = nullptr;
	float** _population = nullptr;
	float** _distance = nullptr;


	int _storage;
	std::vector<int> _addresses;
	std::vector<int> _nodes_of_interest; //storage+addresses

	std::unordered_map<char, std::vector<int>> _nodes_by_group;

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
		int n = _vertex_num;
		float** cost;
		int *pred, *visited;
		float *distance, mindistance;

		cost = AllocateMatrix(_vertex_num);
		distance = new float[_vertex_num]();
		pred = new int[_vertex_num]();
		visited = new int[_vertex_num]();

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

		for(int i : _nodes_of_interest) {
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

		DeleteMatrix(cost, _vertex_num);
	}

public:

	typedef float(*optimizeCB)(float, float);

	void LabelNode(int node, char label) {
		if (_nodes_by_group.find(label) != _nodes_by_group.end()) {
			_nodes_by_group.at(label).push_back(node);
		}
		else {
			_nodes_by_group.insert({label, std::vector<int>()});
			_nodes_by_group.at(label).push_back(node);
		}

		_nodes_of_interest.push_back(node);
	}

	std::unordered_map<char, std::vector<int>> Get_nodes_by_group() {
		return _nodes_by_group;
	}

	Graph(const Graph& other) noexcept {
		*this = other;
	}

	Graph(Graph&& other) noexcept {
		*this = other;
	}

	Graph(int v_num , float** matrix) noexcept : _vertex_num(v_num) {
		OverwriteMatrix(matrix, _weight_matrix, _vertex_num, _vertex_num);
	}

	Graph& operator=(const Graph& other) {
		OverwriteMatrix(other._weight_matrix, _weight_matrix, other._vertex_num, _vertex_num);
		OverwriteMatrix(other._population, _population, other._vertex_num, _vertex_num);
		OverwriteMatrix(other._distance, _distance, other._vertex_num, _vertex_num);

		_route = other._route;
		_weight_distance = other._weight_distance;
		_nodes_of_interest = other._nodes_of_interest;
		_storage = other._storage;
		_addresses = other._addresses;
		_vertex_num = other._vertex_num;
	}

	Graph& operator=(Graph&& other) {
		_weight_matrix = other._weight_matrix;
		other._weight_matrix = nullptr;

		_route = std::move(other._route);
		_weight_distance = std::move(other._weight_distance);
		_nodes_of_interest = std::move(other._nodes_of_interest);
		_storage = std::move(other._storage);
		_addresses = std::move(other._addresses);
	}

	Graph(std::string filename) {
		ParseJSON(filename);
	}

	//for testing purposes
	/*Graph(int matrix[5][5]): _vertex_num(5) {
		_weight_matrix = AllocateMatrix(_vertex_num);
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				_weight_matrix[i][j] = (float)matrix[i][j];
			}
		}
	}*/

	int GetStore() const {
		return _storage;
	}

	void SetStore(int st) {
		_storage = st;
		_nodes_of_interest.push_back(st);
	}

	void CalculateDistances() {
		if (_weight_matrix == nullptr) return;

		//TODO:support multithreading
		for (int node : _nodes_of_interest) {
			dijkstra(node);
		}
	}

	float GetDistanceBetween(int x, int y) const {
		//TODO: check for validity of input
		return _weight_distance.at({x, y});
	}

	std::vector<int> GetRouteBetween(int x, int y) const {
		//TODO: check for validity of input
		return _route.at({x, y});
	}

	void ParseJSON(std::string filename) {
		json j;

		std::ifstream ifs(filename, std::ifstream::in);
		if (ifs.good() == false) return;
		j << ifs;
		ifs.close();

		if (_distance != nullptr) {
			DeleteMatrix(_distance, _vertex_num);
		}

		if (_population != nullptr) {
			DeleteMatrix(_population, _vertex_num);
		}

		if (_weight_matrix != nullptr) {
			DeleteMatrix(_weight_matrix, _vertex_num);
		}

		_vertex_num = j.at("intersections").size();
		
		_distance = AllocateMatrix(_vertex_num);
		_population = AllocateMatrix(_vertex_num);
		_weight_matrix = AllocateMatrix(_vertex_num);

		for (auto e : j.at("roadSegments")) {
			int x = e.at("NextIntersection").at("Id");
			int y = e.at("PrevIntersection").at("Id");

			_distance[x][y] = e.at("Length");
			_distance[y][x] = e.at("Length");		//every road is two way
			_population[x][y] = e.at("Population");
			_population[y][x] = e.at("Population");
		}
	}

	void ParseGroups(std::string filename) {
		json j;

		std::ifstream ifs(filename, std::ifstream::in);
		if (ifs.good() == false) return;
		j << ifs;
		ifs.close();

		for (auto group : j.at("groups")) {
			int id = group.at("id");
			int g = group.at("group");
			if ((char)g == 's') {
				SetStore(id)
			}
			else {
				LabelNode(id, (char)g);
			}
			
		}
	}

	//for testing purposes
	void Writeoutmatrices() const {
		std::cout << "Population: \n";
		for (int i = 0; i < _vertex_num; ++i) {
			for (int j = 0; j < _vertex_num; ++j) {
				std::cout << i << "," << j << ": " << _population[i][j] << "\n";
			}
		}

		std::cout << "\n Distance: \n";
		for (int i = 0; i < _vertex_num; ++i) {
			for (int j = 0; j < _vertex_num; ++j) {
				std::cout << i << "," << j << ": " << _distance[i][j] << "\n";
			}
		}

		std::cout << "\n Weight: \n";
		for (int i = 0; i < _vertex_num; ++i) {
			for (int j = 0; j < _vertex_num; ++j) {
				std::cout << i << "," << j << ": " << _weight_matrix[i][j] << "\n";
			}
		}
	}

	void OptimizeForPopulation() {
		OverwriteMatrix(_population, _weight_matrix, _vertex_num);
	}

	void OptimizeForDistance() {
		OverwriteMatrix(_distance, _weight_matrix, _vertex_num);
	}

	void OptimizeForCustom(optimizeCB CB) {
		for (int i = 0; i < _vertex_num; ++i) {
			for (int j = 0; j < _vertex_num; ++j) {
				_weight_matrix[i][j] = CB(_population[i][j], _distance[i][j]);
			}
		}
	}

	~Graph() {
		DeleteMatrix(_weight_matrix, _vertex_num);
	}

	constexpr unsigned int GetVertexNum() const { return _vertex_num; }
};