#include "Graph.h"
#include "DisjointSets.h"
#include<iostream>
#include<queue>
#include<algorithm>

/*inline bool isEccentrisityLess(Graph::Vertex v1, Graph::Vertex v2) {
	if (!v2.isInBiggestConnectedComponent) {
		return false;
	}
	return v2.eccentrisity < v1.eccentrisity;
}

inline bool isEccentrisityGreater(Graph::Vertex v1, Graph::Vertex v2) {
	if (!v2.isInBiggestConnectedComponent) {
		return false;
	}
	return v2.eccentrisity > v1.eccentrisity;
}*/

Graph::Graph() :
	isCorrectVertexStatus(true)
{}

void Graph::addVertex(int vertexNumber) {
	if (!vertexes.count(vertexNumber)) {
		vertexes[vertexNumber] = { -1, -1, false };
		vertexesEdges[vertexNumber].clear();
	}
	isCorrectVertexStatus = false;
}

void Graph::addEdge(int from, int to, long double weight) {
	addEdge({ from, to, weight });
}

void Graph::addEdge(Edge edge) {
	vertexesDists[edge.from][edge.to] = edge.weight;
	vertexesDists[edge.to][edge.from] = edge.weight;
	edges.push_back(edge);
	vertexesEdges[edge.from].push_back(edge);
	if (!vertexes.count(edge.from)) {
		vertexes[edge.from] = { -1, -1, false };
	}
	if (!vertexes.count(edge.to)) {
		vertexes[edge.to] = { -1, -1, false };
	}
	Edge parallelEdge = { edge.to, edge.from, edge.weight };
	edges.push_back(parallelEdge);
	vertexesEdges[parallelEdge.from].push_back(parallelEdge);
	isCorrectVertexStatus = false;
}

size_t Graph::getdelta() {
	if (!isCorrectVertexStatus) {
		findEccentrisities();
	}
	size_t ans = INF;
	for (const auto& v : vertexes) {
		if (v.second.isInBiggestConnectedComponent) {
			ans = std::min(ans, vertexesEdges[v.first].size());
		}
	}
	return ans;
}

size_t Graph::getDelta() {
	if (!isCorrectVertexStatus) {
		findEccentrisities();
	}
	size_t ans = 0;
	for (const auto& v : vertexes) {
		if (v.second.isInBiggestConnectedComponent) {
			ans = std::max(ans, vertexesEdges[v.first].size());
		}
	}
	return ans;
}

int Graph::getRadius() {
	if (!isCorrectVertexStatus) {
		findEccentrisities();
	}
	//return (*std::max_element(vertexes.begin(), vertexes.end(), isEccentrisityLess)).second.eccentrisity;
	int ans = INF;
	for (const auto& v : vertexes) {
		if (v.second.isInBiggestConnectedComponent) {
			ans = std::min(ans, v.second.eccentrisity);
		}
	}
	return ans;
}

int Graph::getDiameter() {
	if (!isCorrectVertexStatus) {
		findEccentrisities();
	}
	//return (*std::max_element(vertexes.begin(), vertexes.end(), isEccentrisityGreater)).second.eccentrisity;
	int ans = 0;
	for (const auto& v : vertexes) {
		ans = std::max(ans, v.second.eccentrisity);
	}
	return ans;
}

std::vector<int> Graph::getCenter() {
	std::vector<int> center;
	int radius = getRadius();
	for (const auto& v : vertexes) {
		if (v.second.eccentrisity == radius) {
			center.push_back(v.first);
		}
	}
	return center;
}

std::unordered_set<int>& Graph::getArticulationPoints() {
	resetTimerCharacteristics();
	findArticulationPoints(0);
	return articulationPoints;
}

std::vector<Graph::Edge> Graph::getBridges() {
	resetTimerCharacteristics();
	findBridges(0);
	return bridges;
}

