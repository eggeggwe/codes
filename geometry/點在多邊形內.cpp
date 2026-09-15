#include <bits/stdc++.h>
using namespace std;

// ==========================================================
// 點在多邊形內判定 (Point in Polygon - 射線法 O(N))
// 適用於任意簡單多邊形 (不自交即可，凸凹皆可)
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
    double operator*(const Point &b) const { return x * b.x + y * b.y; } // 內積
    double operator^(const Point &b) const { return x * b.y - y * b.x; } // 外積
};

// 檢查點 p 是否在線段 ab 上 (含端點)
bool on_segment(Point p, Point a, Point b) {
    return sgn((a - p) ^ (b - p)) == 0 && sgn((a - p) * (b - p)) <= 0;
}

/**
 * @brief 判定點 p 與多邊形 poly 的位置關係 (O(N) 射線法)
 * @return 0: 邊界上 (BOUNDARY), 1: 內部 (INSIDE), -1: 外部 (OUTSIDE)
 */
int point_in_polygon(Point p, const vector<Point> &poly) {
    int n = poly.size();
    bool inside = false;

    for (int i = 0; i < n; i++) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];

        // 1. 先判斷是否恰好在多邊形邊界上 (包含頂點)
        if (on_segment(p, a, b)) {
            return 0; // BOUNDARY
        }

        // 2. 射線法：向 +X 方向發射射線
        // 為避免頂點重合問題，規範邊為下閉上開區間 [min_y, max_y)
        if (a.y > b.y) swap(a, b);

        if (a.y <= p.y && p.y < b.y) {
            // 計算向右射線是否穿過線段 ab
            // 即檢查 p 是否位於有向線段 a -> b 的左側 (外積 > 0)
            if (((b - a) ^ (p - a)) > 0) {
                inside = !inside;
            }
        }
    }

    return inside ? 1 : -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (cin >> n >> m) {
        vector<Point> poly(n);
        for (int i = 0; i < n; i++) {
            cin >> poly[i].x >> poly[i].y;
        }

        while (m--) {
            Point q;
            cin >> q.x >> q.y;
            int res = point_in_polygon(q, poly);
            if (res == 0) {
                cout << "BOUNDARY\n";
            } else if (res == 1) {
                cout << "INSIDE\n";
            } else {
                cout << "OUTSIDE\n";
            }
        }
    }
    return 0;
}