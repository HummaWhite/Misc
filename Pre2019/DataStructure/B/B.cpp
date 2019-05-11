#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 200005;
const int INF = 0x6FFFFFFF;

int fa[L][20], ls[L][20], dep[L], head[L], tot = 0;
int n, q;

struct edge
{
	int u, v, w, next;
} a[L];

inline int rd()
{
	int ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret * 10 + c - '0', c = getchar();
	return ret;
}

void addedge(int u, int v, int w)
{
	a[tot].v = v, a[tot].w = w;
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
			ls[v][0] = a[s].w;
			dfs(v, stp+1);
		}
	}
}

void getfa()
{
	for (int t = 1; t <= 17; t++)
		for (int s = 1; s <= n; s++)
		{
			fa[s][t] = fa[fa[s][t-1]][t-1];
			ls[s][t] = min(ls[s][t-1], ls[fa[s][t-1]][t-1]);
		}
}

int LCA(int x, int y)
{
	int ans = INF;
	if (dep[x] < dep[y]) swap(x, y);
	int h = dep[x] - dep[y];
	int inc = 0;
	while (h)
	{
		if (h & 1) x = fa[x][inc];
		inc++, h >>= 1;
	}
	if (x == y) return x;
	int H = 0;
	while ((1<<H) <= dep[x]) H++;
	for (int s = H; s >= 0; s--)
		if (fa[x][s] != fa[y][s])
			x = fa[x][s], y = fa[y][s];
	return fa[x][0];
}

void init()
{
	memset(head, -1, sizeof head);
	memset(dep, 0, sizeof dep);
	memset(fa, 0, sizeof fa);
	for (int s = 0; s <= n; s++)
		for (int t = 0; t <= 17; t++)
			ls[s][t] = INF;
}

int query(int x, int y)
{
	int ans = INF;
	if (dep[x] < dep[y]) swap(x, y);
	int h = dep[x] - dep[y];
	int inc = 0;
	while (h)
	{
		if (h & 1)
		{
			ans = min(ans, ls[x][inc]);
			x = fa[x][inc];
		}
		inc++, h >>= 1;
	}
	return ans;
}

int main()
{
	n = rd(), q = rd();
	init();
	for (int s = 0; s < n-1; s++)
	{
		int u, v, w;
		u = rd(), v = rd(), w = rd();
		addedge(u, v, w);
		addedge(v, u, w);
	}
	dfs(1, 1);
	getfa();
	while (q--)
	{
		int u, v;
		u = rd(), v = rd();
		int rt = LCA(u, v);
		printf("%d\n", min(query(u, rt), query(v, rt)));
	}
}
