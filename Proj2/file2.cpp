#include <algorithm>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Tipo de dado para representar uma ligação (estação destino, linha de metro)
using Connection = tuple<int, int>;

// Função para calcular o índice de conectividade da rede de metro
int metro_connectivity(int n, const vector<vector<Connection>>& graph);

int main() {
    int n, m, l;
    cin >> n >> m >> l;

    // Grafo representado como uma lista de adjacências
    vector<vector<Connection>> graph(n + 1);

    // Leitura das ligações
    for (int i = 0; i < m; ++i) {
        int x, y, line;
        cin >> x >> y >> line;
        graph[x].emplace_back(y, line);
        graph[y].emplace_back(x, line);  // Grafo não-dirigido
    }

    // Calcular e imprimir o índice de conectividade
    cout << metro_connectivity(n, graph) << endl;

    return 0;
}

bool is_connected(int n, const vector<vector<Connection>>& graph) {
    vector<bool> visited(n + 1, false);
    queue<int> q;

    // Começar com a primeira estação
    q.push(1);
    visited[1] = true;

    int count = 1;  // Contar quantos nós estão conectados
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (const auto& connection : graph[current]) {
            int next = get<0>(connection);  // Obter a estação destino
            // Não usamos structured bindings, usamos get<>() para acessar os
            // valores
            if (!visited[next]) {
                visited[next] = true;
                q.push(next);
                ++count;
            }
        }
    }

    return count == n;  // Verifica se todos os nós foram visitados
}

int min_line_changes(int start, int end,
                     const vector<vector<Connection>>& graph) {
    queue<tuple<int, int, int>> q;  // (estação, linha atual, mudanças de linha)
    unordered_map<int, unordered_map<int, int>>
        visited;  // visited[estação][linha]

    // Iniciar a BFS
    for (const auto& connection : graph[start]) {
        int next = get<0>(connection);
        int line = get<1>(connection);
        q.emplace(next, line, 0);
        visited[next][line] = 0;
    }

    while (!q.empty()) {
        auto front = q.front();
        q.pop();

        int current = get<0>(front);
        int current_line = get<1>(front);
        int changes = get<2>(front);

        if (current == end) return changes;  // Chegamos ao destino

        for (const auto& connection : graph[current]) {
            int next = get<0>(connection);
            int line = get<1>(connection);
            int new_changes =
                changes +
                (line != current_line);  // Incrementar se mudar de linha

            if (!visited[next].count(line) ||
                visited[next][line] > new_changes) {
                visited[next][line] = new_changes;
                q.emplace(next, line, new_changes);
            }
        }
    }

    return -1;  // Sem caminho entre as estações
}

int metro_connectivity(int n, const vector<vector<Connection>>& graph) {
    if (!is_connected(n, graph))
        return -1;  // Se a rede não for conexa, retornar -1

    int max_changes = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            int changes = min_line_changes(i, j, graph);
            if (changes == -1) return -1;  // Estações não conectadas
            max_changes = max(max_changes, changes);
        }
    }

    return max_changes;  // Retorna o índice de conectividade
}
