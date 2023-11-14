#include <bits/stdc++.h>
#include "dancingLinks.h"
using namespace std;

// Constructor for the DancingLinks class
DancingLinks::DancingLinks(int n, int m, int p, vector<vector<int>> &ed) {

    items = m;
    // calc number of nodes needed in table
    pos = vector<int> (m + 5);
    options = vector<vector<int>> ();
    tail = vector<vector<int>> (2, vector<int> (m + 5));

    int tam = m + 2;
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