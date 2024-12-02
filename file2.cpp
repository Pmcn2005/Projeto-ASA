#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Função para calcular o resultado de i ⊕ j usando a tabela de operações
int operate(const vector<vector<int>> &table, int i, int j) {
    return table[i - 1][j - 1];
}

// Função para resolver o problema usando programação dinâmica
string solve(const vector<int> &sequence, const vector<vector<int>> &table,
             int desiredResult, int n) {
    int m = sequence.size();

    // dp[i][j] armazena todas as expressões possíveis para o intervalo [i, j]
    vector<vector<vector<pair<string, int>>>> dp(
        m, vector<vector<pair<string, int>>>(m));

    // Inicializar casos base (intervalos de tamanho 1)
    for (int i = 0; i < m; ++i) {
        dp[i][i].push_back({to_string(sequence[i]), sequence[i]});
    }

    // Preencher a tabela dp para intervalos de tamanho crescente
    for (int len = 2; len <= m; ++len) {  // len é o tamanho do intervalo
        for (int start = 0; start <= m - len; ++start) {
            int end = start + len - 1;  // Determina o final do intervalo

            // Combinar resultados dos subintervalos [start, k] e [k+1, end]
            for (int k = end; k > start; --k) {
                for (auto &left : dp[start][k - 1]) {
                    for (auto &right : dp[k][end]) {
                        int result = operate(table, left.second, right.second);
                        string expr =
                            "(" + left.first + " " + right.first + ")";
                        dp[start][end].push_back({expr, result});
                    }
                }
            }
        }
    }

    // print the dp table
    // for (int i = 0; i < m; ++i) {
    //     for (int j = 0; j < m; ++j) {
    //         cout << "dp[" << i << "][" << j << "]: ";
    //         for (const auto &res : dp[i][j]) {
    //             cout << res.first << " ";
    //         }
    //         cout << endl;
    //     }
    // }

    for (auto &res : dp[0][m - 1]) {
        if (res.second == desiredResult) {
            return res.first;
        }
    }

    return "";
}

int main() {
    // Ler a entrada
    int n, m;
    scanf("%d %d", &n, &m);

    vector<vector<int>> table(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) scanf("%d", &table[i][j]);

    vector<int> sequence(m);
    for (int i = 0; i < m; ++i) scanf("%d", &sequence[i]);

    int desiredResult;
    scanf("%d", &desiredResult);

    // Resolver o problema
    string solution = solve(sequence, table, desiredResult, n);

    // Imprimir resultados
    if (solution.empty()) {
        printf("0\n");
    } else {
        cout << "1\n" << solution << endl;
    }

    return 0;
}