Graph Graph::getMST() {
	markBiggestConnectedComponent();
	Graph ans;
	std::sort(edges.begin(), edges.end());
	DisjointSets vertexesSets(vertexes.size());
	for (int i(0); i < edges.size(); ++i) {
		Edge e = edges[i];
		if (!vertexes[e.from].isInBiggestConnectedComponent) {
			continue;
		}
		if (vertexesSets.findSet(e.from) != vertexesSets.findSet(e.to)) {
			ans.addEdge(e);
			vertexesSets.unionSets(e.from, e.to);
		}
	}
	return ans;
}

std::vector<int> Graph::getCentroid() {
	std::vector<int> ans;
	std::map<int, long double> weights;
	long double minWeight = INF;
	for (const auto& v : vertexes) {
		weights[v.first] = getWeight(v.first);
		minWeight = std::min(minWeight, weights[v.first]);
	}
	for (const auto& v : vertexes) {
		if (weights[v.first] == minWeight) {
			ans.push_back(v.first);
		}
	}
	return ans;
}

std::vector<int> Graph::getPruferCode() {
	Graph copy = *this;
	for (auto v : copy.vertexes) {
		visited[v.first] = false;
	}
	int unvisitedSize = vertexes.size();
	std::set<int> leaves;
	for (auto v : copy.vertexes) {
		if (copy.vertexesEdges[v.first].size() == 1) {
			leaves.insert(v.first);
		}
	}
	std::vector<int> ans;
	while (unvisitedSize > 2) {
		int v = *leaves.begin();
		leaves.erase(leaves.begin());
		--unvisitedSize;
		int p = copy.vertexesEdges[v][0].to;
		ans.push_back(p);
		copy.vertexesEdges[v].clear();
		for (auto u = copy.vertexesEdges[p].begin(); u != copy.vertexesEdges[p].end(); ++u) {
			if (u->to == v) {
				copy.vertexesEdges[p].erase(u);
				break;
			}
		}
		if (copy.vertexesEdges[p].size() == 1) {
			leaves.insert(p);
		}
	}
	return ans;
}

void Graph::printAdjancencyListForPython(std::ostream& cout) {
	cout << '{';
	for (auto v : vertexesEdges) {
		cout << v.first + 1 << " : [";
		for (int i(0); i < v.second.size(); ++i) {
			cout << v.second[i].to + 1;
			if (i != v.second.size() - 1) {
				cout << ", ";
			}
		}
		cout << "], ";
	}
	cout << '}';
}

void Graph::markBiggestConnectedComponent() {
	for (const auto& v : vertexes) {
		visited[v.first] = false;
	}
	for (const auto& v : vertexes) {
		if (!visited[v.first]) {
			cntVertexesInComponent(v.first);
		}
	}
	int maxComponentSize = 0;
	for (const auto& v : vertexes) {
		maxComponentSize = std::max(maxComponentSize, v.second.connectedComponentSize);
	}
	for (auto& v : vertexes) {
		if (v.second.connectedComponentSize == maxComponentSize) {
			v.second.isInBiggestConnectedComponent = true;
		}
	}
}

void Graph::cntVertexesInComponent(int vertex) {
	std::vector<int> component;
	component.push_back(vertex);
	std::queue<int> q;
	q.push(vertex);
	visited[vertex] = true;
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		for (auto u : vertexesEdges[v]) {
			if (!visited[u.to]) {
				q.push(u.to);
				component.push_back(u.to);
				visited[u.to] = true;
			}
		}
	}
	for (int i(0); i < component.size(); ++i) {
		int v = component[i];
		vertexes[v].connectedComponentSize = component.size();
	}
}

void Graph::findEccentrisities() {
	markBiggestConnectedComponent();
	for (const auto& v : vertexes) {
		findEccentrisity(v.first);
	}
}

