struct AC_auto {
    struct node {
        int lf = 0;       // fail link
        int next[26];     // 字典樹邊 / Trie 圖轉移
        node() { memset(next, 0, sizeof(next)); }
    };

    vector<node> trie;
    vector<int> exist;    // 記錄節點是否為單詞結尾（或包含幾個單詞）
    vector<int> bfs;      // 同時作為拓撲排序順序

    AC_auto() {
        trie.emplace_back(); // 根節點 0
        exist.assign(1, 0);  // 同步初始化
    }

    int insert(const string &s) {
        int p = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (!trie[p].next[c]) {
                trie[p].next[c] = trie.size();
                trie.emplace_back();
                exist.push_back(0);
            }
            p = trie[p].next[c];
        }
        exist[p]++;
        return p; // 回傳該單詞在 Trie 中的結尾節點編號
    }

    void build() {
        bfs.clear();
        queue<int> q;
        // 將深度為 1 的節點加入隊列，其 fail 預設為 0
        for (int c = 0; c < 26; c++) {
            if (trie[0].next[c]) {
                q.push(trie[0].next[c]);
            }
        }

        while (!q.empty()) {
            int now = q.front();
            q.pop();
            bfs.push_back(now);

            for (int c = 0; c < 26; c++) {
                int &next = trie[now].next[c];
                int fail_next = trie[trie[now].lf].next[c];
                if (next) {
                    trie[next].lf = fail_next;
                    q.push(next);
                } else {
                    next = fail_next; // Trie 圖路徑壓縮
                }
            }
        }
    }

    // 拓撲優化查詢：統計每個節點被匹配到的次數，O(|T| + |Trie|)
    vector<int> query_freq(const string &t) {
        vector<int> cnt(trie.size(), 0);
        int p = 0;
        for (char ch : t) {
            p = trie[p].next[ch - 'a'];
            cnt[p]++;
        }
        // 依照 BFS 反向（拓撲逆序）將貢獻推回 fail 祖先
        for (int i = (int)bfs.size() - 1; i >= 0; i--) {
            int u = bfs[i];
            cnt[trie[u].lf] += cnt[u];
        }
        return cnt;
    }
};