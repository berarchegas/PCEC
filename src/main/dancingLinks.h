#ifndef DANCINGLINKS_H
#define DANCINGLINKS_H
#include <bits/stdc++.h>
using namespace std;

// Define the node structure for the Dancing Links
struct Node {

	int left, right, up, down, item, len, option;

};

// Define the ordered subset list class
class DancingLinks {
    public:

        DancingLinks();
        
        DancingLinks(int n, int m, int p, vector<vector<int>> &ed, vector<pair<int, int>> &endpoint);

        void hideEdge(bool t, int p);

        void unhideEdge(bool t, int p);

        void hideLine(bool t, int p);

        void unhideLine(bool t, int p);

        void coverColumn(int i);

        void uncoverColumn(int i);

        void markEdge(int i);

        void unmarkEdge(int i);

        vector<vector<Node>> table;

        vector<int> conflict;

    private:
        vector<int> pos;
        vector<vector<int>> options, tail;
        int items;
        
};

#endif