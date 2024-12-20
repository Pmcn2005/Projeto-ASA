#include <algorithm>
#include <climits>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef struct {
    int station_number;
    set<int> connections;
} Station;

typedef struct {
    int line_number;
    set<int> connections;
    set<int> stations;
    string color = "white";
} Line;

void BFS(int start, vector<Line>* lines, vector<int>* distances);

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    int n, m, num_l, x, y, l;

    cin >> n >> m >> num_l;

    vector<int> connections(n, 0);  // Verifica se cada estação está conectada
    vector<Line> lines(num_l);      // Grafo das linhas

    // Leitura dos grafos
    for (int i = 0; i < m; i++) {
        cin >> x >> y >> l;

        connections[x - 1] = 1;
        connections[y - 1] = 1;
        lines[l - 1].line_number = l;
        lines[l - 1].stations.insert(x);
        lines[l - 1].stations.insert(y);
    }

    // Verifica se há uma linha com todas as estações, logo não é preciso mudar
    // de linha
    for (int i = 0; i < num_l; i++) {
        if ((int)lines[i].stations.size() == n) {
            cout << "0\n";
            return 0;
        }
    }

    // Verifica se há estações sem conexão
    for (int i = 0; i < n; i++) {
        if (connections[i] == 0) {
            cout << "-1\n";
            return 0;
        }
    }

    // Verifica se há estações únicas
    for (int i = 0; i < num_l; i++) {
        set<int> intersection0;
        for (int j = 0; j < num_l; j++) {
            if (i != j) {
                set<int> intersection;
                set_intersection(
                    lines[i].stations.begin(), lines[i].stations.end(),
                    lines[j].stations.begin(), lines[j].stations.end(),
                    inserter(intersection, intersection.begin()));

                if (intersection.size() != 0) {
                    lines[i].connections.insert(j);
                }
                intersection0.insert(intersection.begin(), intersection.end());
            }
        }
    }

    vector<int> max_distances(num_l);

    for (int i = 0; i < num_l; i++) {
        vector<int> distances(num_l);
        BFS(i, &lines, &distances);
        max_distances[i] = *max_element(distances.begin(), distances.end());
    }

    int max_length = *max_element(max_distances.begin(), max_distances.end());

    cout << max_length << "\n";

    return 0;
}

void BFS(int start, vector<Line>* lines, vector<int>* distances) {
    for (int i = 0; i < (int)(*lines).size(); i++) {
        (*lines)[i].color = "white";
        (*distances)[i] = INT_MAX;
    }

    queue<int> q;
    q.push(start);
    (*lines)[start].color = "gray";
    (*distances)[start] = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        (*lines)[node].color = "black";

        for (int neighbor : (*lines)[node].connections) {
            if ((*lines)[neighbor].color == "white") {
                (*lines)[neighbor].color = "gray";
                (*distances)[neighbor] = (*distances)[node] + 1;
                q.push(neighbor);
            }
        }
    }
}