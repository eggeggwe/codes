/*n 个点 m 条边有向图,每个点有一个权值，求一条路径，使路径经过的点权值之和最大。你只需要求出这个权值和。*/
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

#define pb push_back
#define rep(i, a, b) for (int i = (a); i < (b); ++i)
using ll = long long;
using vll = vector<ll>;

struct TarjanSCC {
    int n, timer, scc_cnt;
    vll dfn, low, scc_id, inside;
    vector<vll> scc_nodes;
    vector<vll> SCC;
    stack<int> st;

    TarjanSCC(int _n) : n(_n), timer(0), scc_cnt(0),
        dfn(_n, 0), low(_n, 0), scc_id(_n, 0), inside(_n, 0), scc_nodes(1) {}

    void dfs(int u, const vector<vll> &graph) {
        dfn[u] = low[u] = ++timer;
        st.push(u);
        inside[u] = 1;

        for (int v : graph[u]) {
            if (!dfn[v]) {
                dfs(v, graph);
                low[u] = min(low[u], low[v]);
            } else if (inside[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            scc_cnt++;
            scc_nodes.push_back({});
            while (true) {
                int v = st.top(); st.pop();
                inside[v] = 0;
                scc_id[v] = scc_cnt;
                scc_nodes.back().push_back(v);
                if (u == v) break;
            }
        }
    }

    void build(vector<vll> &graph) {
        SCC.assign(scc_cnt + 1, {});
        rep(i, 0, n) {
            if (scc_id[i] == 0) continue;
            for (auto u : graph[i]) {
                if (scc_id[u] == 0) continue;
                if (scc_id[i] == scc_id[u]) continue;
                SCC[scc_id[i]].pb(scc_id[u]);
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vll val(n);
    rep(i, 0, n) cin >> val[i];

    vector<vll> graph(n);
    rep(i, 0, m) {
        int u, v;
        cin >> u >> v;
        u--; v--; // 轉為 0-indexed 配合 struct
        graph[u].pb(v);
    }

    // 1. 初始化並執行 Tarjan
    TarjanSCC tarjan(n);
    rep(i, 0, n) {
        if (!tarjan.dfn[i]) {
            tarjan.dfs(i, graph);
        }
    }

    // 2. 建構縮點後的 DAG
    tarjan.build(graph);
    int k = tarjan.scc_cnt;

    // 3. 透過 scc_nodes 累加各 SCC 的權重
    vll scc_val(k + 1, 0);
    for (int i = 1; i <= k; i++) {
        for (int u : tarjan.scc_nodes[i]) {
            scc_val[i] += val[u];
        }
    }

    // 4. 去重邊並計算入度
    vll in_deg(k + 1, 0);
    for (int i = 1; i <= k; i++) {
        sort(tarjan.SCC[i].begin(), tarjan.SCC[i].end());
        tarjan.SCC[i].erase(unique(tarjan.SCC[i].begin(), tarjan.SCC[i].end()), tarjan.SCC[i].end());
        for (int v : tarjan.SCC[i]) {
            in_deg[v]++;
        }
    }

    // 5. 拓撲排序求最長路 DP
    queue<int> q;
    vll dp(k + 1, 0);
    for (int i = 1; i <= k; i++) {
        dp[i] = scc_val[i];
        if (in_deg[i] == 0) q.push(i);
    }

    ll ans = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ans = max(ans, dp[u]);

        for (int v : tarjan.SCC[u]) {
            dp[v] = max(dp[v], dp[u] + scc_val[v]);
            if (--in_deg[v] == 0) {
                q.push(v);
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
