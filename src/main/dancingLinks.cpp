#include <bits/stdc++.h>
#include "dancingLinks.h"
using namespace std;

// Dummy constructor
DancingLinks::DancingLinks() {}

// Constructor for the DancingLinks class
DancingLinks::DancingLinks(int n, int m, int p, vector<vector<int>> &ed, vector<pair<int, int>> &endpoint) {

    items = m;
    pos = vector<int> (m + 5);
    options = vector<vector<int>> ();
    tail = vector<vector<int>> (2, vector<int> (m + 5));

    int qtd = m + 5;
    for (int i = 0; i < p; i++) {
        int tam;
        cin >> tam;
        vector<int> option(tam - 1);
        int ini, fim; 
        cin >> ini;
        qtd++;
        for (int j = 0; j < tam - 1; j++) {
            cin >> fim;
            option[j] = ed[ini][fim];
            ini = fim;
            qtd++;
        }
        sort(option.begin(), option.end());
        options.push_back(option);
    }
    sort(options.begin(), options.end(), [&] (vector<int> a, vector<int> b) {
        return a.size() > b.size();
    });
    qtd = max(qtd, 6 * m);
    table = vector<vector<Node>> (2, vector<Node> (qtd));

    // build table[0] which is the table with edges and paths
    for (int i = 0; i <= items; i++) {
        table[0][i].down = table[0][i].up = table[0][i].item = i;
        table[0][i].len = 0;
        table[0][i].left = (i + items) % (items + 1);
        table[0][i].right = (i + 1) % (items + 1);
        tail[0][i] = i;
    }
    int at = items + 1, last = 0;
    for (int i = 0; i < (int)options.size(); i++) {
        // add a spacer node
        table[0][at].item = -i;
        table[0][at].up = last;
        table[0][at].down = at + options[i].size();
        last = at + 1;
        at++;
        for (int j = 0; j < (int)options[i].size(); j++) {
            table[0][at].item = options[i][j];
            table[0][options[i][j]].len++;
            table[0][at].down = options[i][j];
            table[0][at].up = tail[0][options[i][j]];
            table[0][options[i][j]].up = at;
            table[0][tail[0][options[i][j]]].down = at;
            tail[0][options[i][j]] = at;
            table[0][at].option = i;
            table[0][at].len = (int)options[i].size();
            at++;
        }
    }
    // last spacer node
    table[0][at].item = - (int)options.size();
    table[0][at].up = last;
    table[0][at].down = 0;

    // build table[1] which is the table with nodes and edges
    items = n;
    for (int i = 0; i <= items; i++) {
        table[1][i].down = table[1][i].up = table[1][i].item = i;
        table[1][i].len = 2;
        table[1][i].left = (i + items) % (items + 1);
        table[1][i].right = (i + 1) % (items + 1);
        tail[1][i] = i;
    }
    at = items + 1, last = 0;
    for (int i = 1; i <= m; i++) {
        // add a spacer node
        table[1][at].item = -i + 1;
        table[1][at].up = last;
        table[1][at].down = at + 2;
        last = at + 1;
        at++;

        pos[i] = at;
        table[1][at].item = endpoint[i].first;
        table[1][at].down = endpoint[i].first;
        table[1][at].up = tail[1][endpoint[i].first];
        table[1][endpoint[i].first].up = at;
        table[1][tail[1][endpoint[i].first]].down = at;
        tail[1][endpoint[i].first] = at;
        table[1][at].option = i;
        at++;

        table[1][at].item = endpoint[i].second;
        table[1][at].down = endpoint[i].second;
        table[1][at].up = tail[1][endpoint[i].second];
        table[1][endpoint[i].second].up = at;
        table[1][tail[1][endpoint[i].second]].down = at;
        tail[1][endpoint[i].second] = at;
        table[1][at].option = i;
        at++;
    }
    // last spacer node
    table[1][at].item = -m;
    table[1][at].up = last;
    table[1][at].down = 0;
}

void DancingLinks::hideEdge(bool t, int p) {
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

void DancingLinks::unhideEdge(bool t, int p) {
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

void DancingLinks::hideLine(bool t, int p) {
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

void DancingLinks::unhideLine(bool t, int p) {
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

void DancingLinks::coverColumn(int i) {
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

void DancingLinks::uncoverColumn(int i) {
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