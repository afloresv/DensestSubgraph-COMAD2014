#include <iostream>
#include <cmath>
#include <vector>
#include <map>

using namespace std;

class Subgraph {

	Graph &G;

	public:
	vector<bool> S;
	int V, E;

	Subgraph(Graph &_G) : G(_G) {
		S = vector<bool>(G.V,true);
		V = G.V;
		E = G.E;
	}

	bool operator[] (const int i) { return S[i]; }
	void operator=  (Subgraph sg) {
		S = sg.S;
		V = sg.V;
		E = sg.E;
	}

	void set(int i, bool value) {
		S[i] = value;
	}

	void Calculate() {
		V = E = 0;
		for (int i=0 ; i<G.V ; i++) {
			if (!S[i]) continue;
			V++;
			for (int j=G[i].size()-1 ; j>=0 ; j--) {
				if (!S[G[i][j]]) continue;
				if (i==G[i][j]) E++;
				E++;
			}
		}
		E /= 2;
	}

	double Density() { return (double)E/V; }

	void Print() {

		printf("%f\t{",Density());

		int comma = false;
		for (int i=0 ; i<G.V ; i++) if (S[i]) {
			printf((comma ? ",%d" : "%d"),G.ExternalID(i));
			comma = true;
		}

		printf("}\t{");

		comma = false;
		for (int i=0 ; i<G.V ; i++) {
			if (!S[i]) continue;
			for (int j=G[i].size()-1 ; j>=0 ; j--) {
				if (!S[G[i][j]]) continue;
				printf(
					(comma ? ",(%d,%d)" : "(%d,%d)"),
					G.ExternalID(i),
					G.ExternalID(G[i][j])
				);
				comma = true;
			}
			S[i] = false;
		}

		printf("}\n");
	}

	Graph Shrink() {
		Graph _G(V);
		_G.E = E;
		for (int i=0 ; i<G.V ; i++) {
			if (!S[i]) continue;
			for (int j=G[i].size()-1 ; j>=0 ; j--) {
				if (!S[G[i][j]]) continue;
				_G.AddDirEdge(G.ExternalID(i),G.ExternalID(G[i][j]));
			}
		}
		return _G;
	}
};
