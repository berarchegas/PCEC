#include<bits/stdc++.h>
#include"dancingLinks.h"

using namespace std;
using pii = pair<int, int>;
const int MAXN = 1e5 + 5;
const int INF = 1e9;

vector<vector<int>> vis, ed;
vector<pii> endpoint;
DancingLinks d = DancingLinks();

// passa por todos os vertices da componente e devolve a menor aresta
pii findEdge(int node, int dep, stack<int> &reset) {
    reset.push(node);
    vis[dep][node] = 1;
    pii ans = {INF, 0};
    for (int aux = d.table[1][node].down; aux != node; aux = d.table[1][aux].down) {
        int edge = d.table[1][aux].option;
        ans = min(ans, {d.table[0][edge].len, edge});
        if (!vis[dep][endpoint[edge].first]) {
            ans = min(ans, findEdge(endpoint[edge].first, dep, reset));
        }
        if (!vis[dep][endpoint[edge].second]) {
            ans = min(ans, findEdge(endpoint[edge].second, dep, reset));
        }
    }
    return ans;
}

void dfs(int node, int dep, stack<int> &reset, double &efficiency, int &newSize) {
    reset.push(node);
    vis[dep][node] = 1;
    for (int aux = d.table[1][node].down; aux != node; aux = d.table[1][aux].down) {
        int edge = d.table[1][aux].option;
        if (node == endpoint[edge].first) {
            if (d.table[0][edge].down != edge) {
                efficiency += 1.0/d.table[0][d.table[0][edge].down].len;
            }
            newSize++;
        }
        if (!vis[dep][endpoint[edge].first]) {
            dfs(endpoint[edge].first, dep, reset, efficiency, newSize);
        }
        if (!vis[dep][endpoint[edge].second]) {
            dfs(endpoint[edge].second, dep, reset, efficiency, newSize);
        }
    }
}

// calcula o LB e size da componente
// no momento soh calcula o efficiency bound
void processComponent(int node, int dep, stack<int> &reset, int &newLB, int &newSize) {
    double efficiency = 0;
    dfs(node, dep, reset, efficiency, newSize);
    // cout << efficiency << endl;
    newLB = ceil(efficiency);
}

int search(int dep, int rep, int UB) {

    // cout << "search " << dep << ' ' << rep << ' ' << UB << '\n';

    if (UB <= 1) {
        // cout << "Finish " << dep << ' ' << rep << ' ' << UB << '\n';
        return INF;
    }

    stack<int> reset;
    int edge = findEdge(rep, dep, reset).second;

    // cout << "edge = " << edge << endl;

    while (!reset.empty()) {
        // cout << "reset " << reset.top() << '\n';
        vis[dep][reset.top()] = 0;
        reset.pop();
    } 

    if (d.table[0][edge].down == edge) {
        // cout << "Deu ruim " << dep << ' ' << rep << ' ' << UB << '\n';
        return INF;
    }

	d.coverColumn(edge);

    for (int aux = d.table[0][edge].down; aux != edge; aux = d.table[0][aux].down) {
        // cout << "new path " << UB << ": ";
        for (int node = aux + 1; node != aux;) {
            if (d.table[0][node].item <= 0) {
                node = d.table[0][node].up;
            }
            else {
                d.coverColumn(d.table[0][node].item);
                node++;
            }
        }

        vector<array<int, 3>> components;
        int node = aux;
        do {
            if (d.table[0][node].item <= 0) {
                node = d.table[0][node].up;
            }
            else {
                int a = endpoint[d.table[0][node].item].first;
                int b = endpoint[d.table[0][node].item].second;
                if (!vis[dep][a]) {
                    int newLB = 0, newSize = 0;
                    processComponent(a, dep, reset, newLB, newSize);
                    if (newSize)
                        components.push_back({newLB, newSize, a});
                }
                if (!vis[dep][b]) {
                    int newLB = 0, newSize = 0;
                    processComponent(b, dep, reset, newLB, newSize);
                    if (newSize)
                        components.push_back({newLB, newSize, b});
                }
                node++;
            }
        } while (node != aux);

        // ordena por tamanho
        sort(components.begin(), components.end(), [&] (array<int, 3> a, array<int, 3> b){
            return a[1] < b[1];
        });

        int LB = 1;
        for (int i = 0; i < (int)components.size() && LB < UB; i++) {
            LB = min(LB + components[i][0], UB);
        }
        // cout << LB << ' ' << (int)components.size() << '\n';
        for (int i = 0; i < (int)components.size() && LB < UB; i++) {
            // if (UB == INF) {
            //     LB = min(LB + search(dep + 1, components[i][2], UB), UB);
            // }
            // else {
            //     LB = min(LB + search(dep + 1, components[i][2], UB - LB + components[i][0]) - components[i][0], UB);
            // }
            LB = min(LB + search(dep + 1, components[i][2], UB - LB + components[i][0]) - components[i][0], UB);
        }

        while (!reset.empty()) {
            // cout << "reset " << reset.top() << '\n';
            vis[dep][reset.top()] = 0;
            reset.pop();
        }

        UB = min(UB, LB);

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
    // cout << "return " << dep << ' ' << rep << ' ' << UB << '\n';
    return UB;
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
    
    // if we use some order to process the different components in the middle of search we can do the same thing here
    // we can also propagate upper and lower bounds here
    int UB = INF;
    stack<int> reset;
    vector<array<int, 3>> components;
    for (int i = 1; i <= n; i++) {
        if (!vis[0][i]) {
            int newLB = 0, newSize = 0;
            processComponent(i, 0, reset, newLB, newSize);
            components.push_back({newLB, newSize, i});
        }
    }
    while (!reset.empty()) {
        vis[0][reset.top()] = 0;
        reset.pop();
    }

    // ordena por tamanho
    sort(components.begin(), components.end(), [&] (array<int, 3> a, array<int, 3> b){
        return a[1] < b[1];
    });

    int LB = 0;
    for (int i = 0; i < (int)components.size() && LB < INF; i++) {
        if (components[i][1]) {
            // cout << search(0, components[i][2], INF) << '\n';
            LB += search(0, components[i][2], INF);
        }
    }
    cout << LB << '\n';
}