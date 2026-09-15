#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// =========================================================================
// 1. 標準盧卡斯定理 (Lucas' Theorem) - 最常用！
// 適用條件：模數 p 必須是質數 (通常 p <= 10^5 ~ 10^6)，而 n, k 可達 10^18
// 時間複雜度：單次查詢 O(log_p n)
// 定理內容：C(n, k) ≡ C(n / p, k / p) * C(n % p, k % p) (mod p)
// =========================================================================

ll qpow(ll a, ll b, ll p) {
    ll res = 1; a %= p;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p;
        b >>= 1;
    }
    return res;
}

// 模 p 下的小組合數 C(n, k)
ll C_small(ll n, ll k, ll p) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    ll num = 1, den = 1;
    for (ll i = 0; i < k; ++i) {
        num = num * ((n - i) % p) % p;
        den = den * ((i + 1) % p) % p;
    }
    return num * qpow(den, p - 2, p) % p;
}

// 標準 Lucas 遞迴入口
ll lucas(ll n, ll k, ll p) {
    if (k < 0 || k > n) return 0;
    if (k == 0) return 1;
    return lucas(n / p, k / p, p) * C_small(n % p, k % p, p) % p;
}


// =========================================================================
// 2. 擴展盧卡斯定理 (Extended Lucas / exLucas)
// 適用條件：模數 m 是任意合數，但要求 m 的所有質因數次冪 p^a <= 10^6
// 注意事項：若 m 含有大質數（如 m = 998244353），預處理會配置過大記憶體導致 MLE
// =========================================================================

void ex_gcd(int a, int b, int& x, int& y) {
    if (!b) {
        x = 1; y = 0;
    } else {
        ex_gcd(b, a % b, y, x);
        y -= a / b * x;
    }
}

int inverse(int a, int m) {
    int x, y;
    ex_gcd(a, m, x, y);
    return (x % m + m) % m;
}

int crt_coeff(int m_i, int m) {
    long long mm = m / m_i;
    mm *= inverse(mm, m_i);
    return mm % m;
}

class BinomModPrimePower {
    int p, a, pa;
    vector<int> f;

    long long nu(long long n) {
        long long count = 0;
        do {
            n /= p;
            count += n;
        } while (n);
        return count;
    }

    long long fact_mod(long long n) {
        bool neg = (p != 2 || pa <= 4);
        long long res = 1;
        while (n > 1) {
            if ((n / pa) & neg) res = pa - res;
            res = res * f[n % pa] % pa;
            n /= p;
        }
        return res;
    }

public:
    BinomModPrimePower(int p, int a, int pa) : p(p), a(a), pa(pa), f(pa) {
        f[0] = 1;
        for (int i = 1; i < pa; ++i) {
            f[i] = (i % p ? (long long)f[i - 1] * i % pa : f[i - 1]);
        }
    }

    int binomial(long long n, long long k) {
        // 重要修復：必須特判 k < 0 或 k > n，否則會陷入負數死循環！
        if (k < 0 || k > n) return 0;
        long long v = nu(n) - nu(n - k) - nu(k);
        if (v >= a) return 0;
        auto res = fact_mod(n - k) * fact_mod(k) % pa;
        res = fact_mod(n) * inverse(res, pa) % pa;
        for (; v; --v) res = (res * p) % pa;
        return res % pa;
    }
};

class BinomMod {
    int m;
    vector<BinomModPrimePower> bp;
    vector<long long> crt_m;

public:
    BinomMod(int n) : m(n) {
        for (int p = 2; p * p <= n; ++p) {
            if (n % p == 0) {
                int a = 0, pa = 1;
                for (; n % p == 0; n /= p, ++a, pa *= p);
                bp.emplace_back(p, a, pa);
                crt_m.emplace_back(crt_coeff(pa, m));
            }
        }
        if (n > 1) {
            bp.emplace_back(n, 1, n);
            crt_m.emplace_back(crt_coeff(n, m));
        }
    }

    int binomial(long long n, long long k) {
        if (k < 0 || k > n) return 0;
        long long res = 0;
        for (size_t i = 0; i != bp.size(); ++i) {
            res = (bp[i].binomial(n, k) * crt_m[i] + res) % m;
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 測試標準 Lucas
    ll n = 10, k = 3, p = 7;
    cout << "Lucas C(10, 3) mod 7 = " << lucas(n, k, p) << "\n";

    // 測試 exLucas (包含 k > n 邊界測試)
    BinomMod bm(10); // mod 10
    cout << "exLucas C(5, 10) mod 10 = " << bm.binomial(5, 10) << " (應為 0)\n";
    cout << "exLucas C(10, 3) mod 10 = " << bm.binomial(10, 3) << " (應為 0，因為 120 mod 10 = 0)\n";

    return 0;
}