#include <bits/stdc++.h>
using namespace std;

// 浮點數高斯消去法
// 求解 n 元線性方程組 A * x = B
const int N = 110;
const double eps = 1e-8;
int n;
double a[N][N]; // a[i][0 ~ n-1] 為係數，a[i][n] 為等號右側常數

// 回傳值：
// 0: 唯一解 (解存於 a[i][n])
// 1: 無限多解
// 2: 無解
int gauss() {
    int r = 0;
    for (int c = 0; c < n; c++) {
        int t = r;
        for (int i = r; i < n; i++) {
            if (fabs(a[i][c]) > fabs(a[t][c])) {
                t = i;
            }
        }

        if (fabs(a[t][c]) < eps) continue; // 此行全為 0，自由變數

        // 將主元所在列交換至第 r 列
        for (int i = c; i <= n; i++) swap(a[t][i], a[r][i]);
        // 將主元係數歸一化為 1
        for (int i = n; i >= c; i--) a[r][i] /= a[r][c];

        // 消去其餘所有列（化簡為 Jordan 標準型）
        for (int i = 0; i < n; i++) {
            if (i != r && fabs(a[i][c]) > eps) {
                double factor = a[i][c];
                for (int j = n; j >= c; j--) {
                    a[i][j] -= a[r][j] * factor;
                }
            }
        }
        r++;
    }

    if (r < n) {
        for (int i = r; i < n; i++) {
            if (fabs(a[i][n]) > eps) return 2; // 0 == 非 0 -> 無解
        }
        return 1; // 無限多解
    }

    return 0; // 唯一解
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++) {
            cin >> a[i][j];
        }
    }

    int res = gauss();
    if (res == 0) {
        for (int i = 0; i < n; i++) {
            // 避免輸出 -0.00
            if (fabs(a[i][n]) < eps) a[i][n] = 0;
            cout << fixed << setprecision(2) << a[i][n] << "\n";
        }
    } else if (res == 1) {
        cout << "Infinite group solutions\n";
    } else {
        cout << "No solution\n";
    }

    return 0;
}
