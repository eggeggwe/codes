#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// =========================================================================
// 64 位元安全乘法取模
// 在 64-bit 競賽環境 (Linux) 使用 __int128，在 32-bit 本地環境自動回退二進制乘法
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
    while (b) {
        ll t = a % b;
        a = b;
        b = t;
    }
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

// 隨機數生成器：使用 64 位梅森旋轉演算法，徹底杜絕 Windows rand() 只有 15 位元的問題
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

// =========================================================================
// 1. 確定性 Miller-Rabin 質數測試 (2^64 範圍內 100% 正確)
// 使用前 12 個質數作為基底：{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}
// =========================================================================
bool is_prime(ll n) {
    if (n < 2) return false;
    static const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int p : primes) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    ll d = n - 1;
    int s = 0;
    while (!(d & 1)) {
        d >>= 1;
        s++;
    }
    for (int p : primes) {
        ll x = qpow(p, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

// =========================================================================
// 2. Pollard's Rho 演算法求大數因數
// =========================================================================
ll pollard_rho(ll n) {
    if (n % 2 == 0) return 2;
    if (is_prime(n)) return n;

    while (true) {
        ll c = rng() % (n - 1) + 1;
        auto f = [&](ll v) { return (mul_mod(v, v, n) + c) % n; };
        ll x = rng() % (n - 2) + 2, y = x;
        ll d = 1;
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            if (x == y) break;
            d = gcd(abs(x - y), n);
        }
        if (d > 1 && d < n) return d;
    }
}

// 遞迴分解質因數
void factorize(ll n, vector<ll> &factors) {
    if (n <= 1) return;
    if (is_prime(n)) {
        factors.push_back(n);
        return;
    }
    ll d = pollard_rho(n);
    factorize(d, factors);
    factorize(n / d, factors);
}

// =========================================================================
// 3. 試除法質因數分解 (O(sqrt(n)))
// 適用條件：n <= 10^12，結構簡單，競賽中最常臨時手寫
// =========================================================================
vector<pair<ll, int>> factorize_simple(ll n) {
    vector<pair<ll, int>> res;
    for (ll i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                cnt++;
            }
            res.push_back({i, cnt});
        }
    }
    if (n > 1) res.push_back({n, 1});
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (cin >> t) {
        while (t--) {
            ll n;
            cin >> n;
            vector<ll> factors;
            factorize(n, factors);
            sort(factors.begin(), factors.end());

            cout << n << " = ";
            for (size_t i = 0; i < factors.size(); ++i) {
                if (i) cout << " * ";
                cout << factors[i];
            }
            cout << "\n";
            cout << (is_prime(n) ? "質數" : "合數") << "\n";
        }
    }
    return 0;
}
