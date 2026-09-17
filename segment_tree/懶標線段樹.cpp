// 線段樹: 區間加值、區間乘值、區間求和 (mod m)
// 用途: 同時支援區間加法/乘法/區間求和，全程取模 m
// 複雜度: build O(n)，update/mul/qurey 均為 O(log n)
// 使用方法:
//   build(1,1,n)                 初始化，wt[1..n] 為初始值
//   mul(1,1,n,l,r,k)             區間 [l,r] 全部乘上 k
//   update(1,1,n,l,r,k)          區間 [l,r] 全部加上 k
//   res=0; qurey(1,1,n,l,r)      查詢區間和，結果存於全域變數 res
// 注意: 區間賦值 = 先乘0再加值；下標從1開始
#include<iostream>
#include<algorithm>
#include<stdio.h>
using namespace std;
typedef long long ll;
#define N 400010
int n,q,m;
ll wt[N];
//線段數
ll a[N<<3],lazy[N<<3],lazy2[N<<3];
ll res = 0;//查詢答案
void pushdown(int rt,int len)
{
    lazy[rt<<1]+=lazy[rt];
    lazy[rt<<1|1]+=lazy[rt];//rt<<1是偶數 所以在or1就等於是加一 but faster
    a[rt<<1]+=lazy[rt]*(len-(len>>1));
    a[rt<<1|1]+=lazy[rt]*(len>>1);
    lazy[rt<<1]%=m;
    lazy[rt<<1|1]%=m;
    a[rt<<1]%=m;
    a[rt<<1|1]%=m;
    lazy[rt]=0;
}
void pd2(int rt,int len)
{
    lazy2[rt<<1]*=lazy2[rt];
    lazy2[rt<<1|1]*=lazy2[rt];
    lazy[rt<<1]*=lazy2[rt];
    lazy[rt<<1|1]*=lazy2[rt];
    a[rt<<1]*=lazy2[rt];
    a[rt<<1|1]*=lazy2[rt];
    lazy2[rt<<1]%=m;
    lazy2[rt<<1|1]%=m;
    lazy[rt<<1]%=m;
    lazy[rt<<1|1]%=m;
    a[rt<<1]%=m;
    a[rt<<1|1]%=m;
    lazy2[rt]=1;
}

void build(int rt,int l,int r)
{
    lazy2[rt]=1;
    if(l==r){
        a[rt]=wt[l];
        return;
    }
    int mid=(l+r)>>1;
    build(rt<<1,l,mid);
    build(rt<<1|1,mid+1,r);
    a[rt]=(a[rt<<1]+a[rt<<1|1])%m;
}

void qurey(int rt,int l,int r,int L,int R)
{
    if(L<=l && r<=R)
    {
        res+=a[rt];
        res%=m;
        return;
    }else{
        if(lazy2[rt]!=1)pd2(rt,r-l+1);
        if(lazy[rt])pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)qurey(rt<<1,l,mid,L,R);
        if(mid<R)qurey(rt<<1|1,mid+1,r,L,R);
    }
}

void update(int rt,int l,int r,int L,int R,ll k)
{
    if(L<=l && r<=R)
    {
        lazy[rt]+=k;
        lazy[rt]%=m;
        a[rt]+=k*(r-l+1);
        a[rt]%=m;
    }else{
        if(lazy2[rt]!=1)pd2(rt,r-l+1);
        if(lazy[rt])pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)update(rt<<1,l,mid,L,R,k);
        if(mid<R)update(rt<<1|1,mid+1,r,L,R,k);
        a[rt]=(a[rt<<1]+a[rt<<1|1])%m;
    }
}

void mul(int rt,int l,int r,int L,int R,ll z)
{
    if(L<=l && r<=R)
    {
        lazy2[rt]*=z;
        lazy2[rt]%=m;
        lazy[rt]*=z;
        lazy[rt]%=m;
        a[rt]*=z;
        a[rt]%=m;
    }else{
        if(lazy2[rt]!=1)pd2(rt,r-l+1);
        if(lazy[rt])pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)mul(rt<<1,l,mid,L,R,z);
        if(mid<R)mul(rt<<1|1,mid+1,r,L,R,z);
        a[rt]=(a[rt<<1]+a[rt<<1|1])%m;
    }
}


//assign 先乘以0，後面用add
//============================================================================

int main(){
cin >> n >> q >> m;
for(int i = 1;i<=n;i++)
{
    cin >> wt[i];
}
build(1,1,n);
for(int i = 1;i<=q;i++)
{
    int mode;
    cin >> mode;
    ll x,y,k;
    cin >> x >> y;
    if(mode == 1)
    {
        cin >> k;
        mul(1,1,n,x,y,k);
    }else if(mode == 2)
    {
        cin >> k;
        update(1,1,n,x,y,k);
    }else if(mode == 3)
    {
        res = 0;
        qurey(1,1,n,x,y);
        cout << res << endl;
    }
}
return 0;
}
