#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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

// ==========================================
// 1. 單點乘法逆元 (利用快速冪與費馬小定理)
// 適用條件：模數 p 必須是質數，且 gcd(a, p) == 1
// ==========================================
ll qpow(ll a, ll b, ll p) {
    ll res = 1;
    a %= p;
    while (b) {
        if (b & 1) res = mul_mod(res, a, p);
        a = mul_mod(a, a, p);
        b >>= 1;
    }
    return res;
}

ll inv_fermat(ll a, ll p) {
    return qpow(a, p - 2, p);
}

// ==========================================
// 2. 單點乘法逆元 (利用擴展歐幾里得 exgcd)
// 適用條件：只需 gcd(a, m) == 1，模數 m 可以是任意合數
// ==========================================
ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (!b) { x = 1; y = 0; return a; }
    ll g = exgcd(b, a % b, y, x);
    y -= (a / b) * x;
    return g;
}

ll inv_exgcd(ll a, ll m) {
    ll x, y;
    ll g = exgcd(a, m, x, y);
    if (g != 1) return -1; // gcd != 1，無逆元
    return (x % m + m) % m;
}

// ==========================================
// 3. 線性預處理 1 ~ n 的所有逆元 (O(n))
// 適用條件：模數 p 為質數，n < p
// ==========================================
const int MAXN = 3000005;
int inv[MAXN];

void init_linear_inv(int n, int p) {
    inv[1] = 1;
    for (int i = 2; i <= n; ++i) {
        inv[i] = (int)(1LL * (p - p / i) * inv[p % i] % p);
    }
}

// ==========================================
// 4. 階乘與階乘逆元預處理 (O(n) 預處理，O(1) 查詢組合數 C(n, k))
// 適用條件：模數 p 為質數，最常考的 ICPC 組合數學模板
// ==========================================
ll fact[MAXN], invFact[MAXN];

void init_combination(int n, ll p) {
    fact[0] = invFact[0] = 1;
    for (int i = 1; i <= n; i++) {
        fact[i] = fact[i - 1] * i % p;
    }
    // 透過費馬小定理求出最大階乘的逆元
    invFact[n] = inv_fermat(fact[n], p);
    // 逆向線性推出所有階乘逆元：invFact[i] = invFact[i+1] * (i+1)
    for (int i = n - 1; i >= 1; i--) {
        invFact[i] = invFact[i + 1] * (i + 1) % p;
    }
}

ll nCr(int n, int r, ll p) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % p * invFact[n - r] % p;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    if (cin >> n >> p) {
        init_linear_inv(n, p);
        for (int i = 1; i <= n; ++i) {
            cout << inv[i] << "\n";
        }
    }
    return 0;
}
