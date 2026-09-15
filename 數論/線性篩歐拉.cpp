#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ==========================================
// 1. 線性篩預處理歐拉函數 phi (O(n))
// 適用條件：需要查詢多筆 1 ~ n 範圍內的 phi 值
// ==========================================
const int MAXN = 1000005;
int phi[MAXN];
vector<int> primes;
bool is_prime[MAXN];

void init_phi(int n) {
    fill(is_prime, is_prime + n + 1, true);
    is_prime[0] = is_prime[1] = false;
    phi[1] = 1;
    primes.clear();

    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            phi[i] = i - 1; // 質數 p 的 phi(p) = p - 1
        }
        for (int p : primes) {
            if (1LL * i * p > n) break;
            is_prime[i * p] = false;
            if (i % p == 0) {
                phi[i * p] = phi[i] * p; // p 是 i 的質因數
                break; // 保證線性複雜度
            } else {
                phi[i * p] = phi[i] * (p - 1); // p 與 i 互質
            }
        }
    }
}

// ==========================================
// 2. 單點求歐拉函數 phi(n) (O(sqrt(n)))
// 適用條件：單次查詢，n 可達 10^12 ~ 10^14
// ==========================================
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

    int n = 100;
    init_phi(n);
    cout << "phi(10) = " << phi[10] << "\n";
    cout << "get_phi(998244353) = " << get_phi(998244353) << "\n";
    return 0;
}
