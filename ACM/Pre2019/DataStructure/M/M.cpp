#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 100005;
const int INF = 1 << 30;

int n, m, fa[L][17], dep[L], siz[L], gfa[L], dis[L], vis[L];

struct edge
{
	int u, v, next;
} a[L<<1];

int head[L], tot = 0;

inline int rd()
{
	int ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

void addedge(int u, int v)
{
	a[tot].u = u, a[tot].v = v;
	a[tot].next = head[u];
	head[u] = tot++;
}

void dfs(int x, int stp)
{
	dep[x] = stp;
	for (int s = head[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		if (!dep[v])
		{
			fa[v][0] = x;
			dfs(v, stp+1);
		}
	}
}

void getfa()
{
	for (int t = 1; (1<<t) <= n; t++)
		for (int s = 1; s <= n; s++)
			fa[s][t] = fa[fa[s][t-1]][t-1];
}

int LCA(int x, int y)
{
	if (dep[x] < dep[y]) swap(x, y);
	int h = dep[x] - dep[y], inc = 0;
	while (h)
	{
		if (h & 1) x = fa[x][inc];
		h >>= 1, inc++;
	}
	if (x == y) return x;
	int H = 0;
	while ((1<<H) <= dep[x]) H++;
	for (int s = H; s >= 0; s--)
		if (fa[x][s] != fa[y][s])
			x = fa[x][s], y = fa[y][s];
	return fa[x][0];
}

int gcrt = -1, totsize, grasub = 0;

void getroot(int x, int f)
{
	siz[x] = 1;
	int maxsub = 0;
	for (int s = head[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		if (vis[v] || v == f) continue;
		getroot(v, x), siz[x] += siz[v];
		maxsub = max(maxsub, siz[v]);
	}
	maxsub = max(maxsub, totsize - siz[x]);
	if (maxsub < grasub) gcrt = x, grasub = maxsub;
}

void build(int x, int f)
{
	vis[x] = 1, gfa[x] = f;
	for (int s = head[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		if (vis[v]) continue;
		grasub = totsize = siz[v];
		getroot(v, x);
		build(gcrt, x);
	}
}

int getdist(int x, int y)
{
	int rt = LCA(x, y);
	return dep[x] + dep[y] - (dep[rt] << 1);
}

void update(int x)
{
	int u = x;
	while (~x)
	{
		dis[x] = min(dis[x], getdist(x, u));
		x = gfa[x];
	}
}

int query(int x)
{
	int ans = INF;
	int u = x;
	while (~x)
	{
		int cur = getdist(x, u) + dis[x];
		ans = min(ans, cur);
		x = gfa[x];
	}
	return ans;
}

void init()
{
	memset(head, -1, sizeof head);
	memset(dep, 0, sizeof dep);
	memset(vis, 0, sizeof vis);
	memset(gfa, -1, sizeof gfa);
	for (int s = 1; s <= n; s++) dis[s] = INF;
}

int main()
{
	n = rd(), m = rd();
	init();
	for (int s = 0; s < n-1; s++)
	{
		int u, v;
		u = rd(), v = rd();
		addedge(u, v);
		addedge(v, u);
	}
	dfs(1, 1);
	getfa();
	totsize = n, grasub = n;
	getroot(1, -1);
	build(gcrt, -1);
	update(1);
	while (m--)
	{
		int opt, x;
		opt = rd(), x = rd();
		if (opt == 1) update(x);
	}
}
