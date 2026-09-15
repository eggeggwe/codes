// Plans on the website have three types:

// With a plan of this type you can open a portal from planet v  to planet u.
// With a plan of this type you can open a portal from planet v  to any planet with index in range  [l, r].
// With a plan of this type you can open a portal from any planet with index in range  [l, r]  to planet  v.
// 求最短路
// 將區間化成節點，開兩個線段樹，一進一出，最後直接DFS
#include<iostream>
#include<algorithm>
#include<queue>
#include<stdio.h>
using namespace std;
typedef long long ll;
#define N 200100
struct nice{
    int to,next;
    ll weight;
}edges[5000000];
int n,q;
int head[N<<3],index = 1;
void wirte(int x,int y,ll z)
{
    //cerr << x << ' '<< y <<' '<< z << endl;
    edges[index].weight = z;
    edges[index].to = y;
    edges[index].next = head[x];
    head[x] = index++;
}
struct node
{
    int pos;
    ll dis;
    bool operator <(const node &other)const
    {
        return other.dis<dis;
    }
};

ll leaves[N];
int dx = 400010;
void build(int rt,int l,int r)
{
    if(l==r){
        leaves[l] = rt;
        return;
    }
    int mid=(l+r)>>1;
    build(rt<<1,l,mid);
    build(rt<<1|1,mid+1,r);
    //cerr << "NICE: "<<rt << ' ' << l << ' ' << r << endl;
    wirte((rt<<1)+dx,rt+dx,0);
    wirte(((rt<<1)|1)+dx, rt+dx, 0);
    wirte(rt,rt<<1|1,0);
    wirte(rt,(rt<<1),0);
}

void connect(int rt,int l,int r,int L,int R,int u,ll w,int mode)
{
    if(L<=l && r<=R)
    {
        if(mode == 2)
        wirte(leaves[u],rt,w);
        else
        wirte(rt+dx,leaves[u],w);
        return;
    }else{
        int mid=(l+r)>>1;
        if(L<=mid)connect(rt<<1,l,mid,L,R,u,w,mode);
        if(mid<R)connect(rt<<1|1,mid+1,r,L,R,u,w,mode);
    }
}

priority_queue<node> pq;

ll lenght[N<<3],check[N<<3];
void dijkstra(int s){
    fill(lenght, lenght+(N<<3), -1);
    fill(check, check+(N<<3), false);
    lenght[s] = 0;
    pq.push((node){s, 0});
    while(!pq.empty())
    {
        node index = pq.top();
        pq.pop();
        int now = index.pos,len = index.dis;
        if(!check[now])
        {
            check[now] = 1;
            for(int i = head[now]; i; i = edges[i].next)
            {
                int next = edges[i].to;
                if(lenght[next] == -1 || lenght[next] > lenght[now] + edges[i].weight)
                {
                    lenght[next]=lenght[now]+edges[i].weight;
                    pq.push((node){next, lenght[next]});
                }
            }
        }
    }
}

int st;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
cin >> n >> q >> st;
build(1,1,n);
for(int i = 1;i<=n;i++)
{
    wirte(leaves[i],leaves[i]+dx,0);
    wirte(leaves[i]+dx,leaves[i],0);
}
for(int i = 1;i<=q;i++)
{
    int mode;
    cin >> mode;
    ll u,v,w,l,r;
    if(mode == 1)
    {
        cin >> u >> v >> w;
        wirte(leaves[u],leaves[v],w);
    }else
    {
        cin >> u >> l >> r >> w;
        connect(1,1,n,l,r,u,w,mode);
    }
}
    dijkstra(leaves[st]);
    for(int i = 1;i<=n;i++)
    {
        cout << lenght[leaves[i]] << ' ';
    }
}

