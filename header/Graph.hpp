#pragma once

#include<vector>
#include<unordered_map>
#define INFINITY 99999999

template <>
struct std::tr1::hash<std::pair<int, int> > {
public:
	size_t operator()(std::pair<int, int> x) const throw() {
		size_t h = (x.first ^ x.second) * x.first + x.second;
		return h;
	}
};

class Graph {

	const int vertex_num;

	int** weight_matrix;

	std::vector<int> storages;
	std::vector<int> addresses;
	std::vector<int> nodes_of_interest; //storages+addresses

	//distances and routes variables only contain the distances and routes for the nodes of interest
	std::unordered_map<std::pair<int, int>, int> distances; //distances[{2,5}] gives the minimum distance between the node 2 and 5
	std::unordered_map<std::pair<int, int>, std::vector<int>> routes; //routes[3][4] gives a vector that contains the route nodes from node 3 to node 4 in order


	int** AllocateMatrix() {
		int **matrix = new int*[vertex_num];
		for (int i = 0; i < vertex_num; ++i) {
			matrix[i] = new int[vertex_num];
		}

		return matrix;
	}

	void DeleteMatrix(int** matrix, unsigned int size) {
		for (int i = 0; i < size; ++i) {
			delete[] matrix[i];
		}
		delete[] matrix;
	}

	void dijkstra(int startnode) {
		int n = vertex_num;
		int** cost;
		int *distance, *pred, *visited;

		cost = AllocateMatrix();
		distance = new int[vertex_num]();
		pred = new int[vertex_num]();
		visited = new int[vertex_num]();

		int count, mindistance, nextnode, i, j;

		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				if (weight_matrix[i][j] == 0)
					cost[i][j] = INFINITY;
				else
					cost[i][j] = weight_matrix[i][j];

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
				//std::cout << "\nDistance of node" << i << "=" << distance[i];

				distances.insert_or_assign({ startnode, i }, distance[i]);

				std::vector<int> route;
				//std::cout << "\nPath=" << i;

				j = i;
				do {
					j = pred[j];
					//std::cout << "<-" << j;
					route.push_back(j);
				} while (j != startnode);

				std::reverse(route.begin(), route.end());
				routes.insert_or_assign({ startnode, i }, route);
			}
		}

		delete[] distance, pred, visited;

		DeleteMatrix(cost, vertex_num);
	}

public:

	Graph(const Graph& other) noexcept : vertex_num(other.vertex_num){
		weight_matrix = AllocateMatrix();

		int** matrix = other.weight_matrix;
		for (int i = 0; i < vertex_num; ++i) {
			for (int j = 0; j < vertex_num; ++j) {
				weight_matrix[i][j] = matrix[i][j];
			}
		}

		routes = other.routes;
		distances = other.distances;
		nodes_of_interest = other.nodes_of_interest;
		storages = other.storages;
		addresses = other.addresses;
	}

	Graph(Graph&& other) noexcept : vertex_num(other.vertex_num) {
		weight_matrix = other.weight_matrix;
		other.weight_matrix = nullptr;
		
		routes = std::move(other.routes);
		distances = std::move(other.distances);
		nodes_of_interest = std::move(other.nodes_of_interest);
		storages = std::move(other.storages);
		addresses = std::move(other.addresses);
	}

	Graph(int v_num , int** matrix) noexcept : vertex_num(v_num) {
		weight_matrix = AllocateMatrix();
		for (int i = 0; i < vertex_num; ++i) {
			for (int j = 0; j < vertex_num; ++j) {
				weight_matrix[i][j] = matrix[i][j];
			}
		}
	}

	//for testing purposes
	Graph(int matrix[5][5]): vertex_num(5) {
		weight_matrix = AllocateMatrix();
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				weight_matrix[i][j] = matrix[i][j];
			}
		}
	}

	void CalculateDistances() {
		for (int node : nodes_of_interest) {
			dijkstra(node);
		}
	}

	int GetDistanceBetween(int x, int y) {
		//TODO: check for validity of input
		return distances[{x, y}];
	}

	std::vector<int> GetRouteBetween(int x, int y) {
		//TODO: check for validity of input
		return routes[{x, y}];
	}


	~Graph() {
		DeleteMatrix(weight_matrix, vertex_num);
	}

	constexpr unsigned int GetVertexNum() const { return vertex_num; }
};