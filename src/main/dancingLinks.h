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
        DancingLinks(int n, int m, int p);

        void hideEdge(bool t, int p);

        void unhideEdge(bool t, int p);

        void hideLine(bool t, int p);

        void unhideLine(bool t, int p);

        void coverColumn(int i);

        void uncoverColumn(int i);

    private:
        vector<vector<Node>> table;
        vector<int> pos;
        vector<vector<int>> options, tail;
        int items;
        
};

#endif