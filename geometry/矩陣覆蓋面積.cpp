#include <bits/stdc++.h>
using namespace std;

// ==========================================================
// 矩形面積聯集 (Rectangle Area Union - 掃描線 + 線段樹)
// 時間複雜度：O(N log N)
// ==========================================================

const int MAXN = 200005;

struct Event {
    double x;
    double y1, y2;
    int type; // +1: 入邊, -1: 出邊

    bool operator<(const Event &other) const {
        if (x != other.x) return x < other.x;
        return type > other.type; // 相同 X 時先處理加邊
    }
} events[MAXN * 2];

double ys[MAXN * 2];

struct Node {
    int cnt;
    double len;
} tree[MAXN * 8];

inline void push_up(int node, int l, int r) {
    if (tree[node].cnt > 0) {
        tree[node].len = ys[r + 1] - ys[l];
    } else if (l == r) {
        tree[node].len = 0;
    } else {
        tree[node].len = tree[node << 1].len + tree[node << 1 | 1].len;
    }
}

void update(int node, int l, int r, int ql, int qr, int val) {
    if (ql <= l && r <= qr) {
        tree[node].cnt += val;
        push_up(node, l, r);
        return;
    }
    int mid = (l + r) >> 1;
    if (ql <= mid) update(node << 1, l, mid, ql, qr, val);
    if (qr > mid)  update(node << 1 | 1, mid + 1, r, ql, qr, val);
    push_up(node, l, r);
}

void solve() {
    int n;
    if (!(cin >> n) || n <= 0) return;

    int ev_cnt = 0, y_cnt = 0;
    for (int i = 0; i < n; ++i) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        // 確保座標順序正確 (左下、右上)
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);

        events[ev_cnt++] = {x1, y1, y2, 1};
        events[ev_cnt++] = {x2, y1, y2, -1};
        ys[y_cnt++] = y1;
        ys[y_cnt++] = y2;
    }

    // 1. Y 座標離散化
    sort(ys, ys + y_cnt);
    y_cnt = unique(ys, ys + y_cnt) - ys;

    if (y_cnt <= 1) {
        cout << fixed << setprecision(2) << 0.0 << "\n";
        return;
    }

    // 2. 掃描線事件依 X 排序
    sort(events, events + ev_cnt);

    // 3. 初始化線段樹 (多筆測資重置)
    int max_node = (y_cnt + 2) * 4;
    for (int i = 0; i <= max_node; ++i) {
        tree[i].cnt = 0;
        tree[i].len = 0.0;
    }

    double total_area = 0.0;

    // 4. 掃描線遍歷
    for (int i = 0; i < ev_cnt; ++i) {
        if (i > 0) {
            total_area += (events[i].x - events[i - 1].x) * tree[1].len;
        }

        int ql = lower_bound(ys, ys + y_cnt, events[i].y1) - ys;
        int qr = lower_bound(ys, ys + y_cnt, events[i].y2) - ys - 1;

        if (ql <= qr) {
            update(1, 0, y_cnt - 2, ql, qr, events[i].type);
        }
    }

    cout << fixed << setprecision(2) << total_area << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}