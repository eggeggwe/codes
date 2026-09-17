// There are N segments y=aix+bi(where  x∈[li,ri)). Process Q queries.

// 0 l r a b: Add a segment y=ax+b(where x∈[l,r))
// 1 p: Find the minimal y at x=p. If such y doesn't exist, output INFINITY.
// 使用方法:
//   insert_segment(1,0,M-1,cl,cr,Line(a,b))   加入線段 (座標需先離散化存入 xs[])
//   qry(1,0,M-1,cp)                            查詢 x=p 處最小值，回傳 INT64_MAX 表示不存在
// 注意: 下標從0開始 (離散化後)，需先把所有 l、r-1、p 收集到 xs 再排序去重
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;
#define N 1000010
typedef long long ll;
#define L(id) ((id) << 1)
#define R(id) ((id) << 1 | 1)

struct Line {
    ll a, b;
    Line(ll _a = 0, ll _b = INT64_MAX): a(_a), b(_b) {}
    ll operator()(ll x) const { return a * x + b; }
};

static Line seg[N << 2];
static vector<ll> xs;

void insert_line(int idx, int l, int r, Line ln) {
    int m = (l + r) >> 1;
    ll xm = xs[m];
    // 比較中點
    if (ln(xm) < seg[idx](xm)) {
        swap(seg[idx], ln);
    }
    if (l == r) return;
    ll xl = xs[l], xr = xs[r];
    if (ln(xl) < seg[idx](xl))
        insert_line(L(idx), l, m, ln);
    if (ln(xr) < seg[idx](xr))
        insert_line(R(idx), m + 1, r, ln);
}

void insert_segment(int idx, int l, int r, int ql, int qr, Line ln) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        insert_line(idx, l, r, ln);
        return;
    }
    int m = (l + r) >> 1;
    insert_segment(L(idx), l, m, ql, qr, ln);
    insert_segment(R(idx), m + 1, r, ql, qr, ln);
}

ll qry(int id, int l, int r, int x) {
    int m = (l + r) >> 1;
    ll res = seg[id](xs[x]);
    if (l == r) return res;
    if (x <= m) res = min(res, qry(L(id), l, m, x));
    else         res = min(res, qry(R(id), m + 1, r, x));
    return res;
}

struct Op {
    int mode;
    int l, r;
    ll a, b;
    int p;
};

int main() {
    int n, q;
    scanf("%d %d", &n, &q);
    vector<Op> ops;
    ops.reserve(n + q);

    // 讀入初始 N 條線段
    for (int i = 0; i < n; i++) {
        int l, r;
        ll a, b;
        scanf("%d %d %lld %lld", &l, &r, &a, &b);
        ops.push_back({0, l, r - 1, a, b, 0});
        xs.push_back(l);
        xs.push_back(r - 1);
    }
    // 讀入 Q 個操作
    for (int i = 0; i < q; i++) {
        int mode;
        scanf("%d", &mode);
        if (mode == 0) {
            int l, r;
            ll a, b;
            scanf("%d %d %lld %lld", &l, &r, &a, &b);
            ops.push_back({0, l, r - 1, a, b, 0});
            xs.push_back(l);
            xs.push_back(r - 1);
        } else {
            int p;
            scanf("%d", &p);
            ops.push_back({1, 0, 0, 0, 0, p});
            xs.push_back(p);
        }
    }

    // 坐標壓縮
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    int M = xs.size();

    // 初始化線段樹
    for (int i = 0; i < 4 * N; i++) {
        seg[i] = Line(0, INT64_MAX);
    }

    // 處理所有操作
    for (auto &op : ops) {
        if (op.mode == 0) {
            int cl = lower_bound(xs.begin(), xs.end(), op.l) - xs.begin();
            int cr = lower_bound(xs.begin(), xs.end(), op.r) - xs.begin();
            insert_segment(1, 0, M - 1, cl, cr, Line(op.a, op.b));
        } else {
            int cp = lower_bound(xs.begin(), xs.end(), op.p) - xs.begin();
            ll ans = qry(1, 0, M - 1, cp);
            if (ans == INT64_MAX) printf("INFINITY\n");
            else                   printf("%lld\n", ans);
        }
    }
    return 0;
}

