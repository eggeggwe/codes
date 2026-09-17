/*給定一個 $n$ 個點的帶權樹，和 $m$ 條路徑 $(u_i, v_i)$。你可以選擇樹上的一條邊，將其權重變為 0。你的目標是最小化 $m$ 條路徑中，最長那條路徑的長度。輸入:第一行: n m接下來 $n-1$ 行: a b t (邊 $(a, b)$ 權重為 $t$)接下來 $m$ 行: u v (一條路徑的起點和終點)輸出:一個整數，代表最小化的「最長路徑長度」。*/
// 使用方法:
//   Tree::wirte(x,y,z)                建邊 (自動雙向)
//   Tree::build_tables(root,n)        建立 LCA 倍增表 (需在讀完所有邊後呼叫一次)
//   Tree::path(x,y)                    查詢 x 到 y 的路徑權重和
//   Tree::lca(x,y)                     查詢 x,y 的最近公共祖先
//   Tree::run_tree_diff(root)          執行樹上差分 (需先對 Tree::dif[] 打標記)
// 本檔案 main() 額外實作「二分答案 + 樹上差分」求最小化最長路徑 (運輸計劃問題)
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <queue>
#include <string.h>
#include <climits> // 為了 INT_MAX

using namespace std;
typedef long long ll;
#define N 500010
#define LOG_N 20 // 倍增法 logN 上限

//================================================================
// 樹 模組 (LCA, 倍增路徑和, 樹上差分)
//================================================================
namespace Tree {
    struct nice{
        int to, next;
        int weight;
    } edges[N << 1]; // 儲存邊

    int head[N], indexx = 1;
    
    // 倍增法相關
    int depth[N];       // 節點深度
    int f[N][LOG_N + 1]; // f[i][j]: i 的 2^j 祖先
    int w[N][LOG_N + 1]; // w[i][j]: i 到 2^j 祖先的路徑和
    
    // 樹上差分相關
    int dif[N];           // 差分陣列
    int edge[N << 1]; // 儲存每條邊被覆蓋的次數
    
    // DFS 輔助
    int check[N];

    // 添加一條雙向邊
    void wirte(int x, int y, int z) {
        edges[indexx].weight = z;
        edges[indexx].to = y;
        edges[indexx].next = head[x];
        head[x] = indexx++;
        
        edges[indexx].weight = z;
        edges[indexx].to = x;
        edges[indexx].next = head[y];
        head[y] = indexx++;
    }

    // DFS 預處理 建立 depth, f[][0], w[][0]
    void dfs(int now) {
        check[now] = 1;
        for (int i = head[now]; i; i = edges[i].next) {
            int next = edges[i].to;
            if (!check[next]) {
                depth[next] = depth[now] + 1;
                f[next][0] = now;
                w[next][0] = edges[i].weight;
                dfs(next);
            }
        }
    }

    // 建立完整的倍增表
    void build_tables(int root, int n_nodes) {
        memset(check, 0, sizeof(check));
        depth[root] = 1;
        dfs(root);

        // 處理根節點
        f[root][0] = root; 
        w[root][0] = 0;

        // 建立 f 和 w 的 2^i 表
        for (int i = 1; i <= LOG_N; i++) {
            for (int now = 1; now <= n_nodes; now++) {
                f[now][i] = f[f[now][i - 1]][i - 1];
                w[now][i] = w[now][i - 1] + w[f[now][i - 1]][i - 1];
            }
        }
    }

    // 查詢 x 到 y 的路徑權重和
    int path(int x, int y) {
        int ans = 0;
        if (depth[x] < depth[y]) swap(x, y);
        
        for (int i = LOG_N; i >= 0; i--) {
            if (depth[f[x][i]] >= depth[y]) {
                ans += w[x][i];
                x = f[x][i];
            }
        }
        if (x == y) return ans;
        
        for (int i = LOG_N; i >= 0; i--) {
            if (f[x][i] != f[y][i]) {
                ans += (w[x][i] + w[y][i]);
                x = f[x][i];
                y = f[y][i];
            }
        }
        ans += w[x][0] + w[y][0];
        return ans;
    }

