/*有n个元素，第i个元素有ai,bi,ci三个属性，设f(i)表示满足aj≤ai &nbsp; 且bj≤bi且cj≤ci且j不等於i的j的数量。
对于d∈[0, n)，求f(i)=d的数量。
输入格式
----
第一行两个整数n, k，表示元素数量和最大属性值。

接下来n行，每行三个整数ai,bi,ci，分别表示三个属性值。
*/
#include <iostream>
#include <algorithm>

using namespace std;

const int MAXN = 1e5 + 5;
const int MAXK = 2e5;

int n, k, cnt[MAXN];

struct Data{
    int x, y, z;
    
    int operator < (const Data &o) const {
        return x != o.x ? (x < o.x) : (y != o.y ? (y < o.y) : (z < o.z));
    }
    
    int operator == (const Data &o) const {
        return x == o.x && y == o.y && z == o.z;
    }
}data[MAXN];

struct Seg{
    struct Node{
        int val;
        Node *ch[2];
        
        Node(int val = 0) : val(val) {
            ch[0] = ch[1] = NULL;
        }
    };
    
    Node *rt;
    
    Seg() {
        rt = NULL;
    }
    
    void Modify(Node *&now, int pos, int val = 1, int nl = 1, int nr = MAXK) {
        if (!now) now = new Node();
        if (nl == nr) {
            now->val += val;
            return;
        }
        int mid = nl + nr >> 1;
        if (pos <= mid) Modify(now->ch[0], pos, val, nl, mid);
        else Modify(now->ch[1], pos, val, mid + 1, nr);
        now->val = (now->ch[0] ? now->ch[0]->val : 0) + (now->ch[1] ? now->ch[1]->val : 0);
    }
    
    int Query(Node *now, int l, int r, int nl = 1, int nr = MAXK) {
        if (!now) return 0;
        if (l == nl && r == nr) return now->val;
        int mid = nl + nr >> 1;
        if (r <= mid) return Query(now->ch[0], l, r, nl, mid);
        else if (l > mid) return Query(now->ch[1], l, r, mid + 1, nr);
        return Query(now->ch[0], l, mid, nl, mid) + Query(now->ch[1], mid + 1, r, mid + 1, nr);
    }
};

Seg tree[MAXK * 4 + 5];

void Modify(int now, int posx, int posy, int val, int nl = 1, int nr = MAXK) {
    tree[now].Modify(tree[now].rt, posy, val);
    if (nl == nr) return;
    int mid = nl + nr >> 1;
    if (posx <= mid) Modify(now << 1, posx, posy, val, nl, mid);
    else Modify(now << 1 | 1, posx, posy, val, mid + 1, nr);
}

int Query(int now, int xl, int xr, int yl, int yr, int nl = 1, int nr = MAXK) {
    if (xl == nl && xr == nr) return tree[now].Query(tree[now].rt, yl, yr);
    int mid = nl + nr >> 1;
    if (xr <= mid) return Query(now << 1, xl, xr, yl, yr, nl, mid);
    else if (nl > mid) return Query(now << 1 | 1, xl, xr, yl, yr, mid + 1, nr);
    return Query(now << 1, xl, mid, yl, yr, nl, mid) + Query(now << 1 | 1, mid + 1, xr, yl, yr, mid + 1, nr);
}

int main() {
    cin >> n >> k;
    for (int i = 1; i <= n; i++) cin >> data[i].x >> data[i].y >> data[i].z;
    sort(data + 1, data + n + 1);
    int sum = 1;
    for (int i = 1; i <= n; i++) {
        if (data[i + 1] == data[i]) {
            sum++;
            continue;
        }
        Modify(1, data[i].y, data[i].z, sum);
        int res = Query(1, 1, data[i].y, 1, data[i].z);
        cnt[res] += sum;
        sum = 1;
    }
    for (int i = 1; i <= n; i++) cout << cnt[i] << endl;
    return 0;
}
