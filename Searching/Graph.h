#include <iostream>
#include <list>
#include <queue>

namespace Searching
{

    class Graph
    {
    public:
        Graph(int vertices);
        void addEdge(int src, int dest);
        void BFS(int startVertex);

        int numVertices;
        std::list<int> *adjLists;
        bool *visited;
    };

    Graph::Graph(int vertices)
    {
        numVertices = vertices;
        adjLists = new std::list<int>[vertices];
        visited = new bool[numVertices];
        for (int i = 0; i < numVertices; i++)
        {
            visited[i] = false;
        }
    }

    void Graph::addEdge(int src, int dest)
    {
        adjLists[src].push_back(dest);
        adjLists[dest].push_back(src);
    }

} // namespace Searching