    // 查詢 x 和 y 的最近公共祖先 (LCA)
    int lca(int x, int y) {
        if (depth[x] < depth[y]) swap(x, y);
        
        for (int i = LOG_N; i >= 0; i--) {
            if (depth[f[x][i]] >= depth[y]) {
                x = f[x][i];
            }
        }
        if (x == y) return x;
        
        for (int i = LOG_N; i >= 0; i--) {
            if (f[x][i] != f[y][i]) {
                x = f[x][i];
                y = f[y][i];
            }
        }
        return f[x][0];
    }
    
    // 執行樹上差分統計
    void dfs2(int now) {
        check[now] = 1;
        for (int i = head[now]; i; i = edges[i].next) {
            int next = edges[i].to;
            if (!check[next]) {
                dfs2(next);
                dif[now] += dif[next];
                edge[i] = dif[next];
                edge[(i%2) ? (i+1) : (i-1)] = dif[next]; // 更新反向邊
            }
        }
    }

    // 執行樹上差分
    void run_tree_diff(int root) {
        memset(check, 0, sizeof(check));
        dfs2(root);
    }

} // namespace Tree

//================================================================
// 主程式 (運輸計劃 問題邏輯)
//================================================================

int n, m;
int a[N], b[N], dis[N]; // 儲存 m 條路徑的起終點和原始長度

int main() {
    cin >> n >> m;

    for (int i = 1; i <= n - 1; i++) {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);
        Tree::wirte(x, y, z);
    }

    // 1. 預處理: 建立 LCA 和路徑和所需的倍增表
    Tree::build_tables(1, n);

    // 2. 計算所有 m 條路徑的原始長度, 並找到最大值
    int maxx = 0; // 原始最長路徑
    for (int i = 1; i <= m; i++) {
        scanf("%d %d", &a[i], &b[i]);
        dis[i] = Tree::path(a[i], b[i]);
        maxx = max(maxx, dis[i]);
    }

    // 3. 找到權重最大的 *單一* 邊 (用於二分答案的下界)
    int max_single_edge = 0;
    for (int i = 1; i <= 2 * (n - 1); i++) {
        max_single_edge = max(max_single_edge, Tree::edges[i].weight);
    }

    // 4. 二分答案 (最小的最大值)
    int l = maxx - max_single_edge; 
    int r = maxx;                 
    int ans = INT_MAX;

    while (r >= l) {
        int mid = (l + r) / 2;
        int sum = 0; // 統計有多少條路徑 > mid

        // 重置樹上差分陣列
        memset(Tree::dif, 0, sizeof(Tree::dif));
        memset(Tree::edge, 0, sizeof(Tree::edge));

        // 4a. 找出所有 "超時" 的路徑, 並打上差分標記
        for (int i = 1; i <= m; i++) {
            if (dis[i] > mid) {
                Tree::dif[a[i]] += 1;
                Tree::dif[b[i]] += 1;
                Tree::dif[Tree::lca(a[i], b[i])] -= 2;
                sum++;
            }
        }

        // 4b. 檢查 `mid` 是否可行
        if (sum == 0) {
            ans = min(ans, mid);
            r = mid - 1;
            continue;
        }

        // 4c. 執行樹上差分, 統計 "公共邊"
        Tree::run_tree_diff(1);

        int max_common_edge_weight = 0;
        // 找出所有被 `sum` 條超時路徑 "共同經過" 的邊中, 權重最大的
        for (int i = 1; i <= 2 * (n - 1); i++) {
            if (Tree::edge[i] == sum) {
                max_common_edge_weight = max(max_common_edge_weight, Tree::edges[i].weight);
            }
        }

        // 4d. 判斷
        if (maxx - max_common_edge_weight <= mid) {
            ans = min(ans, mid);
            r = mid - 1; // 嘗試更小的答案
        } else {
            l = mid + 1; // `mid` 太小, 不可行
        }
    }

    cout << ans;
    return 0;
}
