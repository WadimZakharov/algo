/*
 14. «MST». Дан неориентированный связный граф.
 Требуется найти вес минимального остовного дерева в этом графе.
 Вариант 1. С помощью алгоритма Прима.
 */
#include <iostream>
#include <vector>
#include <set>
using namespace std;

class Graph
{
public:
    Graph(size_t N);
    ~Graph();
    Graph(const Graph&) = delete;
    Graph(Graph&&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph& operator=(Graph&&) = delete;

    void LoadGraph(int e1, int e2, int weight);
    size_t Prim();
private:
    size_t size;
    vector<vector<pair<size_t,size_t>>> Siblings;
};

Graph::Graph(size_t N)
{
    size = N;
    Siblings = vector<vector<pair<size_t,size_t>>>(N);
}

Graph::~Graph()
{
    Siblings.clear();
}

void Graph::LoadGraph(int e1, int e2, int weight)
{
    Siblings[e1].push_back(make_pair(e2, weight));
    Siblings[e2].push_back(make_pair(e1, weight));
}

size_t Graph::Prim()
{
    size_t resault=0;
    set<pair<size_t,size_t>> Nodes;
    Nodes.insert(make_pair(0,0));
    vector<bool> visit(size);
    vector<int> tree_weights(size, 10000);
    tree_weights[0] = 0;

    for (size_t i=0; i<size; i++)
    {
         auto node = Nodes.begin();
         Nodes.erase(Nodes.begin());
         auto neighbors = Siblings[node->first];
         visit[node->first] = true;

         for (auto neighbor:neighbors)
         {
             size_t edge = neighbor.first;
             size_t weight = neighbor.second;

             if (weight < tree_weights[edge] && !visit[edge])
             {
                 Nodes.erase(std::make_pair(edge, tree_weights[edge]));
                 tree_weights[edge] = weight;
                 resault += weight;
                 Nodes.insert(std::make_pair(edge, weight));
             }
         }
    }
    return resault;
}


int main()
{
    size_t N, count_edge;
    size_t e1, e2, weight;
    cin >> N >> count_edge;
    Graph graph(N);
    for (size_t i = 0; i < count_edge; i++)
    {
        cin >> e1 >> e2 >> weight;
        graph.LoadGraph(e1-1, e2-1, weight);
    }

    cout << graph.Prim();
}
