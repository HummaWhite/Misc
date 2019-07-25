#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>

using namespace std;

const int L = 10004;
const int INF = 0x6fffffff;

int n, m, st, nd;

inline int rd()
{
	int ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

struct edge
{
	int v, w, next;
} a[L<<1];

int head[L], work[L], tot = 0;

void addedge(int u, int v, int w)
{
	a[tot].v = v, a[tot].w = w;
	a[tot].next = head[u], head[u] = tot++;
}

int dep[L];

int bfs()
{
	memset(dep, -1, sizeof dep);
	queue<int> Q;
	dep[st] = 0;
	Q.push(st);
	while (!Q.empty())
	{
		int u = Q.front(); Q.pop();
		for (int s = head[u]; ~s; s = a[s].next)
		{
			int v = a[s].v;
			if (dep[v] == -1 && a[s].w)
			{
				dep[v] = dep[u] + 1;
				Q.push(v);
				if (v == nd) return 1;
			}
		}
	}
	return 0;
}

int dfs(int x, int cap)
{
	if (x == nd) return cap;
	for (int &s = work[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		if (a[s].w && dep[v] == dep[x] + 1)
		{
			int tmp = dfs(v, min(a[s].w, cap));
			if (tmp)
			{
				a[s].w -= tmp;
				a[s^1].w += tmp;
				return tmp;
			}
		}
	}
	return 0;
}

int dinic()
{
	int ans = 0, add;
	while (bfs())
	{
		for (int s = 0; s <= n; s++) work[s] = head[s];
		while (add = dfs(st, INF)) ans += add;
	}
	return ans;
}

int main()
{
	n = rd(), m = rd();
	st = 1, nd = n;
	memset(head, -1, sizeof head);
	for (int s = 0; s < m; s++)
	{
		int u, v, w;
		u = rd(), v = rd(), w = rd();
		addedge(u, v, w), addedge(v, u, 0);
	}
	printf("%d\n", dinic());
}
