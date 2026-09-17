// SPFA 判斷負環
// 用途: t 筆測資，每筆給 n個點、m條邊 (邊權可負)，判斷圖中是否存在負環
// 複雜度: O(n*m) 最壞情況
// 使用方法: 輸入 t，每筆測資輸入 n,m 後接 m 條邊 (a b c)；c>=0 視為無向邊，c<0 視為單向邊
// 輸出: 每筆測資輸出 YES(有負環) 或 NO(無負環)
#include<iostream>
#include<algorithm>
#include<stdio.h>
#include <queue>
#include <cstring>
using namespace std;
#define N 500010
struct nice{
    int to,next,weight;
}edges[N << 1];
int n,m,t;
int head[N],indexx = 1;
void wirte(int x,int y,int z)
{
    edges[indexx].weight = z;
    edges[indexx].to = y;
    edges[indexx].next = head[x];
    head[x] = indexx++;
}

queue<int> q;

int lenght[N],check[N],check_times[N];
bool spfa(){
    memset(check, 0, sizeof(check));
    memset(check_times, 0, sizeof(check_times));
    memset(lenght,0x3f,sizeof(lenght));
    lenght[1] = 0;
    check[1] = 1;
    q.push(1);
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
                lenght[next]=lenght[now]+edges[i].weight;
                if(!check[next])
                {
                    if(++check_times[next]>=n) return false;
                    check[next] = 1;
                    q.push(next);
                }
            }
        }
    }
    return true;
}


int main(){
cin>>t;
for(int times = 0;times<t;times++)
{
    memset(head, 0, sizeof(head));
    for(int i=0;i<(N << 1);i++) edges[i].next=0;
    indexx = 1;
    cin >> n >> m;
    for(int i = 0;i<m;i++)
    {
        int a,b,c;
        cin >> a >> b >> c;
        if(c>=0)
        {
            wirte(a, b, c);
            wirte(b, a, c);
        }else{
            wirte(a, b, c);
        }
    }
    if(spfa())
    cout << "NO"<<endl;
    else
    cout << "YES"<<endl;
}
    return 0;
}
