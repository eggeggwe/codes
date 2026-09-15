#include <bits/stdc++.h>
using namespace std;

// ==========================================================
// 半平面交 (Half-Plane Intersection - S&I 演算法)
// 時間複雜度：O(N log N)
// ==========================================================

const double EPS = 1e-9;
const double INF = 1e9;

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
};

// 有向直線：向量 v 方向的左側為半平面可行域
struct Line {
    Point p, v;
    double ang;

    Line() {}
    Line(Point p, Point v) : p(p), v(v) {
        ang = atan2(v.y, v.x);
    }

    // 依極角排序；若極角相同，保留靠左側（更緊）的線
    bool operator<(const Line &b) const {
        if (sgn(ang - b.ang) != 0) return ang < b.ang;
        return (v ^ (b.p - p)) > EPS;
    }
};

// 點 p 是否在有向直線 L 的左側 (含邊界)
inline bool on_left(const Line &L, Point p) {
    return (L.v ^ (p - L.p)) >= -EPS;
}

// 兩直線交點
Point line_intersection(const Line &a, const Line &b) {
    Point u = a.p - b.p;
    double t = (b.v ^ u) / (a.v ^ b.v);
    return a.p + a.v * t;
}

// 加入邊界框 [-L, L] x [-L, L]，防止無界區域
void add_bounding_box(vector<Line> &lines, double L = 1e9) {
    lines.push_back(Line({-L, -L}, {2 * L, 0}));
    lines.push_back(Line({L, -L}, {0, 2 * L}));
    lines.push_back(Line({L, L}, {-2 * L, 0}));
    lines.push_back(Line({-L, L}, {0, -2 * L}));
}

/**
 * @brief 計算多個半平面的交集
 * @param lines 半平面集合 (以有向直線的左側為可行域)
 * @return 凸多邊形頂點序列 (逆時針排列)；若交集為空或退化則回傳空 vector
 */
vector<Point> half_plane_intersection(vector<Line> lines) {
    // 1. 依極角排序
    sort(lines.begin(), lines.end());

    // 2. 角度相同時，只保留最左側的一條線
    vector<Line> l;
    for (size_t i = 0; i < lines.size(); i++) {
        if (i > 0 && sgn(lines[i].ang - lines[i - 1].ang) == 0) continue;
        l.push_back(lines[i]);
    }

    int n = l.size();
    vector<Point> pts(n);
    vector<Line> q(n);
    int head = 0, tail = 0;

    // 3. 雙端佇列維護凸多邊形
    for (int i = 0; i < n; i++) {
        while (tail - head >= 2 && !on_left(l[i], pts[tail - 1])) tail--;
        while (tail - head >= 2 && !on_left(l[i], pts[head + 1])) head++;

        q[tail++] = l[i];
        if (tail - head >= 2) {
            pts[tail - 1] = line_intersection(q[tail - 2], q[tail - 1]);
        }
    }

    // 4. 清理首尾
    while (tail - head >= 2 && !on_left(q[head], pts[tail - 1])) tail--;
    while (tail - head >= 2 && !on_left(q[tail - 1], pts[head + 1])) head++;

    if (tail - head < 3) return {};

    // 5. 收集交點
    vector<Point> poly;
    for (int i = head + 1; i < tail; i++) {
        poly.push_back(pts[i]);
    }
    poly.push_back(line_intersection(q[tail - 1], q[head]));

    return poly;
}

// 計算多邊形面積
double polygon_area(const vector<Point> &poly) {
    double area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        area += poly[i] ^ poly[(i + 1) % n];
    }
    return fabs(area) / 2.0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (cin >> n) {
        vector<Line> lines;
        // 若半平面可能無界，建議加入 bounding box：
        // add_bounding_box(lines, 1e6);

        for (int i = 0; i < n; i++) {
            Point a, b;
            cin >> a.x >> a.y >> b.x >> b.y;
            // a -> b 方向的左側為半平面
            lines.push_back(Line(a, b - a));
        }

        vector<Point> poly = half_plane_intersection(lines);
        cout << fixed << setprecision(6);
        cout << "交集凸多邊形面積: " << polygon_area(poly) << "\n";
    }

    return 0;
}
