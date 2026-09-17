// Given a size N integer sequence  a0,a1,…,aN−1. Process the following Q queries in order:

// 0 l r b: For each i=l,…,r−1 ,ai←min⁡(ai,b)
// 1 l r b: For each i=l,…,r−1 ,ai←max⁡(ai,b)
// 2 l r b: For each i=l,…,r−1 ,ai←ai+b
// 3 l r: Print ∑i=lr−1ai(區間總和)
// 使用方法:
//   build(1,1,n)                     初始化
//   update_min(1,1,n,l,r,b)          mode 0: 區間 chmin
//   update_max(1,1,n,l,r,b)          mode 1: 區間 chmax
//   update(1,1,n,l,r,b)              mode 2: 區間加值
//   res=0; qurey(1,1,n,l,r)          mode 3: 查詢區間和
// 注意: qurey_max/qurey_min 為額外功能 (main 未使用)，呼叫前 res 要自行設成 INT64_MIN/INT64_MAX
#include<iostream>
#include<algorithm>
#include<stdio.h>
#include<cstdint>
using namespace std;
typedef long long ll;
#define N 400010
int n,q,t;
ll wt[N];

// 標準三標記寫法 (addAll/addMx/addMn)，比直接比較 mmax 值來隱式編碼標記更穩健:
// chmin 和 chmax 同時使用時，用「比較差異」的寫法會退化成 O(n^2)，實測驗證過。
//線段數
ll sum[N<<2];
ll mx1[N<<2],mx2[N<<2]; int mxc[N<<2]; // 最大值/次大值(嚴格小於mx1)/最大值個數
ll mn1[N<<2],mn2[N<<2]; int mnc[N<<2]; // 最小值/次小值(嚴格大於mn1)/最小值個數
ll addAll[N<<2],addMx[N<<2],addMn[N<<2]; // 全體加值 / 只加給最大值那組 / 只加給最小值那組
ll mxBase[N<<2],mnBase[N<<2]; // 上次「無pending標記」時的 mx1/mn1，用來判斷 pushdown 時哪個child該拿addMx/addMn
ll res = 0;//查詢答案

void apply_(int rt,int len,ll add,ll addmx,ll addmn)
{
    if (!addAll[rt] && !addMx[rt] && !addMn[rt]) { mxBase[rt]=mx1[rt]; mnBase[rt]=mn1[rt]; }
    sum[rt] += add*len + addmx*(ll)mxc[rt] + addmn*(ll)mnc[rt];
    if (mx1[rt] == mn1[rt]) { // 整段只有一個值，最大值組=最小值組
        mx1[rt] += add + addmx + addmn;
        mn1[rt] = mx1[rt];
    } else if ((int)mxc[rt] + (int)mnc[rt] == len) { // 只有最大值/最小值兩組，沒有中間值
        mx1[rt] += add + addmx;
        mn1[rt] += add + addmn;
        mx2[rt] = mn1[rt]; // 只有兩組時，次大值就是最小組的新值 (避免陳舊)
        mn2[rt] = mx1[rt]; // 同理，次小值就是最大組的新值
    } else {
        mx1[rt] += add + addmx;
        if (mx2[rt] != INT64_MIN) mx2[rt] += add;
        mn1[rt] += add + addmn;
        if (mn2[rt] != INT64_MAX) mn2[rt] += add;
    }
    addAll[rt] += add; addMx[rt] += addmx; addMn[rt] += addmn;
}

void pushup(int rt)
{
    int lrt = rt<<1,rrt = rt<<1|1;
    sum[rt] = sum[lrt]+sum[rrt];
    if(mx1[lrt]==mx1[rrt])
    {
        mx1[rt]=mx1[lrt]; mxc[rt]=mxc[lrt]+mxc[rrt]; mx2[rt]=max(mx2[lrt],mx2[rrt]);
    }else if(mx1[lrt]>mx1[rrt])
    {
        mx1[rt]=mx1[lrt]; mxc[rt]=mxc[lrt]; mx2[rt]=max(mx2[lrt],mx1[rrt]);
    }else{
        mx1[rt]=mx1[rrt]; mxc[rt]=mxc[rrt]; mx2[rt]=max(mx1[lrt],mx2[rrt]);
    }
    if(mn1[lrt]==mn1[rrt])
    {
        mn1[rt]=mn1[lrt]; mnc[rt]=mnc[lrt]+mnc[rrt]; mn2[rt]=min(mn2[lrt],mn2[rrt]);
    }else if(mn1[lrt]<mn1[rrt])
    {
        mn1[rt]=mn1[lrt]; mnc[rt]=mnc[lrt]; mn2[rt]=min(mn2[lrt],mn1[rrt]);
    }else{
        mn1[rt]=mn1[rrt]; mnc[rt]=mnc[rrt]; mn2[rt]=min(mn1[lrt],mn2[rrt]);
    }
    mxBase[rt]=mx1[rt]; mnBase[rt]=mn1[rt]; // 剛 pushup 完，無 pending 標記，紀錄乾淨基準值
}

void pushdown(int rt,int len)
{
    int lrt = rt<<1, rrt = rt<<1|1;
    if (addAll[rt] || addMx[rt] || addMn[rt]) {
        // 只有 mx1[child] 剛好等於 pushdown 前的基準值，才該拿 addMx (代表 child 真的持有那個最大值群組)
        ll lmx = (mx1[lrt]==mxBase[rt]) ? addMx[rt] : 0;
        ll lmn = (mn1[lrt]==mnBase[rt]) ? addMn[rt] : 0;
        apply_(lrt, len-(len>>1), addAll[rt], lmx, lmn);
        ll rmx = (mx1[rrt]==mxBase[rt]) ? addMx[rt] : 0;
        ll rmn = (mn1[rrt]==mnBase[rt]) ? addMn[rt] : 0;
        apply_(rrt, len>>1, addAll[rt], rmx, rmn);
        addAll[rt] = addMx[rt] = addMn[rt] = 0;
    }
}

void build(int rt,int l,int r)
{
    addAll[rt]=addMx[rt]=addMn[rt]=0;
    if(l==r){
        mx1[rt]=sum[rt]=mn1[rt]=wt[l];
        mx2[rt] = INT64_MIN;
        mn2[rt] = INT64_MAX;
        mxc[rt] = 1;
        mnc[rt] = 1;
        mxBase[rt] = mnBase[rt] = wt[l];
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
        res = max(res,mx1[rt]);
        return;
    }else{
        pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)qurey_max(rt<<1,l,mid,L,R);
        if(mid<R)qurey_max(rt<<1|1,mid+1,r,L,R);
    }
}

void qurey_min(int rt,int l,int r,int L,int R)
{
    if(L<=l && r<=R)
    {
        res = min(res,mn1[rt]);
        return;
    }else{
        pushdown(rt,r-l+1);
        int mid=(l+r)>>1;
        if(L<=mid)qurey_min(rt<<1,l,mid,L,R);
        if(mid<R)qurey_min(rt<<1|1,mid+1,r,L,R);
    }
}

void update_min(int rt,int l,int r,int L,int R,ll k)
{
    if(mx1[rt]<=k)return;
    if(L<=l && r<=R && k>mx2[rt])
    {
        apply_(rt, r-l+1, 0, k-mx1[rt], 0);
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
    if(mn1[rt]>=k)return;
    if(L<=l && r<=R && k<mn2[rt])
    {
        apply_(rt, r-l+1, 0, 0, k-mn1[rt]);
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
        apply_(rt, r-l+1, k, 0, 0);
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
return 0;
}

