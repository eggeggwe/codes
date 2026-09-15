#include <bits/stdc++.h>
using namespace std;

// ==========================================
// 線段相交與交點模板 (Segment Intersection)
// ==========================================

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

    // 內積 (Dot Product)
    double operator*(const Point &b) const { return x * b.x + y * b.y; }
    // 外積 (Cross Product, 叉積)
    double operator^(const Point &b) const { return x * b.y - y * b.x; }

    bool operator==(const Point &b) const {
        return sgn(x - b.x) == 0 && sgn(y - b.y) == 0;
    }
};

inline double cross(Point a, Point b, Point o) {
    return (a - o) ^ (b - o);
}

// 點 p 是否在線段 ab 上 (包含端點)
bool on_segment(Point p, Point a, Point b) {
    return sgn((a - p) ^ (b - p)) == 0 && sgn((a - p) * (b - p)) <= 0;
}

// 兩直線交點 (須確保兩直線不平行，即 cross(v, w) != 0)
// 直線表示為 p + t*v 與 q + u*w
Point line_intersection(Point p, Point v, Point q, Point w) {
    return p + v * (((q - p) ^ w) / (v ^ w));
}

// 判斷兩線段 ab 與 cd 是否相交 (包含端點重疊、部分共線)
bool segment_intersection(Point a, Point b, Point c, Point d) {
    // 1. 快速排斥實驗 (Bounding Box 快速過濾)
    if (max(a.x, b.x) < min(c.x, d.x) - EPS || max(c.x, d.x) < min(a.x, b.x) - EPS ||
        max(a.y, b.y) < min(c.y, d.y) - EPS || max(c.y, d.y) < min(a.y, b.y) - EPS)
        return false;

    // 2. 跨立實驗
    double c1 = (b - a) ^ (c - a), c2 = (b - a) ^ (d - a);
    double c3 = (d - c) ^ (a - c), c4 = (d - c) ^ (b - c);

    return sgn(c1) * sgn(c2) <= 0 && sgn(c3) * sgn(c4) <= 0;
}

// 求兩線段交點
// 回傳 pair<bool, Point>：first 為是否有唯一交點，second 為交點座標
pair<bool, Point> get_segment_intersection_point(Point a, Point b, Point c, Point d) {
    if (!segment_intersection(a, b, c, d)) return {false, {0, 0}};

    double denom = (b - a) ^ (d - c);
    if (sgn(denom) == 0) {
        // 兩線段共線或平行，檢查是否僅在端點相碰
        if (a == c || a == d) return {true, a};
        if (b == c || b == d) return {true, b};
        // 若重疊區間大於 1 個點，則無唯一交點
        return {false, {0, 0}};
    }
    // 非平行，有唯一交點
    return {true, line_intersection(a, b - a, c, d - c)};
}

void solve() {
    Point a, b, c, d;
    if (!(cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y >> d.x >> d.y)) return;
    
    if (segment_intersection(a, b, c, d)) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (cin >> t) {
        while (t--) solve();
    }
    return 0;
}