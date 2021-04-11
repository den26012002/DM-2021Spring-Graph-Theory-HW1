#pragma once
#include<vector>
#include<set>
#include<unordered_set>
#include<map>
#include<unordered_map>

const int INF = 1e9;

class Graph {
public:
	struct Edge {
		int from, to;
		long double weight;
		bool operator<(const Edge& other) {
			return weight < other.weight;
		}
	};

	struct Vertex {
		int eccentrisity;
		int connectedComponentSize;
		bool isInBiggestConnectedComponent;
	};

public:
	Graph();

	void addVertex(int vertexNumber);

	void addEdge(int from, int to, long double weight = 1);

	void addEdge(Edge edge);

	size_t getdelta();

	size_t getDelta();

	int getRadius();

	int getDiameter();

	std::vector<int> getCenter();

	std::unordered_set<int>& getArticulationPoints();

	std::vector<Edge> getBridges();

	Graph getMST();

	//for trees
	std::vector<int> getCentroid();

	//for trees
	std::vector<int> getPruferCode();

	void printAdjancencyListForPython(std::ostream& cout);

private:
	std::vector<Edge> edges;
	std::map<int, std::vector<Edge>> vertexesEdges;
	std::map<int, std::map<int, long double>> vertexesDists;
	std::map<int, Vertex> vertexes;
	bool isCorrectVertexStatus;
	std::unordered_set<int> notInBiggestConnectedComponent;

	std::map<int, bool> visited;

	std::unordered_set<int> articulationPoints;
	std::vector<Edge> bridges;
	std::unordered_map<int, int> tin, fup;
	int timer;

	void markBiggestConnectedComponent();

	void cntVertexesInComponent(int vertex);

	void findEccentrisities();

	void findEccentrisity(int vertex);
	
	void resetTimerCharacteristics();

	void findArticulationPoints(int vertex, int parent = -1);

	void findBridges(int vertex, int parent = -1);

	//for trees
	long double getWeight(int vertex);

	//for trees
	long double getBranchWeight(int vertex, int parent);

	friend std::ostream& operator<<(std::ostream&, const Graph&);
};
