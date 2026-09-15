// 回傳 z 陣列，z[i] 為 s 與 s[i...] 的最長公共前綴 (LCP) 長度
vector<int> z_algorithm(const string &s) {
    int n = s.size();
    vector<int> z(n);
    z[0] = n; // 若題目/場景需要可改為 0

    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) {
            z[i] = min(z[i - l], r - i);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}