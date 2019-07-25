#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
typedef long long LL;

using namespace std;

const int L = 200005;

inline LL rd()
{
	LL ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

int n, m;

struct edge
{
	int u, v, next; LL w;
} a[L<<1];

int head[L], deg[L], tot = 0;

void addedge(int u, int v, LL w)
{
	a[tot].u = u, a[tot].v = v, a[tot].w = w;
	a[tot].next = head[u], head[u] = tot++;
}

int topsort(LL w)
{
	queue<int> Q;
	while (!Q.empty()) Q.pop();
	for (int s = 1; s <= n; s++)
		if (!deg[s])
			Q.push(s);
	while (!Q.empty())
	{
		int u = Q.front(); Q.pop();
		deg[u]--;
//		printf("%d\n", u);
		for (int s = head[u]; ~s; s = a[s].next)
		{
			if (a[s].w <= w) continue;
			int v = a[s].v;
			deg[v]--;
			if (!deg[v]) Q.push(v);
		}
	}
//	printf("%lld\n", w);
//	for (int s = 1; s <= n; s++) printf("%d ", deg[s]);
	for (int s = 1; s <= n; s++)
		if (~deg[s])
			return 0;
	return 1;
}

bool check(LL w)
{
	memset(deg, 0, sizeof deg);
	for (int s = 0; s < m; s++)
		if (a[s].w > w)
			deg[a[s].v]++;
	bool res = topsort(w);
	return res;
}

int main()
{
	n = rd(), m = rd();
	memset(head, -1, sizeof head);
	LL l = 0, r = 0;
	for (int s = 0; s < m; s++)
	{
		int u, v; LL w;
		u = rd(), v = rd(), w = rd();
		addedge(u, v, w);
		r = max(r, w);
	}
	while (l < r)
	{
		LL m = (l+r) >> 1;
		if (check(m)) r = m;
		else l = m + 1;
	}
	printf("%lld\n", l);
}
