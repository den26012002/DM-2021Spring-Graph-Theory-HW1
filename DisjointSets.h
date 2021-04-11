#pragma once
#include<vector>

class DisjointSets {
public:
	explicit DisjointSets(int _size = 0);

	void makeSet(int x);

	int findSet(int x);

	void unionSets(int x, int y);

private:
	std::vector<int> parents;
	std::vector<int> ranks;
	int size;
};

