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

	while (R.BatchRemoval() || R.SingleRemoval())
		R.CheckDensity();

	S.Print();

	return 0;
}
