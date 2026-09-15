#include <bits/stdc++.h>
using namespace std;

// 支援 string 與 vector<T> 的通用 KMP 前綴函數模板
template <typename T>
struct KMP {
    int m;
    T pat;
    vector<int> pi;

    void init(const T &pattern) {
        pat = pattern;
        m = pat.size();
        pi.assign(m, 0);
        for (int i = 1; i < m; i++) {
            int j = pi[i - 1];
            while (j > 0 && pat[i] != pat[j]) j = pi[j - 1];
            if (pat[i] == pat[j]) j++;
            pi[i] = j;
        }
    }

    // 回傳在 text 中的所有匹配起點下標
    int count_matches(const T &text) {
        int n = text.size();
        int ans = 0;
        for (int i = 0, j = 0; i < n; i++) {
            while (j > 0 && text[i] != pat[j]) j = pi[j - 1];
            if (text[i] == pat[j]) j++;
            if (j == m) {
                ans++;
                j = pi[j - 1];
            }
        }
        return ans;
    }
}

int main() {
    vector<long long> diff_a(n - 1), diff_b(w - 1);
    KMP<vector<long long>> solver;
    solver.init(diff_b);
    cout << solver.count_matches(diff_a) << "\n";

    return 0;
}