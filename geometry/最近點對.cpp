#include <bits/stdc++.h>
using namespace std;

// ==========================================================
// 最近點對 (Closest Pair of Points - 掃描線 O(N log N))
// 回傳最小距離的平方 (避免浮點數誤差)
// ==========================================================

struct Point {
    long long x, y;
    int id;

    bool operator<(const Point &b) const {
        if (x != b.x) return x < b.x;
        return y < b.y;
    }
};

struct CmpY {
    bool operator()(const Point &a, const Point &b) const {
        if (a.y != b.y) return a.y < b.y;
        return a.x < b.x;
    }
};

inline long long dist2(const Point &a, const Point &b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

long long closest_pair(vector<Point> p) {
    int n = p.size();
    if (n <= 1) return 0;

    // 1. 先按 X 座標排序
    sort(p.begin(), p.end());

    // 2. 檢查是否有重複點 (若有，最近距離為 0)
    for (int i = 0; i < n - 1; i++) {
        if (p[i].x == p[i + 1].x && p[i].y == p[i + 1].y) {
            return 0;
        }
    }

    long long d = dist2(p[0], p[1]);
    set<Point, CmpY> active_set; // 依 Y 座標排序的有效點集合
    int l = 0;

    for (int i = 0; i < n; i++) {
        long long cur_d = ceil(sqrt(d));

        // 移除 X 距離已經 >= sqrt(d) 的點 (維持滑動視窗)
        while (l < i && p[i].x - p[l].x >= cur_d) {
            active_set.erase(p[l]);
            l++;
        }

        // 在 Y 座標範圍 [p[i].y - cur_d, p[i].y + cur_d] 內尋找候選點
        // 幾何上證明該範圍內至多只會有 6 個點
        Point low = {-4000000000000000000LL, p[i].y - cur_d, -1};
        auto it = active_set.lower_bound(low);

        while (it != active_set.end() && it->y <= p[i].y + cur_d) {
            d = min(d, dist2(p[i], *it));
            cur_d = ceil(sqrt(d)); // 更新半徑，及時剪枝
            it++;
        }

        active_set.insert(p[i]);
    }

    return d;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (cin >> n) {
        vector<Point> p(n);
        for (int i = 0; i < n; i++) {
            cin >> p[i].x >> p[i].y;
            p[i].id = i;
        }

        long long min_d2 = closest_pair(p);
        cout << min_d2 << "\n";
        // 若題目要求開根號距離：
        // cout << fixed << setprecision(6) << sqrt((double)min_d2) << "\n";
    }

    return 0;
}