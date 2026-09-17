// Dinic 演算法: 最大流
// 用途: 求 S 到 T 的最大流量
// 複雜度: O(V^2 * E)，一般圖跑得比理論快很多
// 使用方法:
//   Dinic d; d.init(n, S, T)          初始化節點數與源點/匯點
//   d.add_edge(u, v, flow)            加一條 u->v 容量 flow 的邊 (自動加反向邊容量0)
//   d.dinic()                          回傳最大流量

struct Dinic{
    struct edge{
        ll v,flow,rv_id;
    };
    vector<vector<edge> > graph;
    vll level;
    vll cur;
    ll S,T,n;
    void init(int N,int ss,int t){
        S=ss;
        T=t;
        n=N;
        graph.assign(n+10,vector<edge>());
        level.assign(n+10,0);
        cur.assign(n+10,0);
    }
    void add_edge(int u,int v,ll flow){
        graph[u].pb({v,flow,graph[v].size()});
        graph[v].pb({u,0,graph[u].size()-1});
    }
    bool bfs(){
        fill(all(level),-1);
        level[S]=0;
        queue<int> q;
        q.push(S);
        while(!q.empty()){
            auto now=q.front();
            q.pop();
            for(auto u:graph[now]){
                if(level[u.v]!=-1)continue;
                if(u.flow<=0)continue;
                level[u.v]=level[now]+1;
                q.push(u.v);
            }
        }
        return level[T]!=-1;
    }
    ll dfs(int x,ll flow){
        if(x==T)return flow;
        ll res=0;
        for(ll &i=cur[x];i<graph[x].size();i++){
            auto &[u,fl,rv_id]=graph[x][i];
            if(fl<=0)continue;
            if(level[u]!=level[x]+1)continue;
            ll tmp=dfs(u,min(flow,fl));
            res+=tmp;
            flow-=tmp;
            fl-=tmp;
            graph[u][rv_id].flow+=tmp;
            if(flow==0)return res;
        }
        if(res==0)level[x]=-1;
        return res;
    }
    ll dinic(){
        ll ans=0;
        while(bfs()){
            cur.assign(n+10,0);
            ans+=dfs(S,LINF);
        }
        return ans;
    }
};