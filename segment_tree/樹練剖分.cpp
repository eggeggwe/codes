// 如题，已知一棵包 N  个结点的树（连通且无环），每个节点上包含一个数值，需要支持以下操作：

// 1 x y z，表示将树从 x  到  y  结点最短路径上所有节点的值都加上  z。

// 2 x y，表示求树从  x  到  y  结点最短路径上所有节点的值之和。

// 3 x z，表示将以  x  为根节点的子树内所有节点值都加上  z。

// 4 x，表示求以  x  为根节点的子树内所有节点值之和。
#include<iostream>
#include<algorithm>
#include<stdio.h>
using namespace std;
typedef long long ll;
#define N 400010
struct nice{
    int to,next;
}edges[N << 1];
int n,m,r,p;
int head[N],index = 1;
void wirte(int x,int y)
{
    edges[index].to = y;
    edges[index].next = head[x];
    head[x] = index++;
}
int w[N],wt[N];
int lenght[N],check[N],size[N],depth[N];
int son[N],id[N],fa[N],cnt,top[N];
//重兒子，新編號，父親，count，鍊的頂端

//線段數
int a[N<<2],lazy[N<<2];
int res = 0;//查詢答案
void pushdown(int rt,int len)
{
    lazy[rt<<1]+=lazy[rt];
    lazy[rt<<1|1]+=lazy[rt];//rt<<1是偶數 所以在or1就等於是加一 but faster
    a[rt<<1]+=lazy[rt]*(len-(len>>1));
    a[rt<<1|1]+=lazy[rt]*(len>>1);
    a[rt<<1]%=p;
    a[rt<<1|1]%=p;
    lazy[rt]=0;
}

void build(int rt,int l,int r)
{
    if(l==r){
        a[rt]=wt[l]%p;
        if(a[rt]>p)a[rt]%=p;
        return;
    }
    int mid=(l+r)>>1;
    build(rt<<1,l,mid);
    build(rt<<1|1,mid+1,r);
    a[rt]=(a[rt<<1]+a[rt<<1|1])%p;
}

void qurey(int rt,int l,int r,int L,int R)
{
    if(L<=l && r<=R)
    {
        res+=a[rt];
        res%=p;
        return;
    }else{
        if(lazy[rt])pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)qurey(rt<<1,l,mid,L,R);
        if(mid<R)qurey(rt<<1|1,mid+1,r,L,R);
    }
}

void update(int rt,int l,int r,int L,int R,int k)
{
    if(L<=l && r<=R)
    {
        lazy[rt]+=k;
        a[rt]+=k*(r-l+1);
    }else{
        if(lazy[rt])pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)update(rt<<1,l,mid,L,R,k);
        if(mid<R)update(rt<<1|1,mid+1,r,L,R,k);
        a[rt]=(a[rt<<1]+a[rt<<1|1])%p;
    }
}
//============================================================================
int sumRange(int x,int y)
{
    int ans = 0;
    while(top[x]!=top[y])
    {
        if(depth[top[x]]<depth[top[y]])swap(x,y);
        res = 0;
        qurey(1,1,n,id[top[x]],id[x]);//ans加上x點到鍊頂端的區間和
        ans+=res;
        ans%=p;
        x=fa[top[x]];//移動x點
    }
    //在同條鍊上
    if(depth[x]>depth[y])swap(x,y);
    res = 0;
    qurey(1,1,n,id[x],id[y]);
    ans+=res;
    return ans%p;
}

void addRange(int x,int y,int k)
{
    k%=p;
    while(top[x]!=top[y])
    {
        if(depth[top[x]]<depth[top[y]])swap(x,y);
        update(1,1,n,id[top[x]],id[x],k);
        x=fa[top[x]];
    }
    if(depth[x]>depth[y])swap(x,y);
    update(1,1,n,id[x],id[y],k);
}

int sumSon(int x)
{
    res = 0;
    qurey(1,1,n,id[x],id[x]+size[x]-1);//子樹編號是連續的
    return res;
}

void addSon(int x,int k)
{
    update(1,1,n,id[x],id[x]+size[x]-1,k);
}
//================================================================
void dfs1(int x,int f,int deep)
{
    depth[x] = deep;
    fa[x] = f;
    size[x] = 1;
    int maxson = -1;//找重兒子
    for(int i = head[x]; i; i = edges[i].next){
        int next = edges[i].to;
        if(next!=f)
        {
            dfs1(next, x, deep+1);
            size[x] += size[next];
            if(size[next]>maxson)son[x] = next,maxson = size[next];
        }
    }
}

void dfs2(int x,int topf){
    id[x] = ++cnt;
    wt[cnt] = w[x];
    top[x] = topf;
    if(!son[x])return;
    dfs2(son[x],topf);
    for(int i = head[x]; i; i = edges[i].next)
    {
        int next = edges[i].to;
        if(next == fa[x] || next == son[x])continue;
        dfs2(next,next);
    }
}

int main(){
cin >> n >> m >> r >> p;
for(int i = 1;i<=n;i++)
{
    cin >> w[i];
}
for(int i = 1;i<n;i++)
{
    int x,y;
    cin >> x >> y;
    wirte(x,y);
    wirte(y,x);
}
dfs1(r,0,1);
dfs2(r,r);
build(1,1,n);
for(int i = 1;i<=m;i++)
{
    int mode;
    cin >>mode;
    if(mode == 1)
    {
        int x,y,z;
        cin >> x >> y >> z;
        addRange(x, y, z);
    }else if(mode == 2)
    {
        int x,y;
        cin >> x >> y;
        cout << sumRange(x, y) << endl;
    }else if(mode == 3)
    {
        int x,z;
        cin >> x >> z;
        addSon(x, z);
    }else if(mode == 4)
    {
        int x;
        cin >> x;
        cout << sumSon(x) << endl;
    }
}
}
