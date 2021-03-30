#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
typedef long long LL;

using namespace std;

const int L = 200050;
const LL INF = 0x6fffffffffffffLL;

LL n, m, od[L], T;
LL fa[L][30], dep[L], sum[L];
bool vis[L], vv[L];

struct p
{
	LL id, len;
	bool operator < (const p x) const { return len > x.len; }
};

struct edge
{
	LL u, v, next, sel, w;
} a[L<<1];

LL head[L], tot = 0;

void addedge(LL u, LL v, LL w)
{
	a[tot].sel = 0;
	a[tot].u = u, a[tot].v = v, a[tot].w = w;
	a[tot].next = head[u], head[u] = tot++;
}

void dfs(int x, int stp)
{
	if (dep[x]) return;
	dep[x] = stp;
	for (int s = head[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		if (!dep[v])
		{
			fa[v][0] = x;
			sum[v] = sum[x] + a[s].w;
			dfs(v, stp+1);
			a[s].sel = a[s^1].sel = 1;
		}
	}
}

void getfa()
{
	for (int t = 1; t <= 18; t++)
		for (int s = 1; s <= n; s++)
			fa[s][t] = fa[fa[s][t-1]][t-1];
}

LL lca(LL x, LL y)
{
	if (dep[x] < dep[y]) swap(x, y);
	LL h = dep[x] - dep[y], inc = 0;
	while (h)
	{
		if (h & 1) x = fa[x][inc];
		h >>= 1, inc++;
	}
	if (x == y) return x;
	LL H = 0;
	while ((1<<H) <= dep[x]) H++;
	for (int s = H; s >= 0; s--)
		if (fa[x][s] != fa[y][s])
			x = fa[x][s], y = fa[y][s];
	return fa[x][0];
}

LL query(LL x, LL y)
{
	LL rt = lca(x, y);
	return sum[x] + sum[y] - sum[rt]*2;
}

LL dis[80][L];

void dijkstra(LL x, LL d)
{
	memset(vis, 0, sizeof vis);
	dis[d][x] = 0;
	priority_queue<p> Q;
	while (!Q.empty()) Q.pop();
	Q.push(p{x, 0});
	while (!Q.empty())
	{
		p tmp = Q.top(); Q.pop();
		LL u = tmp.id, len = tmp.len;
		if (vis[u]) continue;
		vis[u] = 1;
		for (int s = head[u]; ~s; s = a[s].next)
		{
			LL v = a[s].v;
			if (dis[d][v] > len + a[s].w)
			{
				dis[d][v] = len + a[s].w;
				Q.push(p{v, dis[d][v]});
			}
		}
	}
}

void init()
{
	memset(head, -1, sizeof head);
	memset(dis, 0x3f, sizeof dis);
	memset(fa, -1, sizeof fa);
}

int main()
{
	/*freopen("in.txt", "r", stdin);
	freopen("out3.txt", "w", stdout);*/
	scanf("%lld%lld", &n, &m);
	init();
	for (int s = 0; s < m; s++)
	{
		LL u, v, w;
		scanf("%lld%lld%lld", &u, &v, &w);
		addedge(u, v, w), addedge(v, u, w);
	}
	scanf("%lld", &T);
	od[0] = 1;
	for (int s = 1; s <= T; s++) scanf("%d", &od[s]);
	dfs(1, 1); getfa();
	int cnt = 0;
	for (int s = 0; s < tot; s++)
	{
		if (a[s].sel) continue;
		if (!vv[a[s].u]) dijkstra(a[s].u, cnt++), vv[a[s].u] = 1;
		if (!vv[a[s].v]) dijkstra(a[s].v, cnt++), vv[a[s].v] = 1;
	}
	LL ans = 0;
	for (int s = 0; s < T; s++)
	{
		LL u = od[s], v = od[s+1];
		LL cur = query(u, v);
		for (int t = 0; t < cnt; t++)
			cur = min(cur, dis[t][u] + dis[t][v]);
		//printf("%lld\n", cur);
		ans += cur;
	}
	printf("%lld\n", ans);
}
