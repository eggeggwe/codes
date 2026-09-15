#include <bits/stdc++.h>
using namespace std;

// 線性篩法（歐拉篩法）求質數
// 時間複雜度：O(n)
const int MAXN = 10000005; // 依題目記憶體上限與範圍調整 (1e7 約耗 10MB ~ 40MB)
bitset<MAXN> is_prime;
vector<int> primes;
int minp[MAXN]; // minp[i] 儲存 i 的最小質因數 (Smallest Prime Factor, SPF)

void sieve(int n) {
    is_prime.set();
    is_prime[0] = is_prime[1] = 0;

    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
            minp[i] = i;
        }
        for (int p : primes) {
            if (1LL * i * p > n) break;
            is_prime[i * p] = 0;
            minp[i * p] = p;
            if (i % p == 0) break; // 保證每個合數只被最小質因數篩去一次
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    if (cin >> q) {
        sieve(q);
        // 注意：千萬不可使用 endl，否則輸出量大時會嚴重 TLE
        for (int p : primes) {
            cout << p << "\n";
        }
    }
    return 0;
}
