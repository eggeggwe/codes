#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef vector<ll> vll;
#define pb push_back

const int N = 500010;

vector<int> graph[N];
int dfn[N], low[N], cnt = 0;
vector<int> st;
vector<vll> ans;

void dfs(int x, int pr) {
    dfn[x] = low[x] = ++cnt;
    st.pb(x);
    for (auto u : graph[x]) {
        if (u == pr) continue;
        if (!dfn[u]) {
            dfs(u, x);
            low[x] = min(low[x], low[u]);
            if (low[u] >= dfn[x]) {
                ans.pb(vll());
                int start;
                do {
                    start = st.back();
                    st.pop_back();
                    ans.back().pb(start);
                } while (start != u);
                ans.back().pb(x);
            }
        } else {
            low[x] = min(low[x], dfn[u]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        if (u == v) continue; // 自環不影響點雙連通性，且會干擾樹枝/返祖邊判定
        graph[u].pb(v);
        graph[v].pb(u);
    }

    for (int i = 1; i <= n; ++i) {
        if (!dfn[i]) {
            // 處理孤立點（沒有任何鄰邊）：它自己單獨形成一個點雙連通分量
            if (graph[i].empty()) {
                ans.pb({i});
                continue;
            }
            dfs(i, 0);
        }
    }

    cout << ans.size() << "\n";
    for (const auto &comp : ans) {
        cout << comp.size();
        for (auto node : comp) {
            cout << " " << node;
        }
        cout << "\n";
    }

    return 0;
}
