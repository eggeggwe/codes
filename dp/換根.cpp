#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
vector<int> adj[MAXN];
long long sz[MAXN], ans[MAXN];
int n;

// 第一次 DFS：收集子樹資訊 (Bottom-up)
void dfs1(int u, int p) {
    sz[u] = 1;
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs1(v, u);
        sz[u] += sz[v];
        ans[u] += ans[v] + sz[v]; // 累加以 u 為根的子樹內距離總和
    }
}

// 第二次 DFS：換根計算全局答案 (Top-down)
void dfs2(int u, int p) {
    for (int v : adj[u]) {
        if (v == p) continue;
        // 核心換根轉移：v 子樹靠近 1 步，其他點遠離 1 步
        ans[v] = ans[u] - sz[v] + (n - sz[v]);
        dfs2(v, u);
    }
}

void solve() {
    cin >> n;
    for (int i = 1; i <= n; i++) adj[i].clear(), ans[i] = 0;
    
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs1(1, 0); // 算得以 1 為根時的 ans[1]
    dfs2(1, 0); // 換根擴展至所有節點

    for (int i = 1; i <= n; i++) {
        cout << ans[i] << (i == n ? '\n' : ' ');
    }
}