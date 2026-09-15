#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// =========================================================================
// 線性基 (Linear Basis / XOR 空間基底)
// 應用場景：
// 1. 給定一個整數集合，求任選子集的最大異或和 (Max XOR Sum)
// 2. 判斷某個數能否由集合中的數異或組成
// 3. 求集合所有子集異或和中的第 k 小值
// 時間複雜度：插入 O(log V)，查詢 O(log V) (V 為值域，64 位整數最多 62 步)
// =========================================================================

const int MAXL = 62;

struct LinearBasis {
    ll p[MAXL + 1];
    vector<ll> d; // 用於查詢第 k 小
    bool has_zero;

    LinearBasis() {
        memset(p, 0, sizeof(p));
        d.clear();
        has_zero = false;
    }

    // 插入一個元素
    bool insert(ll x) {
        for (int i = MAXL; i >= 0; --i) {
            if (!(x >> i & 1)) continue;
            if (!p[i]) {
                p[i] = x;
                return true;
            }
            x ^= p[i];
        }
        has_zero = true; // x 被完全消成 0，代表存在子集異或為 0
        return false;
    }

    // 查詢子集最大異或和
    ll query_max() {
        ll res = 0;
        for (int i = MAXL; i >= 0; --i) {
            if ((res ^ p[i]) > res) {
                res ^= p[i];
            }
        }
        return res;
    }

    // 查詢子集最小非零異或和
    ll query_min() {
        for (int i = 0; i <= MAXL; ++i) {
            if (p[i]) return p[i];
        }
        return 0;
    }

    // 重構線性基（用於第 k 小查詢，將線性基化為行簡化階梯型）
    void rebuild() {
        d.clear();
        for (int i = MAXL; i >= 0; --i) {
            for (int j = i - 1; j >= 0; --j) {
                if (p[i] >> j & 1) {
                    p[i] ^= p[j];
                }
            }
        }
        for (int i = 0; i <= MAXL; ++i) {
            if (p[i]) d.push_back(p[i]);
        }
    }

    // 查詢所有不同子集異或和中的第 k 小值 (1-indexed)
    // 若無解（例如 k 大於可形成的總數），回傳 -1
    ll query_kth(ll k) {
        if (has_zero) k--;
        if (k == 0) return 0;
        if (k >= (1ULL << d.size())) return -1;

        ll res = 0;
        for (size_t i = 0; i < d.size(); ++i) {
            if (k >> i & 1) {
                res ^= d[i];
            }
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    LinearBasis lb;
    vector<ll> arr = {1, 2, 3, 4, 5};
    for (ll x : arr) lb.insert(x);

    cout << "最大 XOR 和: " << lb.query_max() << "\n";
    lb.rebuild();
    cout << "第 1 小 XOR 和: " << lb.query_kth(1) << "\n";
    cout << "第 2 小 XOR 和: " << lb.query_kth(2) << "\n";

    return 0;
}
