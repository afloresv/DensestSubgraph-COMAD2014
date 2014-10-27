#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include "Graph.cc"
#include "Subgraph.cc"
#include "GreedyRemoval.cc"
#include "PushRelabel.cc"
#include "input.cc"

using namespace std;

Graph G;

int main(int argc, char *argv[]) {

	ReadGraph(argv[argc-1],G);

	Subgraph S1(G);
	GreedyRemoval R(G,S1);
	R.BatchRemoval();
	R.CheckDensity();

	G = S1.Shrink();

	Subgraph S2(G), S(G);
	PushRelabel P(G,S2);

	int l=0, u=G.E, g=-1;
	while (true) {
		int _g = g;
		g = (u+l)/2;
		if (_g==g) break;
		P.GetMinCut(g);
		if (S2.V == 0) {
			u = g;
		} else {
			l = g;
			S = S2;
		}
	}

	S.Print();

	return 0;
}