void Graph::findEccentrisity(int vertex) {
	if (!vertexes[vertex].isInBiggestConnectedComponent) {
		return;
	}
	std::map<int, int> dists;
	for (const auto& v : vertexes) {
		dists[v.first] = -1;
	}
	dists[vertex] = 0;
	std::queue<int> q;
	q.push(vertex);
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		for (int i(0); i < vertexesEdges[v].size(); ++i) {
			int u = vertexesEdges[v][i].to;
			if (dists[u] == -1) {
				q.push(u);
				dists[u] = dists[v] + 1;
			}
		}
	}
	//vertexes[vertex].eccentrisity = (std::max_element(dists.begin(), dists.end(), isEccentrisityGreater))->second;
	vertexes[vertex].eccentrisity = 0;
	for (auto dist : dists) {
		vertexes[vertex].eccentrisity = std::max(vertexes[vertex].eccentrisity, dist.second);
	}
}

void Graph::resetTimerCharacteristics() {
	timer = 0;
	tin.clear();
	fup.clear();
	for (const auto& v : vertexes) {
		tin[v.first] = -1;
		fup[v.first] = INF;
	}
}

void Graph::findArticulationPoints(int vertex, int parent) {
	tin[vertex] = timer++;
	fup[vertex] = tin[vertex];
	int children = 0;
	for (int i(0); i < vertexesEdges[vertex].size(); ++i) {
		int u = vertexesEdges[vertex][i].to;
		if (u == parent) {
			continue;
		}
		if (tin[u] >= 0) {
			fup[vertex] = std::min(fup[vertex], tin[u]);
		} else {
			findArticulationPoints(u, vertex);
			fup[vertex] = std::min(fup[vertex], fup[u]);
			if (fup[u] >= tin[vertex] && parent != -1) {
				articulationPoints.insert(vertex);
			}
			++children;
		}
	}
	if (parent == -1 && children > 1) {
		articulationPoints.insert(vertex);
	}
}

void Graph::findBridges(int vertex, int parent) {
	tin[vertex] = timer++;
	fup[vertex] = tin[vertex];
	for (int i(0); i < vertexesEdges[vertex].size(); ++i) {
		int u = vertexesEdges[vertex][i].to;
		if (u == parent) {
			continue;
		}
		if (tin[u] >= 0) {
			fup[vertex] = std::min(fup[vertex], tin[u]);
		} else {
			findBridges(u, vertex);
			fup[vertex] = std::min(fup[vertex], fup[u]);
			if (fup[u] > tin[vertex]) {
				bridges.push_back({ vertex, u });
			}
		}
	}
}

std::ostream& operator<<(std::ostream& cout, const Graph& graph) {
	cout << "vertexes: { ";
	for (const auto& v : graph.vertexes) {
		cout << v.first + 1 << ", ";
	}
	cout << "}\n";
	cout << "edges: { ";
	std::set<std::set<int>> edges;
	for (const auto& e : graph.edges) {
		edges.insert({ e.from, e.to });
	}
	for (const auto& e : edges) {
		cout << "{";
		for (auto v : e) {
			cout << v + 1;
			if (v == *e.begin()) {
				cout << ", ";
			}
		}
		cout << "}, ";
	}
	cout << "}";
	return cout;
}

long double Graph::getWeight(int vertex) {
	for (const auto& v : vertexes) {
		visited[v.first] = false;
	}
	visited[vertex] = true;
	long double weight = 0;
	for (const auto& v : vertexesEdges[vertex]) {
		weight = std::max(weight, getBranchWeight(v.to, vertex));
	}
	return weight;
}

long double Graph::getBranchWeight(int vertex, int parent) {
	long double weight = 0;
	std::queue<int> q;
	q.push(vertex);
	visited[vertex] = true;
	weight += vertexesDists[vertex][parent];
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		for (auto u : vertexesEdges[v]) {
			if (!visited[u.to]) {
				q.push(u.to);
				visited[u.to] = true;
				weight += vertexesDists[v][u.to];
			}
		}
	}
	return weight;
}