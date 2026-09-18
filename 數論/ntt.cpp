#include <bits/stdc++.h>
using namespace std;

const long long MOD = 998244353;
const long long G = 3;

long long modpow(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) res = res * base % MOD;
        base = base * base % MOD;
        exp >>= 1;
    }
    return res;
}

void NTT(vector<long long>& a, bool invert) {
    int n = a.size();
    if (n <= 1) return;

    vector<int> rev(n);
    int k = __lg(n);
    for (int i = 1; i < n; i++) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        long long wlen = modpow(G, (MOD - 1) / len);
        if (invert) wlen = modpow(wlen, MOD - 2);
        for (int i = 0; i < n; i += len) {
            long long w = 1;
            int half = len >> 1;
            for (int j = 0; j < half; j++) {
                long long u = a[i + j];
                long long v = a[i + j + half] * w % MOD;
                a[i + j] = (u + v >= MOD ? u + v - MOD : u + v);
                a[i + j + half] = (u - v < 0 ? u - v + MOD : u - v);
                w = w * wlen % MOD;
            }
        }
    }

    if (invert) {
        long long inv_n = modpow(n, MOD - 2);
        for (auto& x : a) x = x * inv_n % MOD;
    }
}

vector<long long> convolution(vector<long long> A, vector<long long> B) {
    if (A.empty() || B.empty()) return {};
    int tt = A.size() + B.size() - 1;
    int n = 1;
    while (n < tt) n <<= 1;

    A.resize(n, 0);
    B.resize(n, 0);

    NTT(A, false);
    NTT(B, false);
    for (int i = 0; i < n; i++)
        A[i] = A[i] * B[i] % MOD;
    NTT(A, true);

    A.resize(tt);
    return A;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<long long> a(n), b(m);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < m; i++) cin >> b[i];

    auto c = convolution(a, b);
    for (int i = 0; i < (int)c.size(); i++) {
        cout << c[i] << (i + 1 == (int)c.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}