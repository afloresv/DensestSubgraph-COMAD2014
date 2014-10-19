#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include "Graph.cc"
#include "Subgraph.cc"
#include "GreedyRemoval.cc"
#include "input.cc"

using namespace std;

Graph G;

int main(int argc, char *argv[]) {

	ReadGraph(argv[argc-1],G);
	Subgraph S(G);
	GreedyRemoval R(G,S);

	while (R.E > 0) {
		if (!R.BatchRemoval()) {
			if (R.E <= 0) break;
			R.CheckDensity();
			R.RemoveNext();
			R.CheckDensity();
		}
	}

	S.Print();

	return 0;
}
