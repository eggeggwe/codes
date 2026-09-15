#include <iostream>
#include <cstdio>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

#define N 500010

struct nice {
    int to, next;
} edges[N << 1], nedges[N << 1];

int n, m, indexx = 1, indexn = 1;
int head[N], nhead[N];

void wirte(int x, int y) {
    edges[indexx].to = y;
    edges[indexx].next = head[x];
    head[x] = indexx++;
}

void nwirte(int x, int y) {
    nedges[indexn].to = y;
    nedges[indexn].next = nhead[x];
    nhead[x] = indexn++;
}

int dfn[N], low[N], num;
int check[N], inside[N];
int ne[N], nw[N], in[N];
stack<int> s;

void tarjan(int now) {
    dfn[now] = low[now] = ++num;
    s.push(now);
    check[now] = inside[now] = 1;
    for (int i = head[now]; i; i = edges[i].next) {
        int next = edges[i].to;
        if (!dfn[next]) {
            tarjan(next);
            low[now] = min(low[next], low[now]);
        } else if (inside[next]) {
            low[now] = min(dfn[next], low[now]);
        }
    } 
    
    if (low[now] == dfn[now]) {
        while (true) {
            int q = s.top();
            s.pop();
            inside[q] = 0;
            ne[q] = now;
            if (q == now) break;
            nw[now] += nw[q]; // 把點權累加給代表點 now
        }
    }
}

int F[N];

void topo() {
    queue<int> q;
    //只把是「代表點」且「入度為 0」的點放進隊列
    for (int i = 1; i <= n; i++) {
        if (ne[i] == i && !in[i]) {
            q.push(i);
            F[i] = nw[i];
        }
    }
    
    while (!q.empty()) {   
        int x = q.front();
        q.pop();
        for (int i = nhead[x]; i; i = nedges[i].next) {
            int y = nedges[i].to;
            in[y]--;
            F[y] = max(F[x] + nw[y], F[y]);
            if (!in[y]) q.push(y);
        }
    }
}

int main() {
    // 提升 I/O 速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> m)) return 0;
    for (int i = 1; i <= n; i++) {
        cin >> nw[i];
    }
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        wirte(x, y);
    }

    // 找 SCC
    for (int i = 1; i <= n; i++) {
        if (!check[i]) tarjan(i);
    }

    // 重新建圖
    for (int now = 1; now <= n; now++) {
        for (int i = head[now]; i; i = edges[i].next) {
            int v = edges[i].to;
            if (ne[v] != ne[now]) {
                nwirte(ne[now], ne[v]);
                in[ne[v]]++;
            }
        }
    }

    topo();

    //只統計代表點的 F[i]
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        if (ne[i] == i) {
            sum = max(sum, F[i]);
        }
    }
    cout << sum << "\n";

    return 0;
}
