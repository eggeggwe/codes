#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// =========================================================================
// 離散對數演算法：BSGS (Baby-Step Giant-Step) 與 exBSGS
// 求解同餘方程：a^x ≡ b (mod m) 的最小非負整數解 x
// 時間複雜度：O(sqrt(m))
// =========================================================================

#ifdef __SIZEOF_INT128__
inline ll mul_mod(ll a, ll b, ll m) {
    return (ll)((__int128)a * b % m);
}
#else
inline ll mul_mod(ll a, ll b, ll m) {
    ll res = 0; a %= m;
    while (b) {
        if (b & 1) res = (res + a >= m ? res + a - m : res + a);
        a = (a + a >= m ? a + a - m : a + a);
        b >>= 1;
    }
    return res;
}
#endif

ll gcd(ll a, ll b) {
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

ll qpow(ll a, ll b, ll m) {
    ll res = 1; a %= m;
    while (b) {
        if (b & 1) res = mul_mod(res, a, m);
        a = mul_mod(a, a, m);
        b >>= 1;
    }
    return res;
}

// 1. 標準 BSGS：要求 gcd(a, m) == 1
ll bsgs(ll a, ll b, ll m) {
    a %= m; b %= m;
    if (b == 1 || m == 1) return 0;

    unordered_map<ll, ll> hash;
    ll s = ceil(sqrt(m));
    ll cur = b;

    // Baby-step: 計算 b * a^j (0 <= j < s) 並存入 hash table
    for (ll j = 0; j < s; ++j) {
        hash[cur] = j;
        cur = mul_mod(cur, a, m);
    }

    // Giant-step: 計算 a^(i * s) (1 <= i <= s)
    ll step = qpow(a, s, m);
    cur = 1;
    for (ll i = 1; i <= s + 1; ++i) {
        cur = mul_mod(cur, step, m);
        if (hash.count(cur)) {
            ll ans = i * s - hash[cur];
            return ans;
        }
    }
    return -1; // 無解
}

// 2. 擴展 BSGS (exBSGS)：gcd(a, m) 可以不為 1
ll exbsgs(ll a, ll b, ll m) {
    a %= m; b %= m;
    if (b == 1 || m == 1) return 0;

    ll k = 0, d = 1, g;
    while ((g = gcd(a, m)) > 1) {
        if (b % g != 0) return -1; // 無解
        k++;
        b /= g;
        m /= g;
        d = mul_mod(d, a / g, m);
        if (d == b) return k;
    }

    // 將方程式化為 a^(x - k) * d ≡ b (mod m)，其中 gcd(a, m) == 1
    unordered_map<ll, ll> hash;
    ll s = ceil(sqrt(m));
    ll cur = b;

    for (ll j = 0; j < s; ++j) {
        hash[cur] = j;
        cur = mul_mod(cur, a, m);
    }

    ll step = qpow(a, s, m);
    cur = d;
    for (ll i = 1; i <= s + 1; ++i) {
        cur = mul_mod(cur, step, m);
        if (hash.count(cur)) {
            ll ans = i * s - hash[cur] + k;
            return ans;
        }
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 範例：求解 2^x ≡ 8 (mod 11) -> x = 3
    ll a = 2, b = 8, m = 11;
    cout << "2^x ≡ 8 (mod 11), x = " << exbsgs(a, b, m) << "\n";
    return 0;
}
