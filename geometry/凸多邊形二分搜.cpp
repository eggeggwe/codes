#include <bits/stdc++.h>
using namespace std;

// ==========================================================
// 凸多邊形 O(log N) 包含測試與二分搜模板
// 要求多邊形為嚴格凸多邊形，頂點以逆時針 (CCW) 順序排列
// ==========================================================

const double EPS = 1e-9;

inline int sgn(double x) {
    if (fabs(x) < EPS) return 0;
    return x > 0 ? 1 : -1;
}

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    Point operator+(const Point &b) const { return {x + b.x, y + b.y}; }
    Point operator-(const Point &b) const { return {x - b.x, y - b.y}; }
    Point operator*(double s) const { return {x * s, y * s}; }
    Point operator/(double s) const { return {x / s, y / s}; }

    double operator*(const Point &b) const { return x * b.x + y * b.y; } // 內積
    double operator^(const Point &b) const { return x * b.y - y * b.x; } // 外積
};

// 點 p 是否在線段 ab 上 (含端點)
bool on_segment(Point p, Point a, Point b) {
    return sgn((a - p) ^ (b - p)) == 0 && sgn((a - p) * (b - p)) <= 0;
}

/**
 * @brief O(log N) 判斷點 q 與凸多邊形 p 的關係
 * @param q 待查詢點
 * @param p 逆時針排列且不含重複頂點的凸多邊形 (N >= 3)
 * @return 1: 嚴格內部, 0: 邊界上 (含頂點), -1: 外部
 */
int point_in_convex_polygon(Point q, const vector<Point> &p) {
    int n = p.size();
    if (n < 3) {
        if (n == 1) return (sgn(q.x - p[0].x) == 0 && sgn(q.y - p[0].y) == 0) ? 0 : -1;
        if (n == 2) return on_segment(q, p[0], p[1]) ? 0 : -1;
        return -1;
    }

    // 1. 檢查是否超出由 P0 出發的兩條射線 P0->P1 與 P0->P(n-1) 的夾角範圍
    double cp1 = (p[1] - p[0]) ^ (q - p[0]);
    double cpn = (p[n - 1] - p[0]) ^ (q - p[0]);

    if (sgn(cp1) < 0 || sgn(cpn) > 0) return -1; // 在夾角之外，必在多邊形外

    // 2. 邊界特判：是否在 P0-P1 或 P0-P(n-1) 這兩條邊上
    if (sgn(cp1) == 0) return on_segment(q, p[0], p[1]) ? 0 : -1;
    if (sgn(cpn) == 0) return on_segment(q, p[0], p[n - 1]) ? 0 : -1;

    // 3. 二分搜尋 q 落在由 P0 放射出的哪一個三角形 P0 - P[l] - P[l+1] 中
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = (l + r) / 2;
        if (sgn((p[mid] - p[0]) ^ (q - p[0])) >= 0) {
            l = mid;
        } else {
            r = mid;
        }
    }

    // 4. 判定 q 相對於底邊 P[l] -> P[l+1] 的位置
    double cp = (p[l + 1] - p[l]) ^ (q - p[l]);
    int s = sgn(cp);
    if (s > 0) return 1;  // 嚴格內部
    if (s == 0) return 0; // 在邊界 P[l]-P[l+1] 上
    return -1;            // 外部
}

/**
 * @brief 直線切割凸多邊形 (Convex Cut)
 * @param poly 原始凸多邊形 (逆時針)
 * @param a, b 有向直線 a -> b，保留其左側部分
 * @return 切割後的新凸多邊形
 */
vector<Point> convex_cut(const vector<Point> &poly, Point a, Point b) {
    vector<Point> res;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        Point p1 = poly[i], p2 = poly[(i + 1) % n];
        double c1 = (b - a) ^ (p1 - a);
        double c2 = (b - a) ^ (p2 - a);

        if (sgn(c1) >= 0) res.push_back(p1); // p1 在直線左側或線上

        if (sgn(c1) * sgn(c2) < 0) { // 邊 p1-p2 與直線嚴格相交
            Point v1 = p2 - p1, v2 = b - a;
            Point ip = p1 + v1 * (((a - p1) ^ v2) / (v1 ^ v2));
            res.push_back(ip);
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (cin >> n >> q) {
        vector<Point> poly(n);
        for (int i = 0; i < n; i++) {
            cin >> poly[i].x >> poly[i].y;
        }

        while (q--) {
            Point p;
            cin >> p.x >> p.y;
            int ans = point_in_convex_polygon(p, poly);
            if (ans == 1) {
                cout << "INSIDE\n";
            } else if (ans == 0) {
                cout << "ON_BOUNDARY\n";
            } else {
                cout << "OUTSIDE\n";
            }
        }
    }
    return 0;
}
