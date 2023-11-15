#include<bits/stdc++.h>
#include"dancingLinks.h"

using namespace std;
using pii = pair<int, int>;
using ll = long long;
const int MAXN = 1e5 + 5;
const int INF = 1e9;

mt19937_64 rng((ll) chrono::steady_clock::now().time_since_epoch().count());

vector<ll> hashes;
map<ll, int> mp;
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

void dfs(int node, int dep, stack<int> &reset, ll &newHash, double &efficiency, ll &newSize) {
    reset.push(node);
    vis[dep][node] = 1;
    for (int aux = d.table[1][node].down; aux != node; aux = d.table[1][aux].down) {
        int edge = d.table[1][aux].option;
        if (node == endpoint[edge].first) {
            newHash ^= hashes[edge];
            if (d.table[0][edge].down != edge) {
                efficiency += 1.0/d.table[0][d.table[0][edge].down].len;
            }
            newSize++;
        }
        if (!vis[dep][endpoint[edge].first]) {
            dfs(endpoint[edge].first, dep, reset, newHash, efficiency, newSize);
        }
        if (!vis[dep][endpoint[edge].second]) {
            dfs(endpoint[edge].second, dep, reset, newHash, efficiency, newSize);
        }
    }
}

// calcula o hash, LB e size da componente
// no momento soh calcula o efficiency bound
void processComponent(int node, int dep, stack<int> &reset, ll &newHash, ll &newLB, ll &newSize) {
    double efficiency = 0;
    dfs(node, dep, reset, newHash, efficiency, newSize);
    // cout << efficiency << endl;
    newLB = ceil(efficiency);
}

int search(int dep, int rep, ll hashValue, int UB) {

    // cout << "search " << dep << ' ' << rep << ' ' << UB << '\n';

    if (UB <= 1) {
        // cout << "Finish " << rep << '\n';
        return UB;
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
        return mp[hashValue] = INF;
    }

	d.coverColumn(edge);

    for (int aux = d.table[0][edge].down; aux != edge; aux = d.table[0][aux].down) {
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

        vector<array<ll, 5>> components;
        int node = aux;
        do {
            if (d.table[0][node].item <= 0) {
                node = d.table[0][node].up;
            }
            else {
                int a = endpoint[d.table[0][node].item].first;
                int b = endpoint[d.table[0][node].item].second;
                if (!vis[dep][a]) {
                    ll newHash = 0, newLB = 0, newSize = 0, goodHash;
                    processComponent(a, dep, reset, newHash, newLB, newSize);
                    if (!newHash || mp[newHash]) {
                        newLB = mp[newHash];
                        goodHash = 1;
                    }
                    else {
                        goodHash = 0;
                    }
                    components.push_back({newHash, newLB, newSize, goodHash, a});
                }
                if (!vis[dep][b]) {
                    ll newHash = 0, newLB = 0, newSize = 0, goodHash;
                    processComponent(b, dep, reset, newHash, newLB, newSize);
                    if (!newHash || mp[newHash]) {
                        newLB = mp[newHash];
                        goodHash = 1;
                    }
                    else {
                        goodHash = 0;
                    }
                    components.push_back({newHash, newLB, newSize, goodHash, b});
                }
                node++;
            }
        } while (node != aux);

        // coloca primeiro os caras ja resolvidos por hash e depois ordena por qtd de arestas
        sort(components.begin(), components.end(), [&] (array<ll, 5> a, array<ll, 5> b){
            if (a[3] != b[3]) return a[3] > b[3];
            return a[2] < b[2];
        });

        int LB = 1;
        for (int i = 0; i < (int)components.size() && LB < UB; i++) {
            LB = min(LB + components[i][1], (ll)INF);
        }
        for (int i = 0; i < (int)components.size() && LB < UB; i++) {
            if (!components[i][3]) {
                if (UB == INF) {
                    LB = min(LB + search(dep + 1, components[i][4], components[i][0], UB), UB);
                }
                else {
                    LB = min(LB + search(dep + 1, components[i][4], components[i][0], UB - LB + components[i][1]) - components[i][1], (ll)UB);
                }
            }
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
    // cout << "return " << UB << '\n';
    return mp[hashValue] = UB;
}

int main() {

    int n, m, p;
    cin >> n >> m >> p;

    ed = vector<vector<int>> (n + 1, vector<int> (n + 1));
    vis = vector<vector<int>> (n + 1, vector<int> (n + 1));
    endpoint = vector<pii> (m + 1);
    hashes = vector<ll> (m + 1);
    int a, b;
    for (int i = 1; i <= m; i++) {
        cin >> a >> b;
        if (a > b) swap(a, b);
        endpoint[i] = {a, b};
        ed[a][b] = ed[b][a] = i;
        hashes[i] = rng();
    }

    d = DancingLinks(n, m, p, ed, endpoint);
    mp[0] = 0;
    
    // if we use some order to process the different components in the middle of search we can do the same thing here
    // we can also propagate upper and lower bounds here
    int UB = INF;
    stack<int> reset;
    vector<array<ll, 5>> components;
    for (int i = 1; i <= n; i++) {
        if (!vis[0][i]) {
            ll newHash = 0, newLB = 0, newSize = 0, goodHash;
            processComponent(i, 0, reset, newHash, newLB, newSize);
            if (!newHash) {
                newLB = 0;
                goodHash = 1;
            }
            else {
                goodHash = 0;
            }
            components.push_back({newHash, newLB, newSize, goodHash, i});
        }
    }
    while (!reset.empty()) {
        vis[0][reset.top()] = 0;
        reset.pop();
    }

    // coloca primeiro os caras ja resolvidos por hash e depois ordena por qtd de arestas
    sort(components.begin(), components.end(), [&] (array<ll, 5> a, array<ll, 5> b){
        if (a[3] != b[3]) return a[3] > b[3];
        return a[2] < b[2];
    });

    int LB = 0;
    for (int i = 0; i < (int)components.size() && LB < INF; i++) {
        if (!components[i][3]) {
            cout << search(0, components[i][4], components[i][0], INF) << endl;
            LB += search(0, components[i][4], components[i][0], INF);
        }
    }
    cout << LB << '\n';
}