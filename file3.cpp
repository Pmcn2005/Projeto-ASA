#include <algorithm>
#include <iostream>
#include <string>
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
    vector<vector<pair<vector<int>, vector<string>>>> dp(
        m, vector<pair<vector<int>, vector<string>>>(m));

    // Inicializar casos base (intervalos de tamanho 1)
    for (int i = 0; i < m; ++i) {
        // dp[i][i].push_back({to_string(sequence[i]), sequence[i]});
        dp[i][i].first.push_back(sequence[i]);
        dp[i][i].second.push_back(to_string(sequence[i]));
    }

    // Preencher a tabela dp para intervalos de tamanho crescente
    for (int len = 2; len <= m; ++len) {  // len é o tamanho do intervalo

        for (int start = 0; start <= m - len; ++start) {
            int end = start + len - 1;  // Determina o final do intervalo

            // Combinar resultados dos subintervalos [start, k] e [k+1, end]
            for (int k = end; k > start; --k) {
                int indexLeft = 0;

                if (dp[start][end].first.size() >= static_cast<size_t>(n)) {
                    break;
                }

                for (int &left : dp[start][k - 1].first) {
                    int indexRight = 0;

                    for (int &right : dp[k][end].first) {
                        int result = operate(table, left, right);

                        // if the result is not already in the vector of int's
                        if (find(dp[start][end].first.begin(),
                                 dp[start][end].first.end(),
                                 result) == dp[start][end].first.end()) {
                            dp[start][end].first.push_back(result);

                            string leftString =
                                dp[start][k - 1].second[indexLeft];

                            string rightString = dp[k][end].second[indexRight];

                            string expr =
                                "(" + leftString + " " + rightString + ")";
                            dp[start][end].second.push_back(expr);
                        }
                        indexRight++;
                    }
                    indexLeft++;
                }
            }
        }
    }

    // print the dp table
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << "dp[" << i << "][" << j << "]: ";
            for (int k = 0; k < dp[i][j].first.size(); k++) {
                cout << dp[i][j].first[k] << " ";
            }
            cout << endl;
        }
    }

    int index = 0;
    for (auto &res : dp[0][m - 1].first) {
        if (res == desiredResult) {
            return dp[0][m - 1].second[index];
        }
        index++;
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
        // cout << "1\n" << solution << endl;
        printf("1\n");
        printf("%s\n", solution.c_str());
    }

    return 0;
}
