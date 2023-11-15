#include<bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
const int INF = 1e9;
using pii = pair<int, int>;

// we can optimize here
// only the head nodes need the fields left, right and len
struct Node {
	int left, right, up, down, item, len, option;
} table[2][MAXN];

int pos[MAXN];

void hideEdge(bool t, int p) {
    // hide edge p

    int q = p;
    do {
        if (table[t][q].item <= 0) {
            q = table[t][q].up;
        }
        else {
            table[t][table[t][q].up].down = table[t][q].down;
            table[t][table[t][q].down].up = table[t][q].up;
            table[t][table[t][q].item].len--;
            q++;
        }
    } while (q != p);
}

void unhideEdge(bool t, int p) {
    // unhide edge p

    int q = p;
    do {
        if (table[t][q].item <= 0) {
            q = table[t][q].down;
        }
        else {
            table[t][table[t][q].up].down = q;
            table[t][table[t][q].down].up = q;
            table[t][table[t][q].item].len++;
            q--;
        }
    } while (q != p);
}

void hideLine(bool t, int p) {
    // hide line p

    int q = p + 1;
    while (q != p) {
        if (table[t][q].item <= 0) {
            q = table[t][q].up;
        }
        else {
            table[t][table[t][q].up].down = table[t][q].down;
            table[t][table[t][q].down].up = table[t][q].up;
            table[t][table[t][q].item].len--;
            q++;
        }
    }
}

void unhideLine(bool t, int p) {
    // unhide line p

    int q = p - 1;
    while (q != p) {
        if (table[t][q].item <= 0) {
            q = table[t][q].down;
        }
        else {
            table[t][table[t][q].up].down = q;
            table[t][table[t][q].down].up = q;
            table[t][table[t][q].item].len++;
            q--;
        }
    }
}

void coverColumn(int i) {
    // cover column i

    hideEdge(1, pos[i]);

    int p = table[0][i].down;
    while (p != i) {
        hideLine(0, p);
        p = table[0][p].down;
    }
    table[0][table[0][i].left].right = table[0][i].right;
    table[0][table[0][i].right].left = table[0][i].left;
}

void uncoverColumn(int i) {
    // uncover column i

    table[0][table[0][i].left].right = i;
    table[0][table[0][i].right].left = i;
    int p = table[0][i].up;
    while (p != i) {
        unhideLine(0, p);
        p = table[0][p].up;
    }

    unhideEdge(1, pos[i]);
}

vector<vector<int>> options;
int tail[2][MAXN], vis[5000][5000], ed[500][500], items;
pii endpoint[MAXN];

// passa por todos os vertices da componente e devolve a menor aresta
pii dfs(int node, int dep, stack<int> &reset) {
    // cout << "dfs " << node << "  ";
    reset.push(node);
    vis[dep][node] = 1;
    pii ans = {INF, 0};
    for (int aux = table[1][node].down; aux != node; aux = table[1][aux].down) {
        int edge = table[1][aux].option;
        // cout << edge << ' ';
    }
    // cout << '\n';
    for (int aux = table[1][node].down; aux != node; aux = table[1][aux].down) {
        int edge = table[1][aux].option;
        ans = min(ans, {table[0][edge].len, edge});
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
    if (table[1][rep].down == rep) {
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

    if (table[0][edge].down == edge) {
        // cout << "Deu ruim\n";
        return INF;
    }

	coverColumn(edge);

    for (int aux = table[0][edge].down; aux != edge; aux = table[0][aux].down) {
        int tenta = 1;
        // cout << "path = " << table[0][aux].option  << ' ' << dep << '\n';
        for (int node = aux + 1; node != aux;) {
            if (table[0][node].item <= 0) {
                node = table[0][node].up;
            }
            else {
                coverColumn(table[0][node].item);
                node++;
            }
        }
        int node = aux;
        do {
            if (table[0][node].item <= 0) {
                node = table[0][node].up;
            }
            else {
                int a = endpoint[table[0][node].item].first;
                int b = endpoint[table[0][node].item].second;
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
            if (table[0][node].item <= 0) {
                node = table[0][node].down;
            }
            else {
                uncoverColumn(table[0][node].item);
                node--;
            }
        }
    }

	uncoverColumn(edge);
    // cout << "return " << ans << '\n';
    return ans;
}

int main() {

    int n, m, p;
    cin >> n >> m >> p;

    int a, b;
    for (int i = 1; i <= m; i++) {
        cin >> a >> b;
        if (a > b) swap(a, b);
        endpoint[i] = {a, b};
        ed[a][b] = ed[b][a] = i;
    }
    d = DancingLinks(n, m, p);
    items = m;
    for (int i = 0; i < p; i++) {
        int tam;
        cin >> tam;
        vector<int> option(tam - 1);
        int ini, fim; 
        cin >> ini;
        for (int j = 0; j < tam - 1; j++) {
            cin >> fim;
            option[j] = ed[ini][fim];
            ini = fim;
        }
        sort(option.begin(), option.end());
        options.push_back(option);
    }

}