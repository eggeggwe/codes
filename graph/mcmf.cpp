// MCMF (SPFA 版): 最小費用最大流
// 用途: 求 s 到 t 的最大流量，以及達到該流量下的最小花費
// 複雜度: O(V*E) 每輪增廣, 視資料量可能較慢 (若圖大建議改 Dijkstra+勢能版)
// 使用方法:
//   MCMF f; f.init(n, s, t)           初始化
//   f.add_edge(u, v, val, cost)       加一條 u->v 容量 val、單位花費 cost 的邊
//   auto [max_flow, min_cost] = f.solve()
// 注意: 若要求「限定流量下的最小花費」而非「最大流最小費」，需自行在 solve() 迴圈內按流量提前跳出

struct MCMF{
    struct edge{
        ll v,flow,rv_id,cost;
    };
    vector<vector<edge> > graph;
    vector<bool> in_que;
    vector<ll> dis;
    vector<pll> fa;
    ll n,s,t;
    void init(int N,int S,int T){
        n=N;
        s=S;
        t=T;
        graph.assign(n+10,vector<edge>());
        in_que.assign(n+10,0);
        dis.assign(n+10,1e18);
        fa.assign(n+10,{0,0});
    }
    void add_edge(int u,int v,ll val,ll cost){
        graph[u].pb({v,val,graph[v].size(),cost});
        graph[v].pb({u,0,graph[u].size()-1,-cost});
    }
    bool spfa(){
        fill(all(in_que),0);
        fill(all(dis),1e18);
        queue<int> q;

        dis[s]=0;
        in_que[s]=true;
        q.push(s);
        while(!q.empty()){
            int now=q.front();
            q.pop();
            in_que[now]=0;
            for(int i=0;i<graph[now].size();i++){
                auto u = graph[now][i];
                if(u.flow<=0)continue;
                if(dis[u.v]<=dis[now]+u.cost)continue;
                dis[u.v]=dis[now]+u.cost;
                fa[u.v]={now,i};
                if(!in_que[u.v]){
                    in_que[u.v]=1;
                    q.push(u.v);
                }
            }
        }
        return dis[t]!=1e18;
    }
    pll solve(){
        ll max_flow=0,min_cost=0;
        while(spfa()){
            //如果你是要算最小的負值，要加這一行
            //因為到後面dis[t]>=0就該停了
            //if(dis[t] >= 0) break;
            int start=t;
            ll flow=1e18;
            while(start!=s){
                auto [u,id]=fa[start];
                flow = min(flow,graph[u][id].flow);
                start=u;
            }
            max_flow+=flow;
            if(flow==0)return {max_flow,min_cost};
            start=t;
            while(start!=s){
                auto [u,id]=fa[start];
                auto &edge = graph[u][id];
                auto &rv_edge = graph[edge.v][edge.rv_id];

                min_cost+=flow*edge.cost;
                edge.flow-=flow;
                rv_edge.flow+=flow;
                start=u;
            }
        }
        return {max_flow,min_cost};
    }
};