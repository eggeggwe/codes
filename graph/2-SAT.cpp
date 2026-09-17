// 有  n  个布尔变量  x 1 ∼ x n ，另有  m  个需要满足的条件，每个条件的形式都是 「 x i   为  true  /  false  或  x j   为  true  /  false」。比如 「 x 1   为真或  x 3   为假」、「 x 7   为假或  x 2   为假」。

// 2-SAT 问题的目标是给每个变量赋值使得所有条件得到满足。
// 使用方法:
//   wirte(...) 建圖規則: xi 若必須為 true 則連 (i+n)->i 等六種蘊含邊 (見程式碼中判斷式)
//   對 1..2n 每個未走訪節點跑 tarjan(i) 找 SCC
//   若 color[i]==color[i+n] 表示 xi 與 not xi 同一分量 -> 無解 (IMPOSSIBLE)
//   否則 color[i]<color[i+n] 即為 xi 的真值指派
// 注意: 節點 1..n 代表 xi 為真，n+1..2n 代表 xi 為假
#include<iostream>
#include<stdio.h>
#include<stack>
#include<queue>
using namespace std;
#define N 4000005
struct nice{
    int to,next;
}edges[N << 1],nedges[N <<1];
int n,m,indexx = 1;
int head[N],nhead[N];
void wirte(int x,int y)
{
    edges[indexx].to = y;
    edges[indexx].next = head[x];
    head[x] = indexx++;
}

int dfn[N],low[N],ans[N],num,sum = 0,check[N],inside[N],color[N],colornum = 0;
stack <int> s;

void tarjan(int now){
    dfn[now] = low[now] = ++num;
    s.push(now);
    check[now] = inside[now] = 1;
    for(int i = head[now]; i;i = edges[i].next){
            int next = edges[i].to;
            //cout <<now << "?????"<< next << endl;
            if(!dfn[next])
            {
                tarjan(next);
                low[now] = min(low[next],low[now]);
            }
            else if(inside[next])low[now] = min(dfn[next],low[now]);
            } 
            //cout << now << endl;
            if(low[now] == dfn[now])
            {
                //cout << "!!!!!"<<now <<endl;
                colornum++;
                int q = s.top();
                while(q != now)
                {
                    s.pop();
                    inside[q] = 0;
                    color[q] = colornum;
                    q = s.top();
                }
                color[q] = colornum;
                inside[q] = 0;
                s.pop();
            }
    return;
}

int main()
{
cin >> n >> m;
for(int q = 1;q<=m;q++)//xi為a或xj為b => not a than b or not b than a
{
    int i,a,j,b;
    cin >> i >> a >> j >> b;
    //cout << i << " " << a << " " << j << " " << b << endl;
    //0為0 not 0 為 n+0
    if(a == 1 && b == 1)
    {
        wirte(i+n,j);//i0則j1
        wirte(j+n,i);//j0則i1
    }else if(a == 1 && b == 0)
    {
        wirte(i+n,j+n);//i0則j0
        wirte(j,i);//j1則i1
    }else if(a == 0 && b == 1)
    {
        wirte(i,j);//i1則j1
        wirte(j+n,i+n);//j0則i0
    }else if(a == 0 && b == 0)
    {
        wirte(i,j+n);//i1則j0
        wirte(j,i+n);//j1則i0
    }
}
//找環
for(int i = 1;i<=2*n;i++){
if(!check[i])tarjan(i);
}

for(int i = 1;i<=n;i++)
{
    if(color[i] == color[i+n])
    {
        cout << "IMPOSSIBLE" << endl;
        return 0;
    }
}
cout << "POSSIBLE" << endl;
for(int i = 1;i<=n;i++)
{
    cout << (color[i]<color[i+n]) << ' ';
}

}

