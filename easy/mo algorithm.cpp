#include <bits/stdc++.h>
using namespace std;

const int BLOCK_SIZE = 316; // 建議設為 N / sqrt(Q) 或 ~320

struct Query {
    int l, r, id;
    bool operator<(const Query &other) const {
        int b1 = l / BLOCK_SIZE;
        int b2 = other.l / BLOCK_SIZE;
        if (b1 != b2) return b1 < b2;
        // 奇偶區塊排序優化，減少指針來回晃動
        return (b1 & 1) ? (r < other.r) : (r > other.r);
    }
};

int n, q;
int a[100010];
int cnt[100010]; // 記錄每個數字當前出現次數
int ans[1000010];
int cur_pairs = 0;
vector<Query> queries;

inline void add(int val) {
    cnt[val]++;
    if ((cnt[val] & 1) == 0) { // 奇數變偶數，湊成新的一對
        cur_pairs++;
    }
}

inline void remove(int val) {
    if ((cnt[val] & 1) == 0) { // 偶數變奇數，拆散一對
        cur_pairs--;
    }
    cnt[val]--;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    cin >> q;
    queries.assign(q,Query());
    for (int i = 0; i < q; i++) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].id = i;
    }

    sort(queries.begin(), queries.end());

    int curL = 1, curR = 0;
    for (const auto &qry : queries) {
        while (curL > qry.l) add(a[--curL]);
        while (curR < qry.r) add(a[++curR]);
        while (curL < qry.l) remove(a[curL++]);
        while (curR > qry.r) remove(a[curR--]);
        ans[qry.id] = cur_pairs;
    }

    for (int i = 0; i < q; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}