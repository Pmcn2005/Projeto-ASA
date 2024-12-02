#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct bolinha {
    int result;
    int divisonK;
    int leftResult;
    int rightResult;
};

// Função para calcular o resultado de i ⊕ j usando a tabela de operações
int operate(const vector<vector<int>> &table, int i, int j) {
    return table[i - 1][j - 1];
}

string getExpression(vector<vector<vector<bolinha>>> &dp,
                     vector<vector<unordered_map<int, int>>> &resTable,
                     bolinha &bolinha, int start, int end) {
    if (bolinha.divisonK == -1) {
        return to_string(bolinha.result);
    }

    int k = bolinha.divisonK;
    // îr ao unordered_map e pegar o índice do resultado
    int indexLeft = resTable[start][k - 1][bolinha.leftResult];
    int indexRight = resTable[k][end][bolinha.rightResult];

    string leftExpr =
        getExpression(dp, resTable, dp[start][k - 1][indexLeft], start, k - 1);
    string rightExpr =
        getExpression(dp, resTable, dp[k][end][indexRight], k, end);

    return "(" + leftExpr + " " + rightExpr + ")";
}

// Função para resolver o problema usando programação dinâmica
string solve(const vector<int> &sequence, const vector<vector<int>> &table,
             int desiredResult, int n) {
    int m = sequence.size();

    // dp[i][j] armazena todas as expressões possíveis para o intervalo [i, j]
    vector<vector<vector<bolinha>>> dp(m, vector<vector<bolinha>>(m));

    // resTable[i][j] resultado -- Posição vetor de bolinhas
    vector<vector<unordered_map<int, int>>> resTable(
        m, vector<unordered_map<int, int>>(m));

    // Inicializar casos base (intervalos de tamanho 1)
    for (int i = 0; i < m; ++i) {
        dp[i][i].push_back({sequence[i], -1, -1, -1});
        resTable[i][i].insert({sequence[i], 0});
    }

    // Preencher a tabela dp para intervalos de tamanho crescente
    for (int len = 2; len <= m; ++len) {  // len é o tamanho do intervalo

        for (int start = 0; start <= m - len; ++start) {
            int end = start + len - 1;  // Determina o final do intervalo

            // Combinar resultados dos subintervalos [start, k] e [k+1, end]
            for (int k = end; k > start; --k) {
                if (dp[start][end].size() >= static_cast<size_t>(n)) {
                    break;
                }

                for (bolinha &bolinhaLeft : dp[start][k - 1]) {
                    int index = 0;

                    for (bolinha &bolinhaRight : dp[k][end]) {
                        int result = operate(table, bolinhaLeft.result,
                                             bolinhaRight.result);

                        // if the result is not already in the vector of int's
                        if (resTable[start][end].find(result) ==
                            resTable[start][end].end()) {
                            dp[start][end].push_back({result, k,
                                                      bolinhaLeft.result,
                                                      bolinhaRight.result});
                            resTable[start][end].insert({result, index});
                        }
                        index++;
                    }
                }
            }
        }
    }

    // // print dp table
    // for (int i = 0; i < m; ++i) {
    //     for (int j = 0; j < m; ++j) {
    //         cout << "dp[" << i << "][" << j << "]: ";
    //         for (const auto &res : dp[i][j]) {
    //             cout << res.result << " ";
    //         }
    //         cout << endl;
    //     }
    // }

    // // print resTable
    // for (int i = 0; i < m; ++i) {
    //     for (int j = 0; j < m; ++j) {
    //         cout << "resTable[" << i << "][" << j << "]: ";
    //         for (const auto &res : resTable[i][j]) {
    //             cout << res.first << " " << res.second << " ";
    //         }
    //         cout << endl;
    //     }
    // }

    if (resTable[0][m - 1].find(desiredResult) != resTable[0][m - 1].end()) {
        int index = resTable[0][m - 1][desiredResult];
        return getExpression(dp, resTable, dp[0][m - 1][index], 0, m - 1);
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
