#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// =========================================================================
// 擴展中國剩餘定理 (EXCRT)
// 求解同餘方程組：x ≡ r_i (mod m_i)，其中 m_i「不保證」兩兩互質！
// 核心思想：將方程式兩兩合併
// x = m1 * p + r1 = m2 * q + r2 => m1 * p - m2 * q = r2 - r1
// 利用 exgcd 求解特解，並更新合併後的新模數 M = lcm(m1, m2)
// 時間複雜度：O(n log(lcm(m)))
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

ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (!b) { x = 1; y = 0; return a; }
    ll g = exgcd(b, a % b, y, x);
    y -= (a / b) * x;
    return g;
}

// 回傳滿足所有同餘式的最小非負整數解 x
// 若無解，回傳 -1
ll excrt(const vector<ll> &m, const vector<ll> &r) {
    int n = m.size();
    ll M = m[0], ans = (r[0] % m[0] + m[0]) % m[0];

    for (int i = 1; i < n; ++i) {
        ll a = M, b = m[i], c = ((r[i] - ans) % b + b) % b;
        ll x, y;
        ll g = exgcd(a, b, x, y);

        if (c % g != 0) return -1; // 無解 (c 不是 gcd(a, b) 的倍數)

        ll mod = b / g;
        x = mul_mod(x, c / g, mod);
        x = (x % mod + mod) % mod;

        ans += x * M;
        M = M / g * b; // M = lcm(M, b)
        ans = (ans % M + M) % M;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 範例：
    // x ≡ 2 (mod 4)
    // x ≡ 4 (mod 6)
    // lcm(4, 6) = 12，解為 x = 10 (mod 12)
    vector<ll> m = {4, 6};
    vector<ll> r = {2, 4};

    ll ans = excrt(m, r);
    if (ans == -1) cout << "No solution\n";
    else cout << "x = " << ans << "\n";

    return 0;
}
