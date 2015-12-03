#include <iostream.h>
#include <list.h>

using namespace std;

class Graph
{
   int V;
   list<int> *adj;
	public:
	    Graph(int V);
		//function to add edge to graph
	    void addEdge(int v, int w);
	    void BFS(int s);

};

Graph::Graph(int V)
{
    this->V = v;
    adj = new list<int>[V];

}
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);

}
