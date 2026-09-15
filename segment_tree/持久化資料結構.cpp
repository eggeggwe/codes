// n 个整数构成的序列  a，将对于指定的闭区间  [l, r] 查询其区间内的第  k 小值。

#include<iostream>
#include<algorithm>
#include<stdio.h>
using namespace std;
typedef long long ll;
#define N 1000010
int n,m;

int a[N],sum[N<<2],Hash[N];
int L[N<<2],R[N<<2];//index:nodenum val:the nodenum of right or left point
int T[N<<2];//前綴的概念 index代表第i個節點進來的初始nodeNUM
int cnt = 0;//nodeNUM

int build(int l,int r)
{
    int num = ++cnt;
    if(l!=r)
    {
       // cout << l << " " << r << ' ' << num << endl;
        int mid = (l+r)>>1;
        L[num] = build(l,mid);
        R[num] = build(mid+1,r);
    }
    return num;
}

int update(int pre,int l,int r,int x)
{
    int num = ++cnt;
    L[num] = L[pre];
    R[num] = R[pre];
    sum[num] = sum[pre]+1;
    if(l!=r)
    {
        int mid = (l+r)>>1;
        if(x<=mid) L[num] = update(L[pre],l,mid,x);
        else R[num] = update(R[pre],mid+1,r,x);
    }
    return num;
}

int query(int u,int v,int l,int r,int k)
{
    if(l==r) return Hash[l];
    int mid = (l+r)>>1;
    int num = sum[L[v]]-sum[L[u]];
    if(num>=k) return query(L[u],L[v],l,mid,k);
    else return query(R[u],R[v],mid+1,r,k-num);
}

int main()
{
cin >> n >> m;
for(int i = 1;i<=n;i++)
{
    cin >> a[i];
    Hash[i] = a[i];
}
sort(Hash+1,Hash+n+1);
int size = unique(Hash+1,Hash+n+1)-Hash-1;
T[0] = build(1,size);
//cout <<"T[0]: "<<T[0] << endl;
for(int i = 1; i<=n; i++)
{
    int x = lower_bound(Hash+1,Hash+size+1,a[i])-Hash;
    //cout << x << endl;
    T[i] = update(T[i-1],1,size,x);
    //cout <<"T["<<i<<"]: "<<T[i] << endl;
}
for(int i = 1; i<=cnt; i++)
{
    //cout <<"sum["<<i<<"]: "<<sum[i] << endl;
}
while(m--)
{
    int l,r,k;
    cin >> l >> r >> k;
    cout << query(T[l-1],T[r],1,size,k) << endl;
}
return 0;
}
