#pragma once

#include <vector>
#include <unordered_map>

struct Gene {
	std::unordered_map<char, std::vector<int>> order;
	std::vector<char> groups;
};