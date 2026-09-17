/*有n个元素，第i个元素有ai,bi,ci三个属性，设f(i)表示满足aj≤ai &nbsp; 且bj≤bi且cj≤ci且j不等於i的j的数量。
对于d∈[0, n)，求f(i)=d的数量。
输入格式
----
第一行两个整数n, k，表示元素数量和最大属性值。

接下来n行，每行三个整数ai,bi,ci，分别表示三个属性值。

CDQ 分治 + 樹狀陣列 (三維偏序模板，取代舊版「外層排序+內層動態開點線段樹」寫法)
使用方法:
  直接輸入 n, k 後接 n 行 (ai,bi,ci)，main() 會自動排序、去重、CDQ、輸出
  對應 Luogu P3810
原理:
  1. 依 (x,y,z) 字典序排序，讓 x 相同的點自然按 y,z 排好，之後靠 CDQ 遞迴的「左半支配右半」
     機制搭配去重代表點 (new_id 指向同組最後一個)，自動算出 x 相同時彼此互相支配的次數
  2. 把排序後的位置本身當作新的 x (p[i].x=i)，CDQ 遞迴時用這個「排名」判斷左右半
  3. 每層合併時依 y 排序，掃描時用樹狀陣列統計 z 維度，即可在 O(n log^2 n) 內算完
注意: 完全重複 (x,y,z 全同) 的點會合併算一次代表點的答案，再透過 new_id 分給同組其他點
*/
#include <iostream>
#include <algorithm>
#include <cstdio>
using namespace std;
#define N 1000010
typedef long long ll;

int n,k;

struct point{
    int x,y,z,id;
}p[N];

int new_id[N],c[N<<2],b[N],f[N];
bool cmp1(point a,point b)
{
    if(a.x!=b.x)
    return a.x<b.x;
    if(a.y!=b.y)
    return a.y<b.y;
    return a.z<b.z;
}

bool cmp2(point a,point b)
{
    if(a.y!=b.y)
    return a.y<b.y;
    if(a.z!=b.z)
    return a.z<b.z;
    return a.x<b.x;
}

int lowbit(int x)
{
    return x&(-x);
}

void add(int x,int v)
{
    while(x<=k)
    {
        c[x]+=v,x+=lowbit(x);
    }
}

int sum(int x)
{
    int ans = 0;
    while(x)
    {
        ans+=c[x];
        x-=lowbit(x);
    }
    return ans;
}

void cdq(int l,int r)
{
    if(l == r)return;
    int mid = (l+r)>>1;
    cdq(l,mid);
    cdq(mid+1,r);
    sort(p+l,p+1+r,cmp2);
    for(int i = l;i<=r;i++)
    {
        if(p[i].x<=mid)
        {
            add(p[i].z,1);
        }else{
            b[p[i].id]+=sum(p[i].z);
        }
    }
    for(int i = l;i<=r;i++)
    {
        if(p[i].x<=mid)
            add(p[i].z,-1);
    }
}



int main()
{
    cin >>n >> k;
    for(int i = 1;i<=n;i++)
    {
        int a,b,c;
        cin >> a >> b >> c;
        p[i].x = a;
        p[i].y = b;
        p[i].z = c;
        p[i].id = i;
    }
    sort(p+1,p+n+1,cmp1);
    for(int i = 1;i<=n;)
    {
        int j = i+1;
        while(j<=n&&p[i].x==p[j].x&&p[i].y==p[j].y&&p[i].z==p[j].z)
            j++;
        while(i<j)
            new_id[p[i].id] = p[j-1].id,i++;
    }
    for(int i=1;i<=n;i++)
		p[i].x=i;
    cdq(1,n);
    for(int i = 1;i<=n;i++)
        f[b[new_id[p[i].id]]]++;
    for(int i = 0;i<n;i++)
        cout << f[i] << endl;
}
