// 演算法用途：將無向圖的點雙連通分量縮為「方點」，與原圖「圓點」相連成樹，用樹上演算法（樹剖/倍增）解簡單路徑交集、割點與瓶頸權值。
// 輸入輸出規範：輸入為無向圖之 n, m 與 m 條邊；輸出為圓方樹的所有連邊，格式為「圓點編號 方點編號」（方點編號由 n+1 起算）。
// 演算法用途：將無向圖的點雙連通分量縮為「方點」，與原圖「圓點」相連成樹（廣義圓方樹）。
// 節點編號：圓點 1 ~ n，方點 n+1 ~ np。
// 空間需求：點數 MAXN * 2，邊數 MAXN * 4。
// 使用方法:
//   wirte(x,y) 建原圖邊 (自動雙向)，讀完所有邊後對每個未走訪點呼叫 tarjan(i) 並 q.pop() 清殘留堆疊
//   建好的圓方樹邊存在 nwirte 建立的鄰接表 (nhead/nedges) 中，可在其上跑樹狀演算法
// 注意: 方點也會記錄 w[方點]=該點雙連通分量的圓點數 (含割點)，可用於統計簡單路徑數等
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <stack>
using namespace std;

const int MAXN = 100010;
const int MAXM = 200010;
const int MAX_NODE = MAXN * 2;
const int MAX_EDGE = MAXN * 4;

struct Edge {
    int to, next;
} edges[MAXM << 1], nedges[MAX_EDGE];

int n, m;
int head[MAXN], indexx = 1;
void wirte(int x, int y) {
    edges[indexx].to = y;
    edges[indexx].next = head[x];
    head[x] = indexx++;
}

int nhead[MAX_NODE], nindexx = 1;
void nwirte(int x, int y) {
    nedges[nindexx].to = y;
    nedges[nindexx].next = nhead[x];
    nhead[x] = nindexx++;
}

int dfn[MAXN], low[MAXN], num, np;
int w[MAX_NODE]; // 方點記錄分量圓點數
int ind;         // 當前連通分量的圓點總數
stack<int> q;

void tarjan(int now) {
    dfn[now] = low[now] = ++num;
    q.push(now);
    ind++; // 統計當前連通塊的圓點數

    for (int i = head[now]; i; i = edges[i].next) {
        int v = edges[i].to;
        if (!dfn[v]) {
            tarjan(v);
            low[now] = min(low[now], low[v]);
            // 關鍵修正：>= 同時涵蓋割點與橋
            if (low[v] >= dfn[now]) {
                np++;
                w[np] = 0;
                while (true) {
                    int nq = q.top();
                    q.pop();
                    nwirte(np, nq);
                    nwirte(nq, np);
                    w[np]++;
                    if (nq == v) break;
                }
                nwirte(now, np);
                nwirte(np, now);
                w[np]++;
            }
        } else {
            low[now] = min(low[now], dfn[v]);
        }
    }
}

int main()
{
cin >> n >> m;
np = n;
for(int i = 0;i<m;i++)
{
    int x,y;
    scanf("%d%d",&x,&y);
    wirte(x,y);
    wirte(y,x);
}
for(int i = 1;i<=n;i++)
{
    if(!dfn[i])tarjan(i),q.pop();
}
}

