#include<iostream>
#include<fstream>
#include"Graph.h"

std::ifstream fin("graphEdges.txt");
std::ofstream fout("output.txt", std::ios::app);

int main() {
	int vertexSize, edgeSize;
	fin >> vertexSize >> edgeSize;
	Graph graph;
	for (int i(0); i < vertexSize; ++i) {
		graph.addVertex(i);
	}

	for (int i(0); i < edgeSize; ++i) {
		int from, to;
		long double weight;
		fin >> from >> to >> weight;
		graph.addEdge(from - 1, to -1 , weight);
	}
	fout << "delta(G) = " << graph.getdelta() << '\n';
	fout << "Delta(G) = " << graph.getDelta() << '\n';
	fout << "rad(G) = " << graph.getRadius() << '\n';
	fout << "diam(G) = " << graph.getDiameter() << '\n';
	auto ans = graph.getCenter();
	fout << "center(G) = {";
	for (int i(0); i < ans.size(); ++i) {
		fout << ans[i] + 1;
		if (i != ans.size() - 1) {
			fout << ", ";
		}
	}
	fout << "}\n";
	fout << "articulation points: ";
	for (auto i : graph.getArticulationPoints()) {
		fout << i + 1 << ' ';
	}
	fout << '\n';
	fout << "bridges: ";
	for (const auto& i : graph.getBridges()) {
		fout << '{' << i.from + 1 << "; " << i.to + 1 << "} ";
	}
	fout << '\n';
	Graph mst = graph.getMST();
	fout << "MST:\n";
	fout << mst << '\n';
	std::vector<int> centroid = mst.getCentroid();
	fout << "Centroid: {";
	for (int i(0); i < centroid.size(); ++i) {
		fout << centroid[i] + 1;
		if (i != centroid.size() - 1) {
			fout << ", ";
		}
	}
	fout << "}\n";
	fout << "k = ";
	std::vector<int> pruferCode = mst.getPruferCode();
	for (int i(0); i < pruferCode.size(); ++i) {
		++pruferCode[i];
		if (pruferCode[i] > 46) {
			--pruferCode[i];
		}
		if (pruferCode[i] > 27) {
			--pruferCode[i];
		}
		if (pruferCode[i] > 19) {
			--pruferCode[i];
		}
		if (pruferCode[i] > 18) {
			--pruferCode[i];
		}
		if (pruferCode[i] > 9) {
			--pruferCode[i];
		}
		fout << pruferCode[i] << ' ';
	}
	fout << '\n';
	graph.printAdjancencyListForPython(fout);


	return 0;
}