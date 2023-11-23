#include<bits/stdc++.h>
#include"dancingLinks.h"

using namespace std;
using namespace chrono;
using pii = pair<int, int>;
using ll = long long;
const int MAXN = 1e5 + 5;
const int INF = 1e9;

mt19937_64 rng((ll) chrono::steady_clock::now().time_since_epoch().count());

vector<ll> hashes;
map<ll, int> mp;
vector<vector<int>> ed;
vector<int> vis;
vector<pii> endpoint;
DancingLinks d = DancingLinks();

// passa por todos os vertices da componente e devolve a menor aresta
pii findEdge(int node, stack<int> &reset) {
    reset.push(node);
    vis[node] = 1;
    pii ans = {INF, 0};
    for (int aux = d.table[1][node].down; aux != node; aux = d.table[1][aux].down) {
        int edge = d.table[1][aux].option;
        ans = min(ans, {d.table[0][edge].len, edge});
        if (!vis[endpoint[edge].first]) {
            ans = min(ans, findEdge(endpoint[edge].first, reset));
        }
        if (!vis[endpoint[edge].second]) {
            ans = min(ans, findEdge(endpoint[edge].second, reset));
        }
    }
    return ans;
}

void dfs(int node, stack<int> &reset, ll &newHash, int &newSize, double &efficiency, int &odd) {
    reset.push(node);
    vis[node] = 1;
    int qtd = 0;
    for (int aux = d.table[1][node].down; aux != node; aux = d.table[1][aux].down) {
        qtd++;
        int edge = d.table[1][aux].option;
        if (node == endpoint[edge].first) {
            newHash ^= hashes[edge];
            if (d.table[0][edge].down != edge) {
                efficiency += 1.0/d.table[0][d.table[0][edge].down].len;               
            }
            newSize++;
        }
        if (!vis[endpoint[edge].first]) {
            dfs(endpoint[edge].first, reset, newHash, newSize, efficiency, odd);
        }
        if (!vis[endpoint[edge].second]) {
            dfs(endpoint[edge].second, reset, newHash, newSize, efficiency, odd);
        }
    }
    odd += (qtd&1);
}

// calcula o LB e size da componente
// no momento soh calcula o efficiency bound e o edges bound
void processComponent(int node, stack<int> &reset, ll &newHash, int &newLB, int &newSize) {
    double efficiency = 0;
    int odd = 0;
    dfs(node, reset, newHash, newSize, efficiency, odd);
    // cout << efficiency << endl;
    // Im probably calculating the LB wrong
    newLB = max(odd / 2, (int)ceil(efficiency));
}

int search(int rep, int UB, ll hashValue) {

    // cout << "search " << dep << ' ' << rep << ' ' << UB << '\n';

    if (UB <= 1) {
        // cout << "Finish " << dep << ' ' << rep << ' ' << UB << '\n';
        return INF;
    }

    int ans = UB;

    stack<int> reset;
    int edge = findEdge(rep, reset).second;

    // cout << "edge = " << edge << endl;

    while (!reset.empty()) {
        // cout << "reset " << reset.top() << '\n';
        vis[reset.top()] = 0;
        reset.pop();
    } 

    if (d.table[0][edge].down == edge) {
        // cout << "Deu ruim " << dep << ' ' << rep << ' ' << UB << '\n';
        return mp[hashValue] = INF;
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

       vector<array<ll, 5>> components;
        int node = aux;
        do {
            if (d.table[0][node].item <= 0) {
                node = d.table[0][node].up;
            }
            else {
                int a = endpoint[d.table[0][node].item].first;
                int b = endpoint[d.table[0][node].item].second;
                if (!vis[a]) {
                    ll newHash = 0;
                    int newLB = 0, newSize = 0, goodHash;
                    processComponent(a, reset, newHash, newLB, newSize);
                    if (!newHash || mp[newHash]) {
                        newLB = mp[newHash];
                        goodHash = 1;
                    }
                    else {
                        goodHash = 0;
                    }
                    components.push_back({newHash, newLB, newSize, goodHash, a});
                }
                if (!vis[b]) {
                    ll newHash = 0;
                    int newLB = 0, newSize = 0, goodHash;
                    processComponent(b, reset, newHash, newLB, newSize);
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

        while (!reset.empty()) {
            // cout << "reset " << reset.top() << '\n';
            vis[reset.top()] = 0;
            reset.pop();
        }

        // coloca primeiro os caras ja resolvidos por hash e depois ordena por qtd de arestas
        sort(components.begin(), components.end(), [&] (array<ll, 5> a, array<ll, 5> b){
            if (a[3] != b[3]) return a[3] > b[3];
            return a[2] < b[2];
        });

        int LB = 1;
        for (int i = 0; i < (int)components.size() && LB < ans; i++) {
            LB = min(LB + components[i][1], (ll)ans);
        }
        // cout << LB << ' ' << (int)components.size() << '\n';
        for (int i = 0; i < (int)components.size() && LB < ans; i++) {
            if (!components[i][3]) {
                if (ans == INF) {
                    LB = min(LB + search(components[i][4], ans, components[i][0]) - components[i][1], (ll)ans);
                }
                else {
                    LB = min(LB + search(components[i][4], ans - LB + (int)components[i][1], components[i][0]) - components[i][1], (ll)ans);
                }
            }
        }

        ans = min(ans, LB);

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

    if (ans < UB || ans == INF) {
        mp[hashValue] = ans;
    }
    return ans;
}

struct timer : high_resolution_clock {
	const time_point start;
	timer(): start(now()) {}
	int operator()() {
		return duration_cast<milliseconds>(now() - start).count();
	}
};

int main() {

    int n, m, p;
    cin >> n >> m >> p;

    ed = vector<vector<int>> (n + 1, vector<int> (n + 1));
    vis = vector<int> (n + 1);
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

    timer T;
    
    // if we use some order to process the different components in the middle of search we can do the same thing here
    // we can also propagate upper and lower bounds here
    stack<int> reset;
    vector<array<ll, 5>> components;
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) {
            ll newHash = 0;
            int newLB = 0, newSize = 0, goodHash;
            processComponent(i, reset, newHash, newLB, newSize);
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
        vis[reset.top()] = 0;
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
            LB = min(LB + search(components[i][4], INF, components[i][0]), INF);
        }
    }
    cout << LB << ' ' << T() << '\n';
}