#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// =========================================================================
// 線性篩預處理多個積性函數 (歐拉函數 phi, 莫比烏斯函數 mu, 因數個數 d, 因數和 sigma)
// 時間複雜度：O(n) 線性時間
// =========================================================================

const int MAXN = 1000005;
bool is_prime[MAXN];
vector<int> primes;

int phi[MAXN];      // 歐拉函數 phi
int mu[MAXN];       // 莫比烏斯函數 mu
int sum_mu[MAXN];   // mu 的前綴和 (用於數論分塊 / 杜教篩基礎)
int d[MAXN];        // 因數個數 d(n)
int num[MAXN];      // 最小質因數的次數 (輔助計算 d 與 sigma)
ll sigma[MAXN];     // 因數之和 sigma(n)
ll sp[MAXN];        // 最小質因數的貢獻和 (1 + p + ... + p^k)

void sieve(int n) {
    fill(is_prime, is_prime + n + 1, true);
    is_prime[0] = is_prime[1] = false;
    phi[1] = 1;
    mu[1] = 1;
    d[1] = 1;
    sigma[1] = 1;

    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
            mu[i] = -1;
            d[i] = 2;
            num[i] = 1;
            sigma[i] = i + 1;
            sp[i] = i + 1;
        }
        for (int p : primes) {
            if (1LL * i * p > n) break;
            is_prime[i * p] = false;

            if (i % p == 0) {
                phi[i * p] = phi[i] * p;
                mu[i * p] = 0; // 包含平方因子 p^2

                num[i * p] = num[i] + 1;
                d[i * p] = d[i] / (num[i] + 1) * (num[i * p] + 1);

                sp[i * p] = sp[i] * p + 1;
                sigma[i * p] = sigma[i] / sp[i] * sp[i * p];
                break;
            } else {
                phi[i * p] = phi[i] * (p - 1);
                mu[i * p] = -mu[i];

                num[i * p] = 1;
                d[i * p] = d[i] * 2;

                sp[i * p] = p + 1;
                sigma[i * p] = sigma[i] * (p + 1);
            }
        }
    }

    // 計算 mu 前綴和
    for (int i = 1; i <= n; ++i) {
        sum_mu[i] = sum_mu[i - 1] + mu[i];
    }
}

// 數論分塊範例：求 sum_{i=1}^n floor(n / i)
ll block_sum(ll n) {
    ll ans = 0;
    for (ll l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);
        ans += (n / l) * (r - l + 1);
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 100;
    sieve(n);
    cout << "phi(12) = " << phi[12] << " (應為 4)\n";
    cout << "mu(12) = " << mu[12] << " (應為 0)\n";
    cout << "d(12) = " << d[12] << " (應為 6: 1,2,3,4,6,12)\n";
    cout << "sigma(12) = " << sigma[12] << " (應為 28)\n";
    return 0;
}
