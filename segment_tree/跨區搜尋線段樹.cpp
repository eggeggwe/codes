// A string consisting of  0  and  1  is called a  good string  if two consecutive characters in the string are always different.

// 1 L R: Flip each of the L-th to R-th characters of  S. That is, for each integer  i  satisfying  L ≤ i ≤ R, change the  i-th character of  S  to  0  if it is  1, and vice versa.
// 2 L R: Let S  be the string of length  (R − L + 1)  obtained by extracting the  L-th to  R-th characters of  S  (without changing the order). Print  Yes  if  S ′  is a good string and  No  otherwise.
#include<iostream>
#include<algorithm>
#include<stdio.h>
using namespace std;
typedef long long ll;
#define N 500010
int n,q;

struct Node
{
    ll lmax,rmax,amax,lval,rval;
    int L,R;
    bool lazy;
    Node():lmax(0),rmax(0),amax(0),lval(0),rval(0),L(0),R(0),lazy(false) {}
}tree[N<<4];

ll res = 0;
int input[N<<4];

Node merge(Node l,Node r)
{
    Node tmp;
    tmp.lmax = l.lmax;
    tmp.rmax = r.rmax;
    tmp.amax = max(l.amax,r.amax);
    tmp.lval = l.lval;
    tmp.rval = r.rval;
    tmp.L = l.L;
    tmp.R = r.R;
    if(l.rval != r.lval)
    {
        tmp.amax = max(tmp.amax,r.lmax+l.rmax);
        if(r.amax == r.R-r.L+1)tmp.rmax = r.amax+l.rmax;
        if(l.amax == l.R-l.L+1)tmp.lmax = l.amax+r.lmax;
    }
    return tmp;
}

void applyLazy(int rt)
{
    if (tree[rt].lazy) {
        if (tree[rt].R!=tree[rt].L){
            tree[rt<<1].lazy=!tree[rt<<1].lazy;
            tree[rt<<1|1].lazy=!tree[rt<<1|1].lazy;
        }
        tree[rt].lval ^= 1;
        tree[rt].rval ^= 1;
        tree[rt].lazy = false;
    }
}

void build(int rt,int l,int r)
{
    tree[rt].L = l;
    tree[rt].R = r;
    tree[rt].lazy = false;
    if(l == r){
        tree[rt].rval = tree[rt].lval = input[l];
        tree[rt].amax = tree[rt].rmax = tree[rt].lmax = 1;
        return;
    }
    int mid = (l+r)>>1;
    build(rt<<1,l,mid);
    build(rt<<1|1,mid+1,r);
    tree[rt] = merge(tree[rt<<1],tree[rt<<1|1]);
}

Node qurey(int rt,int l,int r,int L,int R)
{
    applyLazy(rt);
    //cout << valid << ' ' << l << ' ' << r << ' ' << L << ' ' << R << endl;
    if(L<=l && r<=R)
    {
        return tree[rt];
    }else{
        bool rcheck,lcheck;
        int mid = (l+r)>>1;
        lcheck = L<=mid;
        rcheck = mid<R;
        if(rcheck && lcheck)
        {
            return merge(qurey(rt<<1,l,mid,L,R),qurey(rt<<1|1,mid+1,r,L,R));
        }else if(rcheck){return qurey(rt<<1|1,mid+1,r,L,R);}
         else if(lcheck){return qurey(rt<<1,l,mid,L,R);}
    }
}

void update(int rt,int l,int r,int L,int R)
{
    applyLazy(rt);
    if (l>R||r<L) return;
    if(L<=l && r<=R)
    {
        tree[rt].lazy^=true;
        applyLazy(rt);
        return;
    }
    int mid = (l+r)>>1;
    update(rt<<1,l,mid,L,R);
    update(rt<<1|1,mid+1,r,L,R);
    applyLazy(rt<<1);
    applyLazy(rt<<1|1);
    tree[rt] = merge(tree[rt<<1],tree[rt<<1|1]);
}

int main()
{
    int n,q;
    cin >> n >> q;
    char k = getchar();
    for(int i = 1;i<=n;i++)
    {
        k = getchar();
        input[i] = k-'0';
    }
    build(1,1,n);
    for(int i = 1;i<=q;i++)
    {
        int mode,l,r;
        cin >> mode >> l >> r;
        if(mode == 1)
        {
            update(1,1,n,l,r);
        }else{
            Node ans = qurey(1,1,n,l,r);
            if(ans.amax == r-l+1)cout << "Yes"<< endl;
            else cout << "No"<<endl;
        }
    }
}

