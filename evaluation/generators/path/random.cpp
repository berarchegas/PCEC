// Receives a graph, constant k and p, outputs k paths
// Paths are generated by selecting a random edge 
// And then expanding that edge into a path randomly
// Each time we expand there is a p% chance that we stop there
// We also stop if we cannot expand the path anymore

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int MAXN = 1e5 + 5;

vector<int> v[MAXN];
set<deque<int>> paths;

int main () { 
    int n, m, k, p;
    cin >> k >> p >> n >> m;
    vector<pii> edges(m);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        v[a].push_back(b);
        v[b].push_back(a);
        edges[i] = {a, b};
    }
    while (paths.size() < k) {
        int id = rng() % m;
        int start = edges[id].first, end = edges[id].second;
        deque<int> path = {start, end};
        set<int> inPath;
        inPath.insert(start);
        inPath.insert(end);
        while (true) {
            int prob = rng() % 101;
            if (prob > p || path.size() < 4) {
                vector<pii> candidates;
                for (int x : v[start]) {
                    if (!inPath.count(x)) candidates.push_back({x, 0});
                }
                for (int x : v[end]) {
                    if (!inPath.count(x)) candidates.push_back({x, 1});
                }
                if (candidates.empty()) break;
                id = rng() % (int)candidates.size();
                if (candidates[id].second == 0) {
                    path.push_front(candidates[id].first);
                    start = candidates[id].first;
                }
                else { 
                    path.push_back(candidates[id].first);
                    end = candidates[id].first;
                }
                inPath.insert(candidates[id].first);
            }
            else break;
        }
        paths.insert(path);
    }
    cout << n << ' ' << m << ' ' << k << '\n';
    for (int i = 0; i < n; i++) {
        for (int x : v[i]) if (x > i) cout << i + 1 << ' ' << x + 1 << '\n';
    }
    for (auto x : paths) {
        cout << x.size() << '\n';
        for (int y : x) cout << y + 1 << ' ';
        cout << '\n';
    }
    return 0;
}