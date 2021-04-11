#include "DisjointSets.h"

DisjointSets::DisjointSets(int _size) :
	size(_size),
	parents(_size),
	ranks(_size)
{
	for (int i(0); i < _size; ++i) {
		parents[i] = i;
		ranks[i] = 1;
	}
}

void DisjointSets::makeSet(int x) {
	if (size < x) {
		size = x;
		parents.resize(x);
		ranks.resize(x);
	}
	parents[x] = x;
	ranks[x] = 1;
}

int DisjointSets::findSet(int x) {
	if (parents[x] == x) {
		return x;
	} else {
		return parents[x] = findSet(parents[x]);
	}
}

void DisjointSets::unionSets(int x, int y) {
	x = findSet(x);
	y = findSet(y);
	if (ranks[x] < ranks[y]) {
		parents[x] = y;
		ranks[y] += ranks[x];
	} else {
		parents[y] = x;
		ranks[x] += ranks[y];
	}
}