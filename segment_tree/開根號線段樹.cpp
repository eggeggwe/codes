#include<iostream>
#include<algorithm>
#include<stdio.h>
#include<math.h>
using namespace std;
typedef long long ll;
#define N 1000010
ll wt[N];
int n,m;
//線段數
ll asum[N<<2],amax[N<<2];
ll res = 0;//查詢答案
void build(int rt,int l,int r)
{
    if(l==r){
        asum[rt]=wt[l];
        amax[rt]=wt[l];
        return;
    }
    int mid=(l+r)>>1;
    build(rt<<1,l,mid);
    build(rt<<1|1,mid+1,r);
    asum[rt]=asum[rt<<1]+asum[rt<<1|1];
    amax[rt] = max(amax[rt<<1],amax[rt<<1|1]);
}

void qurey(int rt,int l,int r,int L,int R)
{
    if(L<=l && r<=R)
    {
        res+=asum[rt];
        return;
    }else{
        int mid=(l+r)>>1;
        if(L<=mid)qurey(rt<<1,l,mid,L,R);
        if(mid<R)qurey(rt<<1|1,mid+1,r,L,R);
    }
}

void update(int rt,int l,int r,int L,int R)
{
    if(l==r)
    {
        asum[rt] = sqrt(asum[rt]);
        amax[rt] = sqrt(amax[rt]);
        //cout << rt << " " << asum[rt] << " " << amax[rt] <<endl;
    }else{
        int mid=(l+r)>>1;
        if(L<=mid && amax[rt<<1]>1)update(rt<<1,l,mid,L,R);
        if(mid<R  && amax[rt<<1|1]>1)update(rt<<1|1,mid+1,r,L,R);
        asum[rt]=asum[rt<<1]+asum[rt<<1|1];
        amax[rt]=max(amax[rt<<1],amax[rt<<1|1]);
    }
}
//============================================================================

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &wt[i]);
    }
    build(1, 1, n);
    for (int i = 1; i <= m; i++) {
        int mode;
        int x, y;
        scanf("%d %d %d", &mode, &x, &y);
        if (x > y) { int t = x; x = y; y = t; }
        if (mode == 1) {
            update(1, 1, n, x, y);
        } else {
            res = 0;
            qurey(1, 1, n, x, y);
            printf("%lld\n", res);
        }
    }
    return 0;
}
