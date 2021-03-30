#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>

using namespace std;

const int L = 3004;

struct edge
{
	int u, v, next;
} a[L<<1], b[L<<1];

int head[L], first[L], tot = 0, cnt = 0, n, m, st, num;
int w[L], p[L], pre[L], val[L];

void addedge(int u, int v)
{
	a[tot].u = u, a[tot].v = v;
	a[tot].next = head[u], head[u] = tot++;
}

void add(int u, int v)
{
	b[cnt].u = u, b[cnt].v = v;
	b[cnt].next = first[u]; first[u] = cnt++;
}

int dfn[L], low[L], vis[L], tm = 0, dep = 0;
int stk[L], top = 0, all = 0;

void tarjan(int x)
{
	dfn[x] = low[x] = ++dep;
	stk[++top] = x, vis[x] = 1;
	for (int s = head[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		if (!dfn[v])
		{
			tarjan(v);
			low[x] = min(low[x], low[v]);
		}
		else if (vis[v]) low[x] = min(low[x], dfn[v]);
	}
	int cur = 0;
	if (low[x] == dfn[x])
	{
		all++;
		while (cur != x)
		{
			cur = stk[top--];
			pre[cur] = all;
			val[all] += w[cur];
			vis[cur] = 0;
		}
	}
}

int dis[L];

void SPFA(int x)
{
	queue<int> Q;
	memset(dis, 0, sizeof dis);
	memset(vis, 0, sizeof vis);
	dis[x] = val[x], vis[x] = 1;
	Q.push(x);
	while (!Q.empty())
	{
		int u = Q.front(); Q.pop();
		vis[u] = 0;
		for (int s = first[u]; ~s; s = b[s].next)
		{
			int v = b[s].v;
			if (dis[v] < dis[u] + val[v])
			{
				dis[v] = dis[u] + val[v];
				if (!vis[v])
				{
					Q.push(v);
					vis[v] = 1;
				}
			}
		}
	}
}

int main()
{
	scanf("%d%d", &n, &m);
	memset(head, -1, sizeof head);
	memset(first, -1, sizeof first);
	for (int s = 0; s < m; s++)
	{
		int u, v; scanf("%d%d", &u, &v);
		addedge(u, v);
	}
	for (int s = 1; s <= n; s++) scanf("%d", &w[s]);
	scanf("%d%d", &st, &num);
	memset(p, 0, sizeof p);
	for (int s = 0; s < num; s++)
	{
		int x; scanf("%d", &x);
		p[x] = 1;
	}
	memset(vis, 0, sizeof vis);
	memset(val, 0, sizeof val);
	tarjan(st);
	//for (int s = 1; s <= all; s++) printf("%d ", val[s]);
	for (int s = 0; s < tot; s++)
		if (pre[a[s].u] != pre[a[s].v])
			add(pre[a[s].u], pre[a[s].v]);
	SPFA(pre[st]);
	int ans = 0;
	for (int s = 1; s <= n; s++)
		if (p[s])
			ans = max(ans, dis[pre[s]]);
	printf("%d\n", ans);
}
