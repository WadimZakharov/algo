/*
11_1. Цикл минимальной длины
        Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
Ввод: v: кол-во вершин (макс. 50000), n: кол-во ребер (макс. 200000), n пар реберных вершин.
Вывод: одно целое число равное длине минимального цикла. Если цикла нет, то вывести -1.
*/
#include <iostream>
#include <vector>
#include<queue>
#include <unordered_set>
using namespace std;
class Graph{
public:
    Graph(int N);
    ~Graph();
    Graph(const Graph&) = delete;
    Graph(Graph&&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph& operator=(Graph&&) = delete;

    void LoadNode(int e1, int e2);
    int SearchMinСycleBFS();
private:
    vector<unordered_set<int>> Siblings;
};

Graph::Graph(int N)
{
    Siblings = vector<unordered_set<int>>(N);
}


Graph::~Graph()
{
    Siblings.clear();
}

void Graph::LoadNode(int e1, int e2)
{
    Siblings[e1].insert(e2);
    Siblings[e2].insert(e1);
}

int Graph::SearchMinСycleBFS()
{
    int len_cycle = -1;
    for (int start_node=0; start_node < Siblings.size(); start_node++)
    {
        queue<int> queue_node;
        vector<int> dist(Siblings.size());
        vector<bool> visit_node(Siblings.size());
        vector<int> parents_node(Siblings.size());

        queue_node.push(start_node);
        visit_node[start_node] = true;
        parents_node[start_node] = -1;

        while (!queue_node.empty()) {
            int actual_node = queue_node.front();
            queue_node.pop();

            if (dist[actual_node]*2 + 1 >= len_cycle && len_cycle != -1)
                break;

            auto adjacency_list = Siblings[actual_node];
            for (auto node:adjacency_list) {
                if (!visit_node[node])
                {
                    visit_node[node] = true;
                    queue_node.push(node);
                    dist[node] = dist[actual_node] + 1;
                    parents_node[node] = actual_node;
                }
                else if (parents_node[node] != actual_node && node != parents_node[actual_node])
                {
                    if (dist[actual_node] + dist[node] + 1 < len_cycle || len_cycle == -1)
                        len_cycle = dist[actual_node] + dist[node] + 1;
                    break;
                }
            }
        }
    }

    return len_cycle;
}

int main() {
    int N = 0;
    int NEdge = 0;
    cin>>N;
    cin>>NEdge;
    Graph graph(N);

    int e1=0;
    int e2=0;
    for (int i=0;i<NEdge;i++)
    {
        cin >> e1 >> e2;
        graph.LoadNode(e1,e2);
    }

    cout << graph.SearchMinСycleBFS();
    return 0;
}