#include <iostream>
#include <cmath>
#include <vector>
#include <map>

using namespace std;

class Graph {

	private:
	vector<int> *g;

	public:
	int V, E;

	Graph(int size = 0) {
		V = E = 0;
		g = new vector<int>[size];
		InEx = new int[size];
	}

	vector<int>& operator[](const int i) {
		return g[i];
	}

	double Density() {
		return (double)E/V;
	}

	void AddDirEdge(int a, int b) {
		a = InternalID(a);
		b = InternalID(b);
		g[a].push_back(b);
	}

	void AddEdge(int a, int b) {
		a = InternalID(a);
		b = InternalID(b);
		g[a].push_back(b);
		if (a!=b) g[b].push_back(a);
		E++;
	}

	private:
	map<int,int> ExIn;
	int *InEx;
	int InternalID(int v) {
		map<int,int>::iterator it = ExIn.find(v);
		if (it == ExIn.end()) {
			ExIn[v] = V;
			InEx[V] = v;
			return V++;
		} else return (it->second);
	}

	public:
	int ExternalID(int v) {
		return InEx[v];
	}
};
