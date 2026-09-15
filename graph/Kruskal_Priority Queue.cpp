#include <iostream>
#include <vector>
#include <queue>
#include <cstdio>

using namespace std;

typedef long long ll;

struct Edge {
    int u, v, w;
    // 定義優先佇列比較規則：權重較小者優先 (小根堆)
    bool operator>(const Edge& other) const {
        return w > other.w;
    }
};

const int MAXN = 5005; // 根據洛谷 P3366，N <= 5000，M <= 200000
int parent_node[MAXN];

// 並查集初始化
void init(int n) {
    for (int i = 1; i <= n; i++) {
        parent_node[i] = i;
    }
}

// 帶路徑壓縮的查找
int find_set(int x) {
    if (x == parent_node[x]) return x;
    return parent_node[x] = find_set(parent_node[x]);
}

// 合併集合
bool union_set(int x, int y) {
    int root_x = find_set(x);
    int root_y = find_set(y);
    if (root_x != root_y) {
        parent_node[root_x] = root_y;
        return true;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    init(n);

    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        pq.push({u, v, w});
    }

    ll total_weight = 0;
    int edge_count = 0;

    while (!pq.empty() && edge_count < n - 1) {
        Edge cur = pq.top();
        pq.pop();

        if (union_set(cur.u, cur.v)) {
            total_weight += cur.w;
            edge_count++;
        }
    }

    // N 個節點的樹必須剛好有 N - 1 條邊
    if (edge_count == n - 1) {
        cout << total_weight << "\n";
    } else {
        cout << "orz\n";
    }

    return 0;
}
