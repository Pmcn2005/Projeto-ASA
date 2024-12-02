#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Função para calcular o resultado de i ⊕ j usando a tabela de operações
int operate(const vector<vector<int>> &table, int i, int j) {
    return table[i - 1][j - 1];
}

// Função recursiva para gerar todas as parentizações priorizando as mais à
// esquerda
void generateParenthesizations(
    const vector<int> &sequence, const vector<vector<int>> &table, int start,
    int end, unordered_map<string, vector<pair<string, int>>> &memo,
    vector<pair<string, int>> &results) {
    // Verificar se o intervalo já foi calculado
    string key = to_string(start) + "," + to_string(end);
    if (memo.find(key) != memo.end()) {
        results = memo[key];
        return;
    }

    // Caso base: um único número
    if (start == end) {
        results.push_back({to_string(sequence[start]), sequence[start]});
        memo[key] = results;
        return;
    }

    // Gerar combinações dividindo em subproblemas, priorizando as mais à
    // esquerda
    for (int k = start + 1; k <= end; ++k) {
        vector<pair<string, int>> leftResults, rightResults;

        // Gerar resultados para as partes esquerda e direita
        generateParenthesizations(sequence, table, start, k - 1, memo,
                                  leftResults);
        generateParenthesizations(sequence, table, k, end, memo, rightResults);

        // Combinar os resultados das partes esquerda e direita
        for (const auto &left : leftResults) {
            for (const auto &right : rightResults) {
                int result = operate(table, left.second, right.second);
                string expr = "(" + left.first + " " + right.first + ")";
                results.push_back({expr, result});
            }
        }
    }

    // Guardar no memo para evitar recomputações
    memo[key] = results;
}

// Função principal para resolver o problema
vector<string> solve(const vector<int> &sequence,
                     const vector<vector<int>> &table, int desiredResult) {
    int n = sequence.size();
    unordered_map<string, vector<pair<string, int>>> memo;
    vector<pair<string, int>> allResults;

    // Gerar todas as combinações possíveis de parentização
    generateParenthesizations(sequence, table, 0, n - 1, memo, allResults);

    // Filtrar as que resultam no valor desejado
    vector<string> validParenthesizations;
    for (const auto &res : allResults) {
        if (res.second == desiredResult) {
            validParenthesizations.push_back(res.first);
        }
    }

    return validParenthesizations;
}

int main() {
    // Ler a entrada
    int n, m;
    cin >> n >> m;

    vector<vector<int>> table(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) cin >> table[i][j];

    vector<int> sequence(m);
    for (int i = 0; i < m; ++i) cin >> sequence[i];

    int desiredResult;
    cin >> desiredResult;

    // Resolver o problema
    vector<string> solutions = solve(sequence, table, desiredResult);

    // Imprimir resultados
    if (solutions.empty()) {
        cout << "0" << endl;
    } else {
        cout << "1" << endl;
        int n = solutions.size();
        cout << solutions[n - 1] << endl;
        // for (int i = 0; i < n; ++i) cout << solutions[i] << endl;
    }

    return 0;
}
