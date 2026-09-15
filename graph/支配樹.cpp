/*
给定一张有向图，求从 1 号点出发，每个点能支配的点的个数（包括自己）。
n,m，表示点数和边数  u,v，表示一条 u 到 v 的有向边
輸出每个点能支配的点的个数
*/
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

const int N = 200010;

int n, m;

struct DomTree {
    vector<int> g[N], tree[N], rg[N], bucket[N];
    int sdom[N], fa[N], dom[N], idom[N], lable[N];
    int arr[N], rev[N], T = 0;
    int sz[N]; // 儲存支配樹上的子樹大小

    void add(int u, int v) {
        g[u].push_back(v);
    }

    void init(int n) {
        for (int i = 0; i <= n; i++) {
            tree[i].clear(); g[i].clear(); rg[i].clear(); bucket[i].clear();
            sdom[i] = fa[i] = dom[i] = idom[i] = lable[i] = 0;
            arr[i] = rev[i] = sz[i] = 0;
        }
        T = 0;
    }

    int Find(int u, int x = 0) {
        if (u == idom[u]) return x ? -1 : u;
        int v = Find(idom[u], x + 1);
        if (v < 0) return u;
        if (sdom[lable[idom[u]]] < sdom[lable[u]])
            lable[u] = lable[idom[u]];
        idom[u] = v;
        return x ? v : lable[u];
    }

    void Union(int u, int v) {
        idom[v] = u;
    }

    void dfs0(int u) {
        T++; arr[u] = T; rev[T] = u;
        lable[T] = sdom[T] = idom[T] = T;
        for (size_t i = 0; i < g[u].size(); i++) {
            int w = g[u][i];
            if (!arr[w]) {
                dfs0(w);
                fa[arr[w]] = arr[u];
            }
            rg[arr[w]].push_back(arr[u]);
        }
    }

    void build(int s) {
        dfs0(s);
        for (int i = T; i >= 1; i--) {
            for (size_t j = 0; j < rg[i].size(); j++) {
                sdom[i] = min(sdom[i], sdom[Find(rg[i][j])]);
            }
            if (i > 1) bucket[sdom[i]].push_back(i);   
            for (size_t j = 0; j < bucket[i].size(); j++) {
                int w = bucket[i][j];
                int v = Find(w);
                if (sdom[v] == sdom[w]) dom[w] = sdom[w];
                else dom[w] = v;
            }
            bucket[i].clear();
            if (i > 1) Union(fa[i], i);
        }

        for (int i = 2; i <= T; i++) {
            if (dom[i] != sdom[i]) {
                dom[i] = dom[dom[i]];
            }
            // 建立支配樹的有向邊：idom -> u
            tree[rev[dom[i]]].push_back(rev[i]);
        }

        // 統計子樹大小：逆 DFS 序累加（保證子節點一定先於父節點被統計）
        for (int i = 1; i <= T; i++) sz[rev[i]] = 1;
        for (int i = T; i >= 2; i--) {
            int u = rev[i];
            int p = rev[dom[i]];
            sz[p] += sz[u];
        }
    }
} DT;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> m)) return 0;

    DT.init(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        DT.add(u, v);
    }

    // P5180 預設從 1 號點出發
    DT.build(1);

    for (int i = 1; i <= n; i++) {
        cout << DT.sz[i] << (i == n ? "" : " ");
    }
    cout << "\n";

    return 0;
}
