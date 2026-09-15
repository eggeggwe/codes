#include <bits/stdc++.h>
using namespace std;

// ==========================================================
// 旋轉卡尺求凸包直徑 (最遠點對距離) - Rotating Calipers
// 時間複雜度：凸包 O(N log N) + 卡尺 O(N)
// ==========================================================

const double EPS = 1e-9;

inline int sgn(double x) {
    if (fabs(x) < EPS) return 0;
    return x > 0 ? 1 : -1;
}

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    Point operator-(const Point &b) const { return {x - b.x, y - b.y}; }
    bool operator<(const Point &b) const { return tie(x, y) < tie(b.x, b.y); }
    bool operator==(const Point &b) const { return sgn(x - b.x) == 0 && sgn(y - b.y) == 0; }

    double dist2() const { return x * x + y * y; }
    double len() const { return hypot(x, y); }
};

inline double cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
inline double cross(Point a, Point b, Point o) { return cross(a - o, b - o); }

// Andrew's Monotone Chain 凸包演算法
// 回傳嚴格頂點 (不含邊上共線點，若需含共線點改 <= 0 為 < 0)
vector<Point> convex_hull(vector<Point> pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = pts.size();
    if (n <= 2) return pts;

    vector<Point> h(2 * n);
    int k = 0;

    // 下凸包
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

/**
 * @brief 旋轉卡尺計算凸包直徑 (最遠點對距離)
 * @param hull 逆時針排列的凸包頂點
 * @return 最遠點對距離 (可依題目需求回傳平方 dist2 避免開根號)
 */
double rotating_calipers_diameter(const vector<Point> &hull) {
    int n = hull.size();
    if (n < 2) return 0.0;
    if (n == 2) return (hull[0] - hull[1]).len();

    int j = 1;
    double max_dist_sq = 0.0;

    // 對凸包每條邊 i -> i+1，推進卡尺頂點 j
    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;
        
        // 比較三角形 (i, ni, j+1) 與 (i, ni, j) 的高/面積
        while (cross(hull[ni] - hull[i], hull[(j + 1) % n] - hull[j]) > EPS) {
            max_dist_sq = max(max_dist_sq, (hull[i] - hull[j]).dist2());
            j = (j + 1) % n;
        }

        // 當前邊對應的對蹠點為 j 與 j+1
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
        for (int i = 0; i < n; ++i) {
            cin >> p[i].x >> p[i].y;
        }

        vector<Point> hull = convex_hull(p);
        double diameter = rotating_calipers_diameter(hull);

        cout << fixed << setprecision(10) << diameter << "\n";
    }

    return 0;
}
