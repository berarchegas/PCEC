// Dynamic Programming solution with Bitmasks to compare with the Branch and Bound
// Works for m <= X
// O(p * 2 ^ m)

#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

const int X = 25;
const int INF = 1000000000;

map<pii, int> mp;
vector<int> masks;
int dp[(1 << X) + 5];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m, p, a, b, tam;
    cin >> n >> m >> p;
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        mp[{a, b}] = mp[{b, a}] = i;
    }
    for (int i = 0; i < p; i++) {
        cin >> tam >> a;
        int msk = 0;
        for (int j = 1; j < tam; j++) {
            cin >> b;
            msk += (1 << mp[{a, b}]);
            a = b;
        }
        masks.push_back(msk);
    }
    for (int i = 0; i < (1 << m); i++) {
        dp[i] = INF;
    }
    dp[0] = 0;
    for (int i = 0; i < (1 << m); i++) {
        if (dp[i] == p + 1) continue;
        for (int x : masks) {
            if ((i & x) == 0) {
                dp[i | x] = min(dp[i | x], dp[i] + 1);
            }
        }
    }
    cout << dp[(1 << m) - 1] << '\n';
    return 0;
}