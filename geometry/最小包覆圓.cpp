#include <bits/stdc++.h>
using namespace std;

// ==========================================================
// 最小包覆圓 (Minimum Enclosing Circle - Welzl 隨機增量法)
// 時間複雜度：期望 O(N)
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

    double operator*(const Point &b) const { return x * b.x + y * b.y; }
    double operator^(const Point &b) const { return x * b.y - y * b.x; }

    double dist2() const { return x * x + y * y; }
    double len() const { return hypot(x, y); }
};

Point rot90(Point p) { return {-p.y, p.x}; }

// 兩直線交點
Point line_intersection(Point p, Point v, Point q, Point w) {
    return p + v * (((q - p) ^ w) / (v ^ w));
}

struct Circle {
    Point o;
    double r;

    bool contains(Point p) const {
        return (p - o).len() <= r + EPS;
    }
};

// 兩點外接圓 (直徑)
Circle circle_2pts(Point a, Point b) {
    return {(a + b) / 2.0, (b - a).len() / 2.0};
}

// 三點外接圓
Circle circumcircle(Point a, Point b, Point c) {
    double cp = (b - a) ^ (c - a);
    if (sgn(cp) == 0) {
        // 三點共線特例：取最遠兩點為直徑
        double d1 = (b - a).len(), d2 = (c - a).len(), d3 = (c - b).len();
        if (d1 >= d2 && d1 >= d3) return circle_2pts(a, b);
        if (d2 >= d1 && d2 >= d3) return circle_2pts(a, c);
        return circle_2pts(b, c);
    }
    // 中垂線交點
    Point m1 = (a + b) / 2.0, v1 = rot90(b - a);
    Point m2 = (a + c) / 2.0, v2 = rot90(c - a);
    Point o = line_intersection(m1, v1, m2, v2);
    return {o, (a - o).len()};
}

/**
 * @brief 隨機增量法求點集的最小包覆圓
 * @return Circle 圓心座標及半徑
 */
Circle min_enclosing_circle(vector<Point> p) {
    int n = p.size();
    if (n == 0) return {{0, 0}, 0};
    if (n == 1) return {p[0], 0};

    // 關鍵：隨機打亂以保證期望 O(N) 複雜度
    mt19937 rng(1337);
    shuffle(p.begin(), p.end(), rng);

    Circle c = circle_2pts(p[0], p[1]);

    for (int i = 2; i < n; i++) {
        if (!c.contains(p[i])) {
            c = circle_2pts(p[0], p[i]);
            for (int j = 0; j < i; j++) {
                if (!c.contains(p[j])) {
                    c = circle_2pts(p[i], p[j]);
                    for (int k = 0; k < j; k++) {
                        if (!c.contains(p[k])) {
                            c = circumcircle(p[i], p[j], p[k]);
                        }
                    }
                }
            }
        }
    }
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Point> p(n);
        for (int i = 0; i < n; i++) {
            cin >> p[i].x >> p[i].y;
        }

        Circle ans = min_enclosing_circle(p);
        cout << fixed << setprecision(6);
        cout << ans.o.x << " " << ans.o.y << " " << ans.r << "\n";
    }

    return 0;
}