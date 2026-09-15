#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// =========================================================================
// 擴展歐拉定理 (Extended Euler's Theorem)
// 對於任意整數 a, m >= 1 以及大整數 b：
// a^b ≡
//   a^b                                (mod m), 當 b < phi(m)
//   a^( (b mod phi(m)) + phi(m) )      (mod m), 當 b >= phi(m)
//
// 備註：此定理對 gcd(a, m) 是否互質「皆成立」，非常適合處理 b 極大（字串讀入）的情形
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

ll qpow(ll a, ll b, ll m) {
    ll res = 1 % m;
    a %= m;
    while (b) {
        if (b & 1) res = mul_mod(res, a, m);
        a = mul_mod(a, a, m);
        b >>= 1;
    }
    return res;
}

ll get_phi(ll n) {
    ll res = n;
    for (ll i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            res = res / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) res = res / n * (n - 1);
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll a, m;
    string b;
    if (!(cin >> a >> m >> b)) return 0;

    // 特殊情況：任何整數 mod 1 都是 0
    if (m == 1) {
        cout << 0 << "\n";
        return 0;
    }

    ll phi = get_phi(m);
    ll bm = 0;
    bool flag = false;

    // 關鍵修復：bm 必須是 long long，否則在 bm * 10 時可能超過 2^31 - 1 溢位成負數
    for (char ch : b) {
        bm = bm * 10 + (ch - '0');
        if (bm >= phi) {
            flag = true;
            bm %= phi;
        }
    }

    if (flag) bm += phi;

    cout << qpow(a, bm, m) << "\n";
    return 0;
}
