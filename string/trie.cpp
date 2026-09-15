struct Trie {
    struct Node {
        int nxt[26];
        int cnt = 0; // 該節點作為結尾的單詞數量
        Node() { memset(nxt, 0, sizeof(nxt)); }
    };

    vector<Node> tree;

    Trie() {
        tree.emplace_back(); // 0 號根節點
    }

    void clear() {
        tree.clear();
        tree.emplace_back();
    }

    void insert(const string &s) {
        int u = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (!tree[u].nxt[idx]) {
                tree[u].nxt[idx] = tree.size();
                tree.emplace_back();
            }
            u = tree[u].nxt[idx];
        }
        tree[u].cnt++;
    }
};