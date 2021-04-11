import networkx as nx
from networkx.algorithms import approximation
from py2opt.routefinder import RouteFinder
f = open("graphEdgesConnected.txt", "r")
s = f.readline()
ps = [int(i) for i in s.split()]
n = ps[0]
m = ps[1]
graph = nx.Graph()
for i in range(0, m):
    s = f.readline()
    ps = [float(i) for i in s.split()]
    v = int(ps[0])
    u = int(ps[1])
    graph.add_edge(v, u, weight = 1)
f.close()
f = open("outputPy.txt", "w")
f.write("maximum stable set = " + str(approximation.independent_set.maximum_independent_set(graph)) + '\n')
f.write("maximum matching = " + str(nx.max_weight_matching(graph)) + '\n')
f.write("minimum vertex cover = " + str(approximation.vertex_cover.min_weighted_vertex_cover(graph)) + '\n')
f.write("minimum edge cover = " + str(nx.min_edge_cover(graph)))
f.write("minimum path that visits every edge: " + str(list(nx.eulerian_circuit(nx.eulerize(graph)))))
dists = dict(nx.all_pairs_shortest_path_length(graph))
paths = dict(nx.all_pairs_shortest_path(graph))
distsArr = []
listsCnt = 0
for i in range(1, n + 1):
    if i == 9 or i == 27 or i == 19 or i == 46 or i == 18:
        continue
    distsArr.append([])
    listsCnt += 1
    for j in range(1, n + 1):
        if j == 9 or j == 27 or j == 19 or j == 46 or j == 18:
            continue
        distsArr[listsCnt - 1].append(dists[i][j])
vertexes = []
for i in range(1, n + 1):
    if i == 9 or i == 27 or i == 19 or i == 46 or i == 18:
        continue
    vertexes.append(i)
routeFinder = RouteFinder(distsArr, vertexes, iterations = 100)
bestDist, bestRoute = routeFinder.solve()
f.write(str(bestDist) + '\n')
f.write(str(bestRoute) + '\n')
bestWay = []
for i in range(0, listsCnt):
    bestWay.append(bestRoute[i])
    bestWay.append(paths[bestRoute[i]][bestRoute[(i + 1) % listsCnt]])
f.write(str(bestWay))
f.close()
