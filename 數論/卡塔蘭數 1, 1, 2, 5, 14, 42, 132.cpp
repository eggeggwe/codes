#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// =========================================================================
// 卡塔蘭數 (Catalan Number)
// 前幾項：C_0=1, C_1=1, C_2=2, C_3=5, C_4=14, C_5=42, C_6=132, C_7=429...
//
// 常見應用：
// 1. n 對合法括號的組合數
// 2. n+1 個葉節點的不同二元樹結構數
// 3. 凸 n+2 邊形被不相交對角線切成三角形的方案數
// 4. 從 (0,0) 走到 (n,n) 不穿過對角線 y=x 的路徑數
// 5. 出棧序列數 (入棧序列為 1, 2, ..., n)
//
// 核心公式：
// C_n = 1 / (n + 1) * C(2n, n) = C(2n, n) - C(2n, n - 1)
// 遞推式：C_n = (4n - 2) / (n + 1) * C_{n-1}
// =========================================================================

// 方法一：模質數 P 下，O(n) 線性遞推預處理 (最常用於 ICPC)
const int MAXN = 200005;
ll catalan[MAXN];
ll inv[MAXN];

void init_catalan(int n, ll p) {
    catalan[0] = catalan[1] = 1;
    inv[1] = 1;
    // 線性預處理分母逆元
    for (int i = 2; i <= n + 1; ++i) {
        inv[i] = 1LL * (p - p / i) * inv[p % i] % p;
    }
    // C_n = C_{n-1} * (4n - 2) / (n + 1)
    for (int i = 2; i <= n; ++i) {
        catalan[i] = catalan[i - 1] * (4 * i - 2) % p * inv[i + 1] % p;
    }
}

// 方法二：無取模時精確計算 (注意：n <= 35，因為 C_36 > 2^63 - 1 會超出 unsigned long long)
unsigned long long get_catalan_small(int n) {
    if (n <= 1) return 1;
    unsigned long long res = 1;
    for (int i = 1; i <= n; ++i) {
        res = res * (4 * i - 2) / (i + 1);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    ll p = 1e9 + 7;
    if (cin >> n) {
        if (n <= 35) {
            cout << "精確小值 C_" << n << " = " << get_catalan_small(n) << "\n";
        }
        init_catalan(n, p);
        cout << "C_" << n << " mod (1e9+7) = " << catalan[n] << "\n";
    }
    return 0;
}
