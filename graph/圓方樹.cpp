// 演算法用途：將無向圖的點雙連通分量縮為「方點」，與原圖「圓點」相連成樹，用樹上演算法（樹剖/倍增）解簡單路徑交集、割點與瓶頸權值。
// 輸入輸出規範：輸入為無向圖之 n, m 與 m 條邊；輸出為圓方樹的所有連邊，格式為「圓點編號 方點編號」（方點編號由 n+1 起算）。
#include<iostream>
#include<algorithm>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stack>
using namespace std;
using ll = long long;
#define N 1000010
struct nice{
    int to,next;
}edges[N << 1],nedges[N << 1];
int n,m;
int head[N],indexx = 1;
void wirte(int x,int y)
{
    edges[indexx].to = y;
    edges[indexx].next = head[x];
    head[x] = indexx++;
}
int nhead[N],nindexx = 1;
void nwirte(int x,int y)
{
    nedges[nindexx].to = y;
    nedges[nindexx].next = nhead[x];
    nhead[x] = nindexx++;
}

int dfn[N],low[N],num,size[N],np = 0,w[N << 1];
ll ans[N];
stack<int> q;
void tarjan(int now){
    dfn[now] = low[now] = ++num;
    q.push(now);
    //cout << now <<' ';
    for(int i = head[now]; i; i = edges[i].next){
        int v = edges[i].to;
        //cout << now << ' '<<v<<endl;
        if(!dfn[v])
        {
            tarjan(v);
            low[now] = min(low[v],low[now]);
            if(low[v]==dfn[now])
            {
                w[++np] = 0;
                int nq = 0;
                while(nq != v)
                {
                    nq = q.top();
                    //cout << nq << ' ' << np << endl;
                    q.pop();
                    nwirte(np,nq);
                    nwirte(nq,np);
                    
                    w[np]++;
                }
                nwirte(now,np);
                nwirte(np,now);
                //cout << now << ' ' << np << endl;
                w[np]++;
                //cout << now <<' '<< size[now] << endl;
            }
        }
        else low[now] = min(dfn[v],low[now]);
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

