#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <vector>

using namespace std;

// Função para calcular o índice de conectividade usando BFS
int calcula_conectividade(const map<int, set<int>>& grafo_linhas) {
    int max_trocas = 0;
    for (auto it = grafo_linhas.begin(); it != grafo_linhas.end(); ++it) {
        int linha_inicial = it->first;

        map<int, int> dist;
        for (auto jt = grafo_linhas.begin(); jt != grafo_linhas.end(); ++jt) {
            dist[jt->first] = numeric_limits<int>::max();
        }

        queue<int> q;
        dist[linha_inicial] = 0;
        q.push(linha_inicial);

        while (!q.empty()) {
            int linha_atual = q.front();
            q.pop();

            for (int vizinho : grafo_linhas.at(linha_atual)) {
                if (dist[vizinho] == numeric_limits<int>::max()) {
                    dist[vizinho] = dist[linha_atual] + 1;
                    q.push(vizinho);
                }
            }
        }

        for (auto jt = dist.begin(); jt != dist.end(); ++jt) {
            if (jt->second == numeric_limits<int>::max()) {
                return -1;  // Há linhas desconectadas
            }
            max_trocas = max(max_trocas, jt->second);
        }
    }
    return max_trocas;
}

int main() {
    int n, m, l;
    cin >> n >> m >> l;

    map<int, set<int>> estacoes;      // Estações mapeadas para linhas
    map<int, set<int>> grafo_linhas;  // Grafo das linhas

    vector<int> estacoes_lidas(
        n, 0);  // Contador de estações lidas, usado para
                // verificar se existem estações desconectadas

    // Ler conexões
    for (int i = 0; i < m; i++) {
        int x, y, linha;
        cin >> x >> y >> linha;

        estacoes[x].insert(linha);
        estacoes[y].insert(linha);
        estacoes_lidas[x - 1]++;
        estacoes_lidas[y - 1]++;
    }

    // verificar se não existe estação desconectada
    for (int i = 0; i < n; i++) {
        if (estacoes_lidas[i] == 0) {
            cout << -1 << endl;
            return 0;
        }
    }

    // verificar se uma linha tem todas as estações
    for (auto it = estacoes.begin(); it != estacoes.end(); ++it) {
        if (it->second.size() == (size_t)l) {
            cout << 0 << endl;
            return 0;
        }
    }

    // Construir grafo de linhas
    for (auto it = estacoes.begin(); it != estacoes.end(); ++it) {
        const set<int>& linhas = it->second;

        for (auto linha1 = linhas.begin(); linha1 != linhas.end(); ++linha1) {
            for (auto linha2 = linhas.begin(); linha2 != linhas.end();
                 ++linha2) {
                if (*linha1 != *linha2) {
                    grafo_linhas[*linha1].insert(*linha2);
                    grafo_linhas[*linha2].insert(*linha1);
                }
            }
        }
    }

    // Calcular índice de conectividade
    int resultado = calcula_conectividade(grafo_linhas);
    cout << resultado << endl;

    return 0;
}
