#include <iostream>
#include <limits>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <set>

using namespace std;

#define repeat(i,n) for (int i=0 ; i<n ; i++)
#define Natural int

#define Node int
#define Graph vector<Node>*

Graph G;
Natural V, E, T;

int *D, *pos, min_pos = 0;
Node *order, cnode = 0;
map<Node,Node> idx;

bool *S;
double density;

Node GetNode(Node a) {
	map<Node,Node>::iterator it = idx.find(a);
	if (it == idx.end()) {
		idx[a] = cnode;
		return cnode++;
	} else return (it->second);
}

bool CompNodes(Node a, Node b) {
	return D[a] < D[b];
}

void ReadGraph() {

	bool isDigraph = false;

	// First 4 lines
	string line;
	char _w1[50], _w2[50];
	getline(cin,line);
	if (line[2]=='D') isDigraph = true;
	getline(cin,line);
	scanf("%[^:]: %u %[^:]: %u",_w1,&V,_w2,&E);
	repeat(i,2) getline(cin,line);

	// Initial Density
	density = (double)E/V;
	T = V;

	G = new vector<Node>[V];
	D = new int[V];
	pos = new int[V];
	order = new Node[V];
	S = new bool[V];

	// Let's read every edge
	Node a, b;
	while ( scanf("%u\t%u",&a,&b) != EOF ) {
		a = GetNode(a);
		b = GetNode(b);
		G[a].push_back(b);
		G[b].push_back(a);
	}

	if (isDigraph) {
		E = 0;
		for (int i=0 ; i<T ; i++) {
			set<Node> s(G[i].begin(),G[i].end());
			G[i] = vector<Node>(s.begin(),s.end());
			E += G[i].size();
		}
		E /= 2;
		density = (double)E/V;
	}

	for (int i=0 ; i<T ; i++) {
		D[i] = G[i].size();
		S[i] = true;
		order[i] = i;
	}

	sort(order,order+T,CompNodes);

	for (int i=0 ; i<T ; i++)
		pos[order[i]] = i;
}

void ReorderNode(Node v) {
	for (int i=pos[v] ; i>0 ; i--) {
		if (D[v] < D[order[i-1]]) {
			pos[v]--;
			pos[order[i-1]]++;
			swap(order[i-1],order[i]);
		} else break;
	}
}

void RemoveNode(Node v) {
	V--;
	D[v] = 0;
	min_pos++;
	for (int i=G[v].size()-1 ; i>=0 ; i--) {
		if (D[G[v][i]] > 1) {
			D[G[v][i]]--;
			E--;
			ReorderNode(G[v][i]);
		} else if (D[G[v][i]] == 1) {
			D[G[v][i]] = 0;
			V--;
			E--;
			min_pos++;
			ReorderNode(G[v][i]);
		}
	}
	//sort(order,order+T,CompNodes);
}

// +-----------------------------------------------------+
// | Instead of removing one node per iteration, this    |
// | modification implements a method for batch removal. |
// | Given the set of n nodes with degree <= k in the    |
// | current graph G = (V,E), in the worst case we       |
// | remove n nodes and k*n edges. The density of the    |
// | remaining graph is greater or equal than the        |
// | density of the current graph if |E| >= k*|V|.       |
// +-----------------------------------------------------+

bool BatchRemoval() {

	Natural _V = V;
	while (true) {
		int k = E/V;
		if (D[order[min_pos]] > k) break;
		// Batch removal of nodes with degree <= k
		for (int i=min_pos ; i<T && D[order[i]]<=k ; i++) {
			V--;
			E -= D[order[i]];
			D[order[i]] = 0;
			for (int j=G[order[i]].size()-1 ; j>=0 ; j--) {
				if (D[G[order[i]][j]] > 0)
					D[G[order[i]][j]]--;
			}
			min_pos++;
		}

		sort(order,order+T,CompNodes);
		for (int i=0 ; i<T ; i++)
			pos[order[i]] = i;
		for ( ; D[order[min_pos]]==0 ; min_pos++, V--);
	}

	if (_V!=V)
		cout << "Batch removal of " << _V-V << " nodes" << endl;

	return (_V!=V);
}

void CheckDensity() {
	double temp_density = (double)E/V;
	if (density < temp_density) {
		density = temp_density;
		cout << "New density: " << density << " (" << V << "," << E << ")" << endl;
		for (int i=0 ; i<T ; i++)
			S[i] = (D[i] > 0);
	}
}

int main() {

	ReadGraph();
	cout << "Initial density: " << density << " (" << V << "," << E << ")" << endl;

	while (E>0) {
		if (!BatchRemoval()) {
			CheckDensity();
			Node v = order[min_pos];
			cout << "Manual removal of " << v << " with degree " << D[v] << endl;
			RemoveNode(v);
			CheckDensity();
		}
	}

	// The result is in S
	cout << "Final Result" << endl;
	cout << density << endl;

	return 0;
}
