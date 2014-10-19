#include <iostream>
#include <cstdio>
#include <vector>
#include <set>

using namespace std;

void ReadGraph(char file[], Graph &G) {

	int V;
	bool isDigraph = false;

	FILE *src = fopen(file,"r");

	// First 4 lines
	char line[200];
	fscanf(src,"%[^\n]\n",line);
	if (line[2]=='D') isDigraph = true;
	fscanf(src,"%*[^\n]\n");
	fscanf(src,"%*[^:]: %d %*[^:]: %*d\n",&V);
	fscanf(src,"%*[^\n]\n");

	G = Graph(V);

	int selfloops = 0, a, b;
	while ( fscanf(src,"%d\t%d\n",&a,&b) != EOF ) {
		G.AddEdge(a,b);
		if (a==b) selfloops++;
	}

	fclose(src);

	// Correct adjacency list for digraphs
	if (isDigraph) {
		G.E = selfloops;
		for (int i=0 ; i<V ; i++) {
			set<int> s(G[i].begin(),G[i].end());
			G[i] = vector<int>(s.begin(),s.end());
			G.E += G[i].size();
		}
		G.E /= 2;
	}
}
