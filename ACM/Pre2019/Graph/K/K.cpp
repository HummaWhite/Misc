#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
typedef long long LL;

using namespace std;

const int N = 10004;
const int L = 220005;
const LL INF = 0x6fffffffffffffffLL;

struct p
{
	LL id, len, stp;
	bool operator < (const p x) const
	{
		return len > x.len;
	}
};

priority_queue<p> Q;

struct edge
{
	int v, w, next;
} a[L<<1];

int head[N], n, m, k, tot = 0;

void addedge(int u, int v, LL w)
{
	a[tot].v = v, a[tot].w = w;
	a[tot].next = head[u];
	head[u] = tot++;
}

LL dist[N][22];
int vis[N][22];

inline LL rd()
{
	LL ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

void dijkstra(int st)
{
	memset(dist, 0x6f, sizeof dist);
	dist[st][0] = 0;
	memset(vis, 0, sizeof vis);
	Q.push(p{st, 0, 0});
	while (!Q.empty())
	{
		p tmp = Q.top(); Q.pop();
		LL u = tmp.id, len = tmp.len, stp = tmp.stp;
		if (vis[u][stp]) continue;
		vis[u][stp] = 1;
		for (int s = head[u]; ~s; s = a[s].next)
		{
			int v = a[s].v;
			if (dist[v][stp] > dist[u][stp] + a[s].w)
			{
				dist[v][stp] = dist[u][stp] + a[s].w;
				Q.push(p{v, dist[v][stp], stp});
			}
			if (stp < k)
			{
				if (dist[v][stp+1] > dist[u][stp])
				{
					dist[v][stp+1] = dist[u][stp];
					Q.push(p{v, dist[v][stp+1], stp+1});
				}
			}
		}
	}
}

int main()
{
	n = rd(), m = rd(), k = rd();
	LL st, nd;
	st = rd(), nd = rd();
	memset(head, -1, sizeof head);
	for (int s = 0; s < m; s++)
	{
		LL u, v, w;
		u = rd(), v = rd(), w = rd();
		addedge(u, v, w), addedge(v, u, w);
	}
	dijkstra(st);
	LL ans = INF;
	for (int s = 0; s <= k; s++) ans = min(ans, dist[nd][s]);
	printf("%lld\n", ans);
}

