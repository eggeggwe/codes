// 嚴格次小生成樹 (Strictly Second Best MST)
// 用途: n個點、m條邊的無向圖，求「權重和嚴格大於最小生成樹」的生成樹中，權重和最小的那個
// 複雜度: O(m log m + n log n)
// 使用方法: 直接輸入 n,m 後接 m 條邊 (u v w)，main() 會自動跑 Kruskal 建 MST、倍增求路徑最大/次大邊
// 原理: 對每條非樹邊 (u,v,w)，嘗試替換路徑 u-v 上小於 w 的最大邊 (若最大邊等於w則改用次大邊，確保嚴格更大)

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long ll;
const int N = 100010;
const int M = 300010;
const ll INF = 1e18;

struct Edge {
    int u, v;
    ll weight;
    bool in_mst;
} edge[M];

struct Adj {
    int to;
    ll weight;
};

int n, m;
int fa[N];
vector<Adj> g[N];

bool cmp(const Edge &a, const Edge &b) {
    return a.weight < b.weight;
}

int find(int x) {
    return (fa[x] == x) ? x : (fa[x] = find(fa[x]));
}

ll mst_weight = 0;
int edges_count = 0;

void kruskal() {
    for (int i = 1; i <= n; i++) fa[i] = i;
    sort(edge + 1, edge + m + 1, cmp);
    for (int i = 1; i <= m; i++) {
        int eu = find(edge[i].u), ev = find(edge[i].v);
        if (eu == ev) continue;
        fa[ev] = eu;
        mst_weight += edge[i].weight;
        edge[i].in_mst = true;
        g[edge[i].u].push_back({edge[i].v, edge[i].weight});
        g[edge[i].v].push_back({edge[i].u, edge[i].weight});
        if (++edges_count == n - 1) break;
    }
}

int depth[N], f[N][20];
ll max_w[N][20], sec_w[N][20];

// 合併兩個區間的最大值與嚴格次大值
void merge_info(ll mx1, ll se1, ll mx2, ll se2, ll &out_mx, ll &out_se) {
    ll vals[4] = {mx1, se1, mx2, se2};
    out_mx = -1;
    out_se = -1;
    for (int i = 0; i < 4; i++) {
        if (vals[i] > out_mx) {
            out_se = out_mx;
            out_mx = vals[i];
        } else if (vals[i] < out_mx && vals[i] > out_se) {
            out_se = vals[i];
        }
    }
}

void dfs(int now, int p, int d, ll w) {
    depth[now] = d;
    f[now][0] = p;
    max_w[now][0] = w;
    sec_w[now][0] = -1;

    for (int i = 1; i <= 18; i++) {
        f[now][i] = f[f[now][i - 1]][i - 1];
        merge_info(max_w[now][i - 1], sec_w[now][i - 1],
                   max_w[f[now][i - 1]][i - 1], sec_w[f[now][i - 1]][i - 1],
                   max_w[now][i], sec_w[now][i]);
    }

    for (auto &e : g[now]) {
        if (e.to != p) {
            dfs(e.to, now, d + 1, e.weight);
        }
    }
}

// 查詢 u 到 v 路徑上，嚴格小於 limit 的最大邊權
ll query_max(int u, int v, ll limit) {
    ll cur_max = -1, cur_sec = -1;

    auto update = [&](ll mx, ll se) {
        merge_info(cur_max, cur_sec, mx, se, cur_max, cur_sec);
    };

    if (depth[u] < depth[v]) swap(u, v);
    for (int i = 18; i >= 0; i--) {
        if (depth[u] - (1 << i) >= depth[v]) {
            update(max_w[u][i], sec_w[u][i]);
            u = f[u][i];
        }
    }

    if (u != v) {
        for (int i = 18; i >= 0; i--) {
            if (f[u][i] != f[v][i]) {
                update(max_w[u][i], sec_w[u][i]);
                update(max_w[v][i], sec_w[v][i]);
                u = f[u][i];
                v = f[v][i];
            }
        }
        update(max_w[u][0], sec_w[u][0]);
        update(max_w[v][0], sec_w[v][0]);
    }

    if (cur_max != -1 && cur_max < limit) return cur_max;
    if (cur_sec != -1 && cur_sec < limit) return cur_sec;
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> m)) return 0;

    for (int i = 1; i <= m; i++) {
        cin >> edge[i].u >> edge[i].v >> edge[i].weight;
        edge[i].in_mst = false;
    }

    kruskal();
    dfs(1, 0, 1, 0);

    ll ans = INF;
    for (int i = 1; i <= m; i++) {
        if (edge[i].in_mst || edge[i].u == edge[i].v) continue;

        ll best_edge = query_max(edge[i].u, edge[i].v, edge[i].weight);
        if (best_edge != -1) {
            ans = min(ans, mst_weight - best_edge + edge[i].weight);
        }
    }

    cout << ans << "\n";
    return 0;
}
