// 線段樹: 動態開點，區間加值、區間求和
// 用途: 值域很大 (如 1e9) 但操作次數不多時，省去預先建樹的空間
// 複雜度: 每次操作新增節點數 O(log V)，V 為值域大小
// 使用方法:
//   DynSeg t; t.init();
//   t.add(l, r, v);   // 區間 [l,r] 加 v
//   t.sum(l, r);      // 查詢區間 [l,r] 和
// 注意: 值域寫死在 add/sum 內的 1e9，需依題目改這兩行的值域上界
struct DynSeg {
    struct Node { ll sum = 0; int lc = 0, rc = 0; };
    struct Tag { ll add = 0; bool has = 0; };
    int tot, root;
    vector<Node> tr;
    vector<Tag> lz;

    void init() {
        tot = root = 0;
        tr.assign(1, Node()); lz.assign(1, Tag());
    }
    int new_node() {
        tr.pb(Node()); lz.pb(Tag());
        return ++tot;
    }
    void pull(int id) {
        tr[id].sum = tr[tr[id].lc].sum + tr[tr[id].rc].sum;
    }
    void apply(int& id, int l, int r, const Tag& t) {
        if (!t.has) return;
        if (!id) id = new_node();
        tr[id].sum += t.add * (r - l + 1);
        lz[id].add += t.add; lz[id].has = 1;
    }
    void push(int id, int l, int r) {
        if (!lz[id].has) return;
        Tag tg = lz[id]; lz[id] = Tag(); // 拷貝防 Realloc
        int mid = l + (r - l) / 2, nxt_l = tr[id].lc, nxt_r = tr[id].rc;
        apply(nxt_l, l, mid, tg); tr[id].lc = nxt_l;
        apply(nxt_r, mid + 1, r, tg); tr[id].rc = nxt_r;
    }
    void upd(int& id, int l, int r, int ql, int qr, const Tag& t) {
        if (!id) id = new_node();
        if (ql <= l && r <= qr) return apply(id, l, r, t);
        push(id, l, r);
        int mid = l + (r - l) / 2;
        if (ql <= mid) { int nxt = tr[id].lc; upd(nxt, l, mid, ql, qr, t); tr[id].lc = nxt; }
        if (qr > mid)  { int nxt = tr[id].rc; upd(nxt, mid + 1, r, ql, qr, t); tr[id].rc = nxt; }
        pull(id);
    }
    ll qry(int id, int l, int r, int ql, int qr) {
        if (!id || r < ql || qr < l) return 0;
        if (ql <= l && r <= qr) return tr[id].sum;
        push(id, l, r);
        int mid = l + (r - l) / 2;
        return qry(tr[id].lc, l, mid, ql, qr) + qry(tr[id].rc, mid + 1, r, ql, qr);
    }
    void add(int ql, int qr, ll v) { upd(root, 1, 1e9, ql, qr, {v, 1}); }
    ll sum(int ql, int qr) { return qry(root, 1, 1e9, ql, qr); }
};