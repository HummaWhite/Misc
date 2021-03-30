#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
typedef long long LL;

using namespace std;

const int L = 80004;
const LL INF = 0x6fffffffffffffffLL;

LL n, m, k, st, nd;

inline LL rd()
{
	LL ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

struct edge {int v, next; LL w;} a[L<<1], b[L<<1];
struct p
{
	int id; LL len;
	bool operator < (const p x) const
	{
		return len > x.len;
	}
};

int heada[L], headb[L], tota = 0, totb = 0;

void addedge(int u, int v, LL w)
{
	a[tota].v = v, a[tota].w = w;
	a[tota].next = heada[u], heada[u] = tota++;
	b[totb].v = u, b[totb].w = w;
	b[totb].next = headb[v], headb[v] = totb++;
}

int vis[L];
LL dist[L];

void dijkstra(int x)
{
	for (int s = 0; s <= n; s++) dist[s] = INF;
	memset(vis, 0, sizeof vis);
	dist[x] = 0;
	priority_queue<p> Q;
	Q.push(p{x, 0});
	while (!Q.empty())
	{
		p tmp = Q.top(); Q.pop();
		int u = tmp.id; LL len = tmp.len;
		if (vis[u]) continue;
		vis[u] = 1;
		for (int s = headb[u]; ~s; s = b[s].next)
		{
			int v = b[s].v;
			if (dist[v] > len + b[s].w)
			{
				dist[v] = len + b[s].w;
				Q.push(p{v, dist[v]});
			}
		}
	}
}

struct anode
{
	int id; LL len;
	bool operator < (const anode x) const
	{
		return len + dist[id] > x.len + dist[x.id];
	}
};

LL ans = 0;

void BFS(int x)
{
	priority_queue<anode> Q;
	Q.push(anode{x, 0});
	while (!Q.empty())
	{
		anode tmp = Q.top(); Q.pop();
		int u = tmp.id;
		LL len = tmp.len;
		if (u == nd)
		{
			k--;
			if (k == 0)
			{
				ans = len;
				return;
			}
		}
		for (int s = heada[u]; ~s; s = a[s].next)
		{
			int v = a[s].v;
			Q.push(anode{v, len + a[s].w});
		}
	}
}

int main()
{
	memset(heada, -1, sizeof heada);
	memset(headb, -1, sizeof headb);
	n = rd(), m = rd(), k = rd();
	st = rd(), nd = rd();
	for (int s = 0; s < m; s++)
	{
		LL u, v, w;
		u = rd(), v = rd(), w = rd();
		addedge(u, v, w);
	}
	dijkstra(nd);
//	for (int s = 0; s <= n; s++) printf("%lld ", dist[s]);
	if (st == nd) k++;
	BFS(st);
	printf("%lld\n", ans);
}
