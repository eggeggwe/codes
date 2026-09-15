//他會修改s(中間加@)和z，請注意
void z_value_pal(string &s,int len,vll &z){
    rep(i,0,len+1)s+=' ';
  len=(len<<1)+1;
  z.assign(len,0);
  for(int i=len-1;i>=0;i--)
    s[i]=i&1?s[i>>1]:'@';
  z[0]=1;
  for(int i=1,l=0,r=0;i<len;i++){
    z[i]=i<r?min(z[l+l-i],(ll)r-i):1;
    while(i-z[i]>=0&&i+z[i]<len&&s[i-z[i]]==s[i+z[i]])++z[i];
    if(i+z[i]>r) l=i,r=i+z[i];
} }

template <typename T = string>
struct Manacher {
    int n;
    vector<int> r; // 變形後的回文半徑
    T t;           // 插入分隔符與哨兵後的序列

    // 預設針對 string，若為數值 vector 可自訂 unused 的分隔符與哨兵
    Manacher(const T &s, typename T::value_type sep = '#', 
             typename T::value_type head = '^', typename T::value_type tail = '$') {
        n = s.size();
        // 變形為: ^ # s0 # s1 # ... # sn-1 # $
        t.push_back(head);
        t.push_back(sep);
        for (const auto &c : s) {
            t.push_back(c);
            t.push_back(sep);
        }
        t.push_back(tail);

        int m = t.size();
        r.assign(m, 0);
        int c = 0, max_r = 0; // 當前回文中心與最右邊界 (開區間)

        for (int i = 1; i < m - 1; i++) {
            int i_mirror = 2 * c - i;
            if (i < max_r) {
                r[i] = min(r[i_mirror], max_r - i);
            }
            // 依靠首尾的 head 和 tail 哨兵，完全不需要寫邊界檢查
            while (t[i - r[i]] == t[i + r[i]]) {
                r[i]++;
            }
            if (i + r[i] > max_r) {
                c = i;
                max_r = i + r[i];
            }
        }
    }

    // 1. 取得原字串中最長回文子字串的長度
    int get_max_len() const {
        int max_val = 0;
        for (int x : r) max_val = max(max_val, x - 1);
        return max_val;
    }

    // 2. 取得最長回文子字串在原字串的 [start, len]
    pair<int, int> get_longest_palindrome() const {
        int best_len = 0, best_center = 0;
        for (int i = 1; i < (int)r.size() - 1; i++) {
            if (r[i] - 1 > best_len) {
                best_len = r[i] - 1;
                best_center = i;
            }
        }
        // 原字串中的起始下標
        int start = (best_center - best_len) / 2;
        return {start, best_len};
    }

    // 3. O(1) 判斷原字串區間 [L, R] 是否為回文 (0-indexed, 包含 L 與 R)
    bool is_palindrome(int L, int R) const {
        if (L > R) return false;
        // 原字串中的區間 [L, R] 在變形字串 t 中的中心對應到 L + R + 2
        int center = L + R + 2;
        int len = R - L + 1;
        // 半徑需大於等於 len + 1 (因為包含中心與間隔符)
        return r[center] - 1 >= len;
    }
};