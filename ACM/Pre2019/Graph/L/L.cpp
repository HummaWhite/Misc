#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
typedef long long LL;

using namespace std;

const int L = 200005;
const LL INF = 0x2fffffffffffffffLL;

int n, m;
LL p[L], val[L], all = 0;

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
	int v, next;
	LL w;
} a[L<<1];

int head[L], work[L], tot = 0;

void addedge(int u, int v, LL w)
{
	a[tot].v = v, a[tot].w = w;
	a[tot].next = head[u], head[u] = tot++;
}

int dep[L];

int bfs()
{
	memset(dep, -1, sizeof dep);
	queue<int> Q;
	dep[0] = 0;
	Q.push(0);
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
				if (v == n+m+1) return 1;
			}
		}
	}
	return 0;
}

LL dfs(int x, LL cap)
{
	if (x == n+m+1) return cap;
	for (int &s = work[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		if (a[s].w && dep[v] == dep[x] + 1)
		{
			LL tmp = dfs(v, min(a[s].w, cap));
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

LL dinic()
{
	LL ret = 0, add;
	while (bfs())
	{
		for (int s = 0; s <= m+n+1; s++) work[s] = head[s];
		while (add = dfs(0, INF<<1)) ret += add;
	}
	return ret;
}

int main()
{
	n = rd(), m = rd();
	memset(head, -1, sizeof head);
	for (int s = 1; s <= m; s++)
	{
		int k; k = rd();
		addedge(n+s, n+m+1, k);
		addedge(n+m+1, n+s, 0);
	}
	for (int s = 1; s <= n; s++)
	{
		int k, num;
		k = rd(), num = rd();
		all += k;
		addedge(0, s, k);
		addedge(s, 0, 0);
		for (int t = 0; t < num; t++)
		{
			int v; v = rd();
			addedge(s, v+n, INF);
			addedge(v+n, s, 0);
		}
	}
	LL ans = all - dinic();
	printf("%lld\n", ans);
}
