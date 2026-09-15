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