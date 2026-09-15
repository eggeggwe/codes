// Given a size N integer sequence  a0,a1,…,aN−1. Process the following Q queries in order:

// 0 l r b: For each i=l,…,r−1 ,ai←min⁡(ai,b)
// 1 l r b: For each i=l,…,r−1 ,ai←max⁡(ai,b)
// 2 l r b: For each i=l,…,r−1 ,ai←ai+b
// 3 l r: Print ∑i=lr−1ai(區間總和)
#include<iostream>
#include<algorithm>
#include<stdio.h>
using namespace std;
typedef long long ll;
#define N 400010
int n,q,t;
ll wt[N];

//線段數
ll sum[N<<2],mmax[N<<2],maxx[N<<2],maxNum[N<<2],mmin[N<<2],minn[N<<2],minNum[N<<2];
ll lazy[N<<2];
ll res = 0;//查詢答案
void pushup(int rt)
{
    maxNum[rt] = 0;
    int lrt = rt<<1,rrt = rt<<1|1;
    sum[rt] = sum[lrt]+sum[rrt];
    if(mmax[lrt]<mmax[rrt])
    {
        mmax[rt] = mmax[rrt];
        maxNum[rt]+=maxNum[rrt];
        maxx[rt] = max(mmax[lrt],maxx[rrt]);
    }else if (mmax[lrt] > mmax[rrt])
    {
        mmax[rt] = mmax[lrt];
        maxNum[rt]+=maxNum[lrt];
        maxx[rt] = max(mmax[rrt],maxx[lrt]);
    } else { 
        mmax[rt] = mmax[lrt];
        maxNum[rt] = maxNum[lrt] + maxNum[rrt];
        maxx[rt] = max(maxx[lrt], maxx[rrt]);
    }
    minNum[rt] = 0;
    if(mmin[lrt]>mmin[rrt])
    {
        mmin[rt] = mmin[rrt];
        minNum[rt]+=minNum[rrt];
        minn[rt] = min(mmin[lrt],minn[rrt]);
    }else if (mmin[lrt] < mmin[rrt])
    {
        mmin[rt] = mmin[lrt];
        minNum[rt]+=minNum[lrt];
        minn[rt] = min(mmin[rrt],minn[lrt]);
    } else { 
        mmin[rt] = mmin[lrt];
        minNum[rt] = minNum[lrt] + minNum[rrt];
        minn[rt] = min(mmin[lrt], minn[rrt]);
    }
}

void pushdown(int rt,int len)
{
    int lrt = rt << 1, rrt = rt << 1|1;
    int mid = (len) >> 1;
    // 1. 優先下推加法標記 (lazy)
    if (lazy[rt] != 0) {
        ll k = lazy[rt];
        sum[lrt] += k*(len-(len>>1));
        mmax[lrt] += k;
        if (maxx[lrt] != INT64_MIN) maxx[lrt] += k;
        mmin[lrt] += k;
        if (minn[lrt] != INT64_MAX) minn[lrt] += k;
        lazy[lrt] += k;
        sum[rrt] += k *(len>>1);
        mmax[rrt] += k;
        if (maxx[rrt] != INT64_MIN) maxx[rrt] += k;
        mmin[rrt] += k;
        if (minn[rrt] != INT64_MAX) minn[rrt] += k;
        lazy[rrt] += k;
        lazy[rt] = 0;
    }
    if(mmax[rt]<mmax[rrt])
    {
        sum[rrt]-=1ll*maxNum[rrt]*(mmax[rrt]-mmax[rt]);
        if (mmin[rrt] == mmax[rrt]) mmin[rrt] = mmax[rt];
        if (minn[rrt] == mmax[rrt]) minn[rrt] = mmax[rt];
        mmax[rrt] = mmax[rt];
    }
    if(mmax[rt]<mmax[lrt])
    {
        sum[lrt]-=1ll*maxNum[lrt]*(mmax[lrt]-mmax[rt]);
        if (mmin[lrt] == mmax[lrt]) mmin[lrt] = mmax[rt];
        if (minn[lrt] == mmax[lrt]) minn[lrt] = mmax[rt];
        mmax[lrt] = mmax[rt];
    }

    if(mmin[rt]>mmin[rrt])
    {
        sum[rrt]+=1ll*minNum[rrt]*(mmin[rt]-mmin[rrt]);
        if (mmax[rrt] == mmin[rrt]) mmax[rrt] = mmin[rt];
        if (maxx[rrt] == mmin[rrt]) maxx[rrt] = mmin[rt];
        mmin[rrt] = mmin[rt];
    }
    if(mmin[rt]>mmin[lrt])
    {
        sum[lrt]+=1ll*minNum[lrt]*(mmin[rt]-mmin[lrt]);
        if (mmax[lrt] == mmin[lrt]) mmax[lrt] = mmin[rt];
        if (maxx[lrt] == mmin[lrt]) maxx[lrt] = mmin[rt];
        mmin[lrt] = mmin[rt];
    }
}

void build(int rt,int l,int r)
{
    lazy[rt] = 0;
    if(l==r){
        mmax[rt]=sum[rt]=mmin[rt]=wt[l];
        maxx[rt] = INT64_MIN;
        minn[rt] = INT64_MAX;
        maxNum[rt] = 1;
        minNum[rt] = 1;
        return;
    }
    int mid=(l+r)>>1;
    build(rt<<1,l,mid);
    build(rt<<1|1,mid+1,r);
    pushup(rt);
}

void qurey(int rt,int l,int r,int L,int R)
{
    if(L<=l && r<=R)
    {
        res+=sum[rt];
        return;
    }else{
        pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)qurey(rt<<1,l,mid,L,R);
        if(mid<R)qurey(rt<<1|1,mid+1,r,L,R);
    }
}

void qurey_max(int rt,int l,int r,int L,int R)
{
    if(L<=l && r<=R)
    {
        res = max(res,mmax[rt]);
        return;
    }else{
        pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)qurey(rt<<1,l,mid,L,R);
        if(mid<R)qurey(rt<<1|1,mid+1,r,L,R);
    }
}

void qurey_min(int rt,int l,int r,int L,int R)
{
    if(L<=l && r<=R)
    {
        res = min(res,mmin[rt]);
        return;
    }else{
        pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)qurey(rt<<1,l,mid,L,R);
        if(mid<R)qurey(rt<<1|1,mid+1,r,L,R);
    }
}

void update_min(int rt,int l,int r,int L,int R,ll k)
{   
    if(mmax[rt]<=k)return;
    if(L<=l && r<=R && k>maxx[rt])
    {
        sum[rt]-=1ll*maxNum[rt]*(mmax[rt]-k);
        if (mmin[rt] == mmax[rt]) mmin[rt] = k;
        if (minn[rt] == mmax[rt]) minn[rt] = k;
        mmax[rt] = k;
        return;
    }else{
        pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)update_min(rt<<1,l,mid,L,R,k);
        if(mid<R)update_min(rt<<1|1,mid+1,r,L,R,k);
        pushup(rt);
    }
}

void update_max(int rt,int l,int r,int L,int R,ll k)
{   
    if(mmin[rt]>=k)return;
    if(L<=l && r<=R && k<minn[rt])
    {
        sum[rt]+=1ll*minNum[rt]*(k-mmin[rt]);
        if (mmax[rt] == mmin[rt]) mmax[rt] = k;
        if (maxx[rt] == mmin[rt]) maxx[rt] = k;
        mmin[rt] = k;
        return;
    }else{
        pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)update_max(rt<<1,l,mid,L,R,k);
        if(mid<R)update_max(rt<<1|1,mid+1,r,L,R,k);
        pushup(rt);
    }
}

void update(int rt,int l,int r,int L,int R,ll k)
{
    if(L<=l && r<=R)
    {
        sum[rt]+=1ll*k*(r-l+1);
        mmax[rt] += k;
        if (maxx[rt] != INT64_MIN) maxx[rt] += k;
        mmin[rt] += k;
        if (minn[rt] != INT64_MAX) minn[rt] += k;
        lazy[rt]+=k;
        return;
    }else{
        pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)update(rt<<1,l,mid,L,R,k);
        if(mid<R)update(rt<<1|1,mid+1,r,L,R,k);
        pushup(rt);
    }
}
//============================================================================

int main(){
ios::sync_with_stdio(0),cin.tie(0);
cin >> n >> q;
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
    x++;
    //cout << "MODE:"<<mode<<endl;
    if(mode == 0)
    {
        cin >> k;
        update_min(1,1,n,x,y,k);
    }else if(mode == 1)
    {   
        cin >> k;
        update_max(1,1,n,x,y,k);
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

