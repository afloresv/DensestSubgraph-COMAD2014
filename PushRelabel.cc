// Adjacency list implementation of FIFO push relabel maximum flow
// with the gap relabeling heuristic.
// Downloaded from http://web.stanford.edu/~liszt90/acm/notebook.html#file3
// and modified by Alejandro Flores: reducing the memory requirements and
// adjusting it to implement the Densest Subgraph algorithm proposed by
// A. V. Goldberg at 'Finding a Maximum Density Subgraph'
// http://www.eecs.berkeley.edu/Pubs/TechRpts/1984/CSD-84-171.pdf

#include <cmath>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

struct Edge {
	int to, cap, flow, index;
	Edge(int to, int cap, int flow, int index) :
		to(to), cap(cap), flow(flow), index(index) {}
};

struct PushRelabel {
	int N, _g;
	vector<vector<Edge> > G;
	vector<int> excess, dist, active, count;
	queue<int> Q;
	Subgraph &S;

	PushRelabel(Graph &_G, Subgraph &_S) : S(_S) {
		N = _G.V + 2;
		G = vector<vector<Edge> >(N);
		_g = 0;
		int s=_G.V, t=_G.V+1;

		for (int i=0 ; i<_G.V ; i++) {
			for (int j=_G[i].size()-1 ; j>=0 ; j--)
				AddEdge(i,_G[i][j],1);
			AddEdge(s,i,_G.E);
			AddEdge(i,t,_G.E-_G[i].size());
		}
	}

	void AddEdge(int from, int to, int cap) {
		G[from].push_back(Edge(to, cap, 0, G[to].size()));
		if (from == to) G[from].back().index++;
		G[to].push_back(Edge(from, 0, 0, G[from].size() - 1));
	}

	void Enqueue(int v) { 
		if (!active[v] && excess[v] > 0) { active[v] = true; Q.push(v); }
	}

	void Push(int from, Edge &e) {
		int amt = min(excess[from], e.cap-e.flow);
		if (dist[from] <= dist[e.to] || amt == 0) return;
		e.flow += amt;
		G[e.to][e.index].flow -= amt;
		excess[e.to] += amt;
		excess[from] -= amt;
		Enqueue(e.to);
	}

	void Gap(int k) {
		for (int v = 0; v < N; v++) {
			if (dist[v] < k) continue;
			count[dist[v]]--;
			dist[v] = max(dist[v], N+1);
			count[dist[v]]++;
			Enqueue(v);
		}
	}

	void Relabel(int v) {
		count[dist[v]]--;
		dist[v] = 2*N;
		for (int i = 0; i < G[v].size(); i++)
			if (G[v][i].cap - G[v][i].flow > 0)
				dist[v] = min(dist[v], dist[G[v][i].to] + 1);
		count[dist[v]]++;
		Enqueue(v);
	}

	void Discharge(int v) {
		for (int i=0 ; excess[v]>0 && i<G[v].size() ; i++) Push(v,G[v][i]);
		if (excess[v] > 0) {
			if (count[dist[v]] == 1)
				Gap(dist[v]);
			else
				Relabel(v);
		}
	}

	void BFS(int v) {
		for (int i=G[v].size()-1 ; i>=0 ; i--) {
			if (G[v][i].to != N-2 && G[v][i].cap - G[v][i].flow > 0 && !S[G[v][i].to]) {
				S.set(G[v][i].to,true);
				BFS(G[v][i].to);
			}
		}
	}

	void GetMinCut(int g) {

		int s=N-2, t=N-1;
		excess = vector<int>(N);
		dist = vector<int>(N);
		active = vector<int>(N);
		count = vector<int>(2*N);
		
		int fix = 2*g - 2*_g;
		_g = g;
		for (int i=0; i<G.size() ; i++)
		for (int j=0; j<G[i].size() ; j++) {
			if (G[i][j].to == t)
				G[i][j].cap += fix;
			G[i][j].flow = 0;
		}

		count[0] = N-1;
		count[N] = 1;
		dist[s] = N;
		active[s] = active[t] = true;
		for (int i = 0; i < G[s].size(); i++) {
			excess[s] += G[s][i].cap;
			Push(s,G[s][i]);
		}

		while (!Q.empty()) {
			int v = Q.front();
			Q.pop();
			active[v] = false;
			Discharge(v);
		}

		excess.clear();
		dist.clear();
		active.clear();
		count.clear();
		
		for (int i=0 ; i<s ; i++)
			S.set(i,false);
		BFS(s);
		S.Calculate();
	}
};
