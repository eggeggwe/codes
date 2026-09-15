#include <bits/stdc++.h>
using namespace std;

// =========================================================================
// 2D 計算幾何基礎全餐 + 凸包 + 旋轉卡尺同捆包 (Geometry Toolkit Bundle)
// =========================================================================

const double EPS = 1e-9;
const double PI = acos(-1.0);

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

    // 內積與外積
    double operator*(const Point &b) const { return x * b.x + y * b.y; } // dot
    double operator^(const Point &b) const { return x * b.y - y * b.x; } // cross

    bool operator<(const Point &b) const {
        if (sgn(x - b.x) != 0) return x < b.x;
        return y < b.y;
    }
    bool operator==(const Point &b) const {
        return sgn(x - b.x) == 0 && sgn(y - b.y) == 0;
    }

    double dist2() const { return x * x + y * y; }
    double len() const { return hypot(x, y); }
    Point unit() const { return *this / len(); }
    Point rot90() const { return {-y, x}; } // 逆時針旋轉 90 度
    Point rot(double rad) const { // 逆時針旋轉 rad 弧度
        return {x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad)};
    }
};

// 以 o 為參考點計算向量 oa 與 ob 的外積 (轉向判斷: > 0 為左轉/逆時針)
inline double cross(Point a, Point b, Point o) {
    return (a - o) ^ (b - o);
}

// 點積
inline double dot(Point a, Point b, Point o) {
    return (a - o) * (b - o);
}

// --- 投影與對稱 ---

// 點 p 在直線 ab 上的投影點
Point project(Point p, Point a, Point b) {
    Point v = b - a;
    return a + v * (((p - a) * v) / v.dist2());
}

// 點 p 關於直線 ab 的對稱點
Point reflect(Point p, Point a, Point b) {
    return project(p, a, b) * 2.0 - p;
}

// 點 p 到直線 ab 的最短距離
double dist_to_line(Point p, Point a, Point b) {
    return fabs((b - a) ^ (p - a)) / (b - a).len();
}

// 點 p 到線段 ab 的最短距離
double dist_to_segment(Point p, Point a, Point b) {
    if (sgn(dot(p, b, a)) < 0) return (p - a).len();
    if (sgn(dot(p, a, b)) < 0) return (p - b).len();
    return dist_to_line(p, a, b);
}

// --- 直線與線段相交 ---

// 兩直線交點 (直線以 p + t*v 與 q + u*w 表示，需確保 cross(v, w) != 0)
Point line_intersection(Point p, Point v, Point q, Point w) {
    return p + v * (((q - p) ^ w) / (v ^ w));
}

// 點 p 是否在線段 ab 上 (含端點)
bool on_segment(Point p, Point a, Point b) {
    return sgn((a - p) ^ (b - p)) == 0 && sgn((a - p) * (b - p)) <= 0;
}

// --- 多邊形相關 ---

// 多邊形有向面積 (逆時針為正，順時針為負)
double polygon_area(const vector<Point> &poly) {
    double area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        area += poly[i] ^ poly[(i + 1) % n];
    }
    return area / 2.0;
}

// 多邊形重心 (Centroid)
Point polygon_centroid(const vector<Point> &poly) {
    Point c(0, 0);
    double scale = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        Point p = poly[i], q = poly[(i + 1) % n];
        double cross_pq = p ^ q;
        c = c + (p + q) * cross_pq;
        scale += cross_pq;
    }
    return c / (3.0 * scale);
}

// --- 凸包 (Convex Hull) ---

/**
 * @brief Andrew's Monotone Chain 凸包演算法
 * @param pts 任意點集
 * @return 逆時針排列且不含重複起點的凸包頂點 (嚴格凸頂點)
 */
vector<Point> convex_hull(vector<Point> pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = pts.size();
    if (n <= 2) return pts;

    vector<Point> h(2 * n);
    int k = 0;

    // 下凸包 (若要保留凸包邊上的共線點，將 <= 0 改為 < 0)
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(h[k - 1], pts[i], h[k - 2]) <= 0) k--;
        h[k++] = pts[i];
    }

    // 上凸包
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross(h[k - 1], pts[i], h[k - 2]) <= 0) k--;
        h[k++] = pts[i];
    }

    h.resize(k - 1); // 移除尾端重複的起點
    return h;
}

// --- 旋轉卡尺求直徑 ---

double rotating_calipers_diameter(const vector<Point> &hull) {
    int n = hull.size();
    if (n < 2) return 0.0;
    if (n == 2) return (hull[0] - hull[1]).len();

    int j = 1;
    double max_dist_sq = 0.0;

    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;
        while (((hull[ni] - hull[i]) ^ (hull[(j + 1) % n] - hull[j])) > EPS) {
            max_dist_sq = max(max_dist_sq, (hull[i] - hull[j]).dist2());
            j = (j + 1) % n;
        }
        max_dist_sq = max(max_dist_sq, (hull[i] - hull[j]).dist2());
        max_dist_sq = max(max_dist_sq, (hull[ni] - hull[j]).dist2());
        max_dist_sq = max(max_dist_sq, (hull[i] - hull[(j + 1) % n]).dist2());
        max_dist_sq = max(max_dist_sq, (hull[ni] - hull[(j + 1) % n]).dist2());
    }

    return sqrt(max_dist_sq);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (cin >> n) {
        vector<Point> p(n);
        for (int i = 0; i < n; i++) {
            cin >> p[i].x >> p[i].y;
        }

        vector<Point> hull = convex_hull(p);
        cout << "凸包頂點數: " << hull.size() << "\n";
        cout << fixed << setprecision(6);
        cout << "凸包面積: " << polygon_area(hull) << "\n";
        cout << "凸包直徑: " << rotating_calipers_diameter(hull) << "\n";
    }

    return 0;
}
