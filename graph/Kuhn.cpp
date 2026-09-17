// 匈牙利演算法 (Kuhn): 二分圖最大匹配 (不需權重)
// 用途: 左邊 n 個點、右邊 m 個點，求最大匹配數
// 複雜度: O(V*E)
// 使用方法:
//   kuhn k; k.init(n, m)          初始化 (0-indexed)
//   k.add_edge(u, v)              左點 u 連右點 v
//   k.build()                     執行匹配，結果存於 k.ans
// 注意: build() 內先做貪心配對再對未配對點跑增廣路，加速常數
struct kuhn{
    vector<vll> graph;
    ll n,m;
    vector<bool> visited;
    vector<bool> tag;
    vll mat;
    int ans=0;
    void init(int N,int M){
        n=N;
        m=M;
        graph.assign(n,vll());
        mat.assign(m,-1);
        tag.assign(n,0);
        ans=0;
    }
    void add_edge(int u,int v){
        graph[u].pb(v);
    }
    bool dfs(int x){
        for(auto u:graph[x]){
            if(visited[u])continue;
            visited[u]=1;                
            if(mat[u]==-1 || dfs(mat[u])){
                mat[u]=x;
                tag[x]=true;
                return true;
            }
        }
        return false;
    }
    void build(){
        rep(i,0,n){
            for(auto u:graph[i]){
                if(mat[u]==-1){
                    mat[u]=i;
                    ans++;
                    tag[i]=1;
                    break;
                }
            }
        }
        rep(i,0,n){
            visited.assign(m,0);
            if(!tag[i] && dfs(i)){
                ans++;
            }
        }
    }
};