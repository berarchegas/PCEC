#include<bits/stdc++.h>
#include"dancingLinks.h"

using namespace std;

const int MAXN = 1e5 + 5;
const int INF = 1e9;
using pii = pair<int, int>;

vector<vector<int>> vis, ed;
vector<pii> endpoint;
DancingLinks d = DancingLinks();

// passa por todos os vertices da componente e devolve a menor aresta
pii dfs(int node, int dep, stack<int> &reset) {
    // cout << "dfs " << node << "  ";
    reset.push(node);
    vis[dep][node] = 1;
    pii ans = {INF, 0};
    for (int aux = d.table[1][node].down; aux != node; aux = d.table[1][aux].down) {
        int edge = d.table[1][aux].option;
        // cout << edge << ' ';
    }
    // cout << '\n';
    for (int aux = d.table[1][node].down; aux != node; aux = d.table[1][aux].down) {
        int edge = d.table[1][aux].option;
        ans = min(ans, {d.table[0][edge].len, edge});
        if (!vis[dep][endpoint[edge].first]) {
            ans = min(ans, dfs(endpoint[edge].first, dep, reset));
        }
        if (!vis[dep][endpoint[edge].second]) {
            ans = min(ans, dfs(endpoint[edge].second, dep, reset));
        }
    }
    return ans;
}

int search(int dep, int rep) {

    // cout << "search " << dep << ' ' << rep << '\n';

    // se o representante nao tem mais nenhuma aresta
    if (d.table[1][rep].down == rep) {
        // cout << "Finish " << rep << '\n';
        return 0;
    }

    int ans = INF;
    stack<int> reset;
    int edge = dfs(rep, dep, reset).second;

    // cout << "edge = " << edge << endl;

    while (!reset.empty()) {
        // cout << "reset " << reset.top() << '\n';
        vis[dep][reset.top()] = 0;
        reset.pop();
    } 

    if (d.table[0][edge].down == edge) {
        // cout << "Deu ruim\n";
        return INF;
    }

	d.coverColumn(edge);

    for (int aux = d.table[0][edge].down; aux != edge; aux = d.table[0][aux].down) {
        int tenta = 1;
        // cout << "path = " << table[0][aux].option  << ' ' << dep << '\n';
        for (int node = aux + 1; node != aux;) {
            if (d.table[0][node].item <= 0) {
                node = d.table[0][node].up;
            }
            else {
                d.coverColumn(d.table[0][node].item);
                node++;
            }
        }
        int node = aux;
        do {
            if (d.table[0][node].item <= 0) {
                node = d.table[0][node].up;
            }
            else {
                int a = endpoint[d.table[0][node].item].first;
                int b = endpoint[d.table[0][node].item].second;
                if (!vis[dep][a] && tenta < INF) {
                    tenta = min(tenta + search(dep + 1, a), INF);
                    dfs(a, dep, reset);
                }
                if (!vis[dep][b] && tenta < INF) {
                    tenta = min(tenta + search(dep + 1, b), INF);
                    dfs(b, dep, reset);
                }
                node++;
            }
        } while (node != aux && tenta < INF);

        while (!reset.empty()) {
            // cout << "reset " << reset.top() << '\n';
            vis[dep][reset.top()] = 0;
            reset.pop();
        }

        ans = min(ans, tenta);

        for (int node = aux - 1; node != aux;) {
            if (d.table[0][node].item <= 0) {
                node = d.table[0][node].down;
            }
            else {
                d.uncoverColumn(d.table[0][node].item);
                node--;
            }
        }
    }

	d.uncoverColumn(edge);
    // cout << "return " << ans << '\n';
    return ans;
}

int main() {

    int n, m, p;
    cin >> n >> m >> p;

    ed = vector<vector<int>> (n + 1, vector<int> (n + 1));
    vis = vector<vector<int>> (n + 1, vector<int> (n + 1));
    endpoint = vector<pii> (m + 1);
    int a, b;
    for (int i = 1; i <= m; i++) {
        cin >> a >> b;
        if (a > b) swap(a, b);
        endpoint[i] = {a, b};
        ed[a][b] = ed[b][a] = i;
    }

    d = DancingLinks(n, m, p, ed, endpoint);

    cout << search(0, 1) << '\n';
}