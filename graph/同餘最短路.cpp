/*
当出现形如「给定 𝑛个整数，求这 𝑛个整数能拼凑出多少的其他整数（𝑛个整数可以重复取）」，以及「给定 𝑛个整数，求这 𝑛个整数不能拼凑出的最小（最大）的整数」，或者「至少要拼几次才能拼出模 𝐾余 𝑝的数」的问题时可以使用同余最短路的方法。
题目大意：给定 𝑥，𝑦，𝑧，ℎ，对于 𝑘 ∈[1,ℎ]，有多少个 𝑘
k 能够满足 𝑎𝑥 +𝑏𝑦 +𝑐𝑧 =𝑘。（0 ≤𝑎,𝑏,𝑐，1 ≤𝑥,𝑦,𝑧 ≤10^5，ℎ ≤2^63 −1）
 * 
 * 
 * 
 * */
// 使用方法:
//   對每個餘數 i∈[0,x) 建邊 i -> (i+y)%x 權重y, i -> (i+z)%x 權重z
//   以節點0為源點跑最短路 (SPFA/dijkstra皆可，此檔用SPFA)
//   lenght[i] = 用 y,z 湊出「模 x 餘 i」的最小總和
//   對每個餘數 i，若 h>=lenght[i]，能湊出的數量為 (h-lenght[i])/x + 1，加總即為答案
// 注意: 圖為 0-indexed 節點 (0..x-1)，跟一般模板的 1-indexed 不同，抄用時要注意
#include<iostream>
#include<algorithm>
#include<stdio.h>
#include <queue>
#include <cstring>
using namespace std;
using ll = long long;
#define N 500010
struct nice{
    int to,next;
    ll weight;
}edges[N << 1];
ll h,x,y,z;
ll head[N],indexx = 1;
void wirte(int x,int y,ll z)
{
    edges[indexx].weight = z;
    edges[indexx].to = y;
    edges[indexx].next = head[x];
    head[x] = indexx++;
}

queue<int> q;

ll lenght[N],check[N],check_times[N];//length[n][0] indeicate the shortest [1] indeicate the second shortest 
void spfa(){
    memset(check, 0, sizeof(check));
    memset(check_times, 0, sizeof(check_times));
    memset(lenght,0x3f,sizeof(lenght));
    lenght[0] = 0;
    check[0] = 1;
    q.push(0);
    while(!q.empty())
    {
        int now = q.front();
        q.pop();
        check[now] = 0;
        for(int i = head[now]; i; i = edges[i].next)
        {
            int next = edges[i].to;
            if(lenght[next]>lenght[now]+edges[i].weight)
           {
                lenght[next] = lenght[now]+edges[i].weight;
                if(!check[next])
                {
                    check[next] = 1;
                    q.push(next);
                }
            }
        }
    }
}


int main(){
cin >> h >> x >> y >> z;
for(int i = 0;i<x;i++)
{
    wirte(i,(i+y)%x,y);
    wirte(i,(i+z)%x,z);
}
h--;
spfa();
ll ans = 0;
for(int i = 0;i<x;i++)
{
    if(h>=lenght[i]) ans+=(h-lenght[i])/x+1;
}
cout << ans<<endl;

return 0;
}
