#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include "Graph.cc"
#include "Subgraph.cc"
#include "PushRelabel.cc"
#include "input.cc"

using namespace std;

Graph G;

int main(int argc, char *argv[]) {

	ReadGraph(argv[argc-1],G);
	Subgraph S(G), _S(G);
	PushRelabel P(G,_S);

	int l=0, u=G.E, g=-1;
	while (true) {
		int _g = g;
		g = (u+l)/2;
		if (_g==g) break;
		P.GetMinCut(g);
		if (_S.V == 0) {
			u = g;
		} else {
			l = g;
			S = _S;
		}
	}

	S.Print();

	return 0;
}
