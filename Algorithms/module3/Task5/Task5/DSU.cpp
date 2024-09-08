#include "DSU.h"

DSU::DSU(int count) :
    parent(count),
    rank(count)
{
    for (int i = 0; i < count; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}

int DSU::FindParent(int v) {
    if (v == parent[v]) {
        return v;
    }
    parent[v] = FindParent(parent[v]);
    return parent[v];
}

void DSU::Union(int v1, int v2) {
    int p1 = FindParent(v1);
    int p2 = FindParent(v2);
    if (p1 == p2) {
        return;
    }

    if (rank[p1] < rank[p2]) {
        parent[p1] = p2;
    }
    else {
        parent[p2] = p1;
        if (rank[p1] == rank[p2]) {
            ++rank[p1];
        }
    }
}
