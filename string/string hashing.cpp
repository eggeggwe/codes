struct HashString {
    int n;
    static const long long MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;
    static const long long B1 = 313, B2 = 317;
 
    vector<long long> h1, h2, p1, p2;
 
    void init(const string &s) {
        n = s.size();
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);
        p1.assign(n + 1, 1);
        p2.assign(n + 1, 1);
 
        for (int i = 0; i < n; i++) {
            long long c = (unsigned char)s[i];
            h1[i + 1] = (h1[i] * B1 + c) % MOD1;
            h2[i + 1] = (h2[i] * B2 + c) % MOD2;
            p1[i + 1] = (p1[i] * B1) % MOD1;
            p2[i + 1] = (p2[i] * B2) % MOD2;
        }
    }
 
    pll query(int l, int r) const {
        if (r < l) return {0, 0};
        long long val1 = (h1[r + 1] - h1[l] * p1[r - l + 1]) % MOD1;
        if (val1 < 0) val1 += MOD1;
 
        long long val2 = (h2[r + 1] - h2[l] * p2[r - l + 1]) % MOD2;
        if (val2 < 0) val2 += MOD2;
 
        return {val1, val2};
    }
};
 