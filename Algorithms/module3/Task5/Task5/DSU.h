#ifndef DSU_H
#define DSU_H

#include <vector>

class DSU {
public:
    DSU(int count);
    int FindParent(int v);
    void Union(int v1, int v2);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

#endif // DSU_H
