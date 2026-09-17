// 模板證明 by
// 給定一張最初有m條邊的無向圖，接著有q個操作，每個操作要麼 新增 一條邊，要麼 刪除 一條邊。每個時刻我們都要能回答「目前圖中有幾個連通塊？」。由於q最多可達10^5，直接在線上（online）每次更新重跑 DFS 或 BFS 顯然過慢。
// 正式名稱: 線段樹分治 (離線動態連通性，非持久化資料結構，命名易混淆注意)
// 使用方法: 依序輸入 T 筆測資，每筆先給初始 m 條邊，再給 q 個操作 (N=新增，其他字元=刪除)
// 注意: 節點需為 1..n，parent 陣列大小為 n+1
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <vector>
#include <stack>
#include <string.h>
#include <unordered_map>
#include <cstdint>
using namespace std;
typedef long long ll;
#define N 1000000

vector<int> parent;
stack<pair<int,int>> ops;
int components;

int find(int x) {
    while (parent[x] >= 0) x = parent[x];
    return x;
}

bool merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return false;
    if (-parent[x] > -parent[y]) swap(x, y);
    ops.emplace(x, parent[x]);
    ops.emplace(y, parent[y]);
    parent[y] += parent[x];
    parent[x] = y;
    --components;
    return true;
}

void rollback(int snap) {
    while ((int)ops.size() > snap) {
        auto p = ops.top(); ops.pop();
        parent[p.first] = p.second;
        if ((int)ops.size() % 2 == 0) {
                ++components;
        }
    }
}


struct Event {
    int u, v;
};

struct input_evens {
    char mode;
    int u, v, t;
};

vector<Event> tr[4*N+10];
vector<input_evens> op;
unordered_map<uint64_t, vector<int>> last;

int n, m, q, T;


void insert_event(int idx, int lb, int rb, int L, int R, const Event &e) {
    if (L > R) return;
    if (L <= lb && rb <= R) {
        tr[idx].push_back(e);
        return;
    }
    int mid = (lb + rb) / 2;
    if (L <= mid)      insert_event(idx<<1, lb, mid, L, R, e);
    if (R >  mid)      insert_event(idx<<1|1, mid+1, rb, L, R, e);
}

void traversal(int idx, int lb, int rb) {
    int snap = ops.size();
    for (auto &e : tr[idx]) {
        merge(e.u, e.v);
    }
    if (lb == rb) {
        cout << components << "\n"; 
    } else {
        int mid = (lb + rb) / 2;
        traversal(idx<<1, lb, mid);
        traversal(idx<<1|1, mid+1, rb);
    }
    rollback(snap);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;
    while (T--) {
        cin >> n >> m >> q;
        last.reserve(m+q);
        parent.assign(n + 1, -1); // 節點可能是 1-indexed，多留一格避免越界
        components = n;
        while (!ops.empty()) ops.pop();
        op.clear();
        last.clear();
        for (int i = 1; i <= 4*q; i++) tr[i].clear();

        for (int i = 0; i < m; i++) {
            int a, b; cin >> a >> b;
            if (a > b) swap(a, b);
            op.push_back({'N', a, b, 1});
        }
        for (int i = 1; i <= q; i++) {
            char md; int a, b;
            cin >> md >> a >> b;
            if (a > b) swap(a, b);
            op.push_back({md, a, b, i});
        }
        for (auto &e : op) {
            uint64_t key = (uint64_t(e.u) << 32) | e.v;
            if (e.mode == 'N') {
                last[key].emplace_back(e.t);
            } else {
                auto &stk = last[key];
                int L = stk.back(); stk.pop_back();
                insert_event(1, 1, q, L, e.t-1, {e.u, e.v});
                if (stk.empty()) last.erase(key);
            }
        }
        for (auto &kv : last) {
            uint64_t key = kv.first;
            int u = int(key >> 32), v = int(key & 0xFFFFFFFF);
            for (int L : kv.second) {
                insert_event(1, 1, q, L, q, {u, v});
            }
        }
        traversal(1, 1, q);
    }
    return 0;
}

