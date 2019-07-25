#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>

using namespace std;

const int L = 300005;
const int INF = 0x6fffffff;

struct edge
{
	int u, v, next;
} e[L];

int head[L], tot = 0;
int a, b, n, m;

void addedge(int u, int v)
{
	e[tot].u = u, e[tot].v = v;
	e[tot].next = head[u], head[u] = tot++;
}

inline int rd()
{
	int ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

int vis[L], fr[L], da[L], db[L], ma[L], mb[L], dis;

int searchP()
{
	queue<int> Q;
	memset(da, -1, sizeof da);
	memset(db, -1, sizeof db);
	dis = INF;
	for (int s = 1; s <= a; s++)
		if (ma[s] == -1)
			Q.push(s), da[s] = 0;
	while (!Q.empty())
	{
		int u = Q.front(); Q.pop();
		if (da[u] > dis) break;
		for (int s = head[u]; ~s; s = e[s].next)
		{
			int v = e[s].v;
			if (~db[v]) continue;
			db[v] = da[u] + 1;
			if (mb[v] == -1) dis = db[v];
			else da[mb[v]] = db[v] + 1, Q.push(mb[v]);
		}
	}
	return dis != INF;
}

int dfs(int u)
{
	for (int s = head[u]; ~s; s = e[s].next)
	{
		int v = e[s].v;
		if (!vis[v] && db[v] == da[u]+1)
		{
			vis[v] = 1;
			if (mb[v] != -1 && db[v] == dis) continue;
			if (mb[v] == -1 || dfs(mb[v]))
			{
				mb[v] = u, ma[u] = v;
				return 1;
			}
		}
	}
	return 0;
}

int match()
{
	int ans = 0;
	memset(ma, -1, sizeof ma);
	memset(mb, -1, sizeof mb);
	while (searchP())
	{
		memset(vis, 0, sizeof vis);
		for (int s = 1; s <= a; s++)
			if (ma[s] == -1 && dfs(s))
				ans++;
	}
	printf("%d\n", ans);
}

int main()
{
	a = rd(), b = rd(), m = rd();
	n = a + b;
	memset(head, -1, sizeof head);
	for (int s = 0; s < m; s++)
	{
		int u, v;
		u = rd(), v = rd();
		addedge(u, v);
	}
	match();
}
