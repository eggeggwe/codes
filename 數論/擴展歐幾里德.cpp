#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ==========================================
// 擴展歐幾里得演算法 (Extended Euclidean Algorithm)
// 求解 ax + by = gcd(a, b) 的一組整數解 (x, y)
// 回傳 gcd(a, b)
// ==========================================
ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    ll g = exgcd(b, a % b, y, x);
    y -= (a / b) * x;
    return g;
}

// ==========================================
// 一次不定方程 (Linear Diophantine Equation)
// 求解 ax + by = c
// 若有解，將 (x, y) 調整為 x 的最小非負整數解，並回傳 true
// 若無解，回傳 false
// ==========================================
bool solve_diophantine(ll a, ll b, ll c, ll &x, ll &y, ll &g) {
    g = exgcd(a, b, x, y);
    // 貝祖定理：c 必須是 gcd(a, b) 的倍數才存在整數解
    if (c % g != 0) return false;

    ll k = c / g;
    x *= k;
    y *= k;

    // 通解公式：
    // x' = x + t * (b / g)
    // y' = y - t * (a / g)
    if (b != 0) {
        ll step_x = abs(b / g);
        // 將 x 調整至 [0, step_x - 1] 的最小非負整數解
        x = (x % step_x + step_x) % step_x;
        y = (c - a * x) / b;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll a = 3, b = 2, c = 5;
    ll x, y, g;
    if (solve_diophantine(a, b, c, x, y, g)) {
        cout << "gcd(" << a << ", " << b << ") = " << g << "\n";
        cout << a << "*(" << x << ") + " << b << "*(" << y << ") = " << c << "\n";
        cout << "x 的最小非負整數解: " << x << "\n";
    } else {
        cout << "無整數解\n";
    }
    return 0;
}
