#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

struct NodeCompare {
	int *D;
	NodeCompare() {}
	NodeCompare(int *_D) : D(_D) {}
	bool operator() (int a, int b) {
		return (D[a] < D[b]);
	}
};

class GreedyRemoval {

	Graph &G;
	int *degree, *order, *pos, minpos;
	double density;
	NodeCompare CompNodes;

	void ReorderNode(int v) {
		for (int i=pos[v] ; i>0 ; i--) {
			if (degree[v] < degree[order[i-1]]) {
				pos[v]--;
				pos[order[i-1]]++;
				swap(order[i-1],order[i]);
			} else break;
		}
	}

	public:
	Subgraph &S;
	int V, E;

	GreedyRemoval(Graph &_G, Subgraph &_S) : G(_G), S(_S) {
		degree = new int[G.V];
		CompNodes = NodeCompare(degree);
		order  = new int[G.V];
		pos    = new int[G.V];
		minpos = 0;
		V = G.V;
		E = G.E;
		density = G.Density();
		for (int i=0 ; i<G.V ; i++) {
			degree[i] = G[i].size();
			order[i] = i;
		}
		sort(order,order+G.V,CompNodes);
		for (int i=0 ; i<G.V ; i++)
			pos[order[i]] = i;
	}

	void RemoveNext() {
		int v = order[minpos];
		V--;
		E -= degree[v];
		degree[v] = 0;
		minpos++;
		for (int i=G[v].size()-1 ; i>=0 ; i--) {
			if (degree[G[v][i]] > 1) {
				degree[G[v][i]]--;
				ReorderNode(G[v][i]);
			} else if (degree[G[v][i]] == 1) {
				degree[G[v][i]] = 0;
				V--;
				minpos++;
				ReorderNode(G[v][i]);
			}
		}
	}

	bool BatchRemoval() {
		int _V = V;
		while (E>0) {
			int k = E/V;
			if (degree[order[minpos]] > k) break;
			// Batch removal of nodes with degree <= k
			for (int i=minpos ; i<G.V && degree[order[i]]<=k ; i++) {
				V--;
				E -= degree[order[i]];
				degree[order[i]] = 0;
				minpos++;
				for (int j=G[order[i]].size()-1 ; j>=0 ; j--) {
					if (degree[G[order[i]][j]] > 0)
						degree[G[order[i]][j]]--;
				}
			}
			sort(order,order+G.V,CompNodes);
			for (int i=0 ; i<G.V ; i++)
				pos[order[i]] = i;
			for ( ; minpos<G.V && degree[order[minpos]]==0 ; minpos++, V--);
		}
		return (V != _V);
	}

	void CheckDensity() {
		double temp_density = (double)E/V;
		if (density < temp_density) {
			density = temp_density;
			S.V = V;
			S.E = E;
			for (int i=0 ; i<G.V ; i++)
				S.set(i, degree[i]>0);
		}
	}
};
