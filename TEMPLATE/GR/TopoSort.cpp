#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>

using namespace std;

const int L = 1005;

struct edge
{
	int u, v, next;
} a[L<<4];

int head[L], deg[L], cnt[L], pre[L], vis[L], dep[L], n, m, tot = 0;
int p[L], num = 0;
int Q[L], tail = 0;
int map[L][L];

void addedge(int u, int v)
{
	a[tot].u = u;
	a[tot].v = v, a[tot].next = head[u];
	head[u] = tot++;
}

int find(int x)
{
	if (x != pre[x]) pre[x] = find(pre[x]);
	return pre[x];
}

bool flg = 0;

void toposort()
{
	queue<int> Q;
	memset(vis, 0, sizeof vis);
	memset(dep, 0, sizeof dep);
	for (int s = 1; s <= n; s++)
		if (!vis[find(s)])
			vis[find(s)] = 1, p[num++] = find(s);
	for (int s = 0; s < num; s++)
		if (!deg[p[s]])
			Q.push(p[s]);
	int pn = 0;
	while (!Q.empty())
	{
		int u = Q.front(); Q.pop();
		deg[u]--, pn++;
		for (int s = head[u]; ~s; s = a[s].next)
		{
			int v = a[s].v;
			deg[v]--;
			dep[v] = max(dep[v], dep[u]+1);
			if (!deg[v]) Q.push(v);
		}
	}
	if (pn != num) { printf("-1\n"); return; }
	int ans = 0;
	for (int s = 0; s < num; s++) ans += (dep[p[s]]+1) * cnt[p[s]];
	printf("%d\n", ans);
}

int main()
{
	scanf("%d%d", &n, &m);
	memset(head, -1, sizeof head);
	memset(deg, 0, sizeof deg);
	for (int s = 0; s < L; s++) pre[s] = s, cnt[s] = 1;
	for (int s = 0; s < m; s++)
	{
		int opt, u, v; scanf("%d%d%d", &opt, &u, &v);
		if (opt == 3)
		{
			int x = find(u), y = find(v);
			if (x != y)
			{
				pre[y] = x;
				cnt[x] += cnt[y];
			}
			continue;
		}
		if (opt == 1) swap(u, v);
		addedge(find(u), find(v));
	}
	for (int s = 0; s < tot; s++)
		if (find(a[s].u) == find(a[s].v))
			flg = 1;
	if (flg) { printf("-1\n"); return 0; }
	memset(map, 0, sizeof map);
	for (int s = 0; s < tot; s++)
		if (!map[find(a[s].u)][find(a[s].v)])
			map[find(a[s].u)][find(a[s].v)] = 1, deg[find(a[s].v)]++;
	toposort();
}
