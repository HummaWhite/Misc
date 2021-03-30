#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
#include<map>

using namespace std;

const int L = 600005;
const int INF = 0x3fffffff;

map<pair<int, int>, int> mp;
map<pair<int, int>, int>::iterator iter;

struct p
{
	int id,  len;
	bool operator < (const p x) const { return len > x.len; }
};

struct edge
{
	int v, w, next;
} a[L<<1];

int head[L], n, m, tot = 0, cnt = 0;

void addedge(int u, int v, int w)
{
	a[tot].v = v, a[tot].w = w;
	a[tot].next = head[u], head[u] = tot++;
}

inline int rd()
{
	int ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

int dis[L], vis[L];

void dijkstra(int x)
{
	for (int s = 0; s < L ;s++) dis[s] = INF;
	memset(vis, 0, sizeof vis);
	dis[x] = 0;
	priority_queue<p> Q;
	Q.push(p{x, 0});
	while (!Q.empty())
	{
		int u = Q.top().id, len = Q.top().len; Q.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (int s = head[u]; ~s; s = a[s].next)
		{
			int v = a[s].v;
			if (dis[v] > len + a[s].w)
			{
				dis[v] = len + a[s].w;
				Q.push(p{v, dis[v]});
			}
		}
	}
	printf("%d\n", dis[n] == INF<<1 ? -1 : (dis[n]>>1));
}

int main()
{
	n = rd(), m = rd();
	memset(head, -1, sizeof head);
	for (int s = 0; s < m; s++)
	{
		int u, v, c;
		u = rd(), v = rd(), c = rd();
		iter = mp.find(make_pair(u, c));
		int pu, pv;
		if (iter == mp.end())
		{
			cnt++;
			mp.insert(make_pair(make_pair(u, c), n+cnt));
			pu = n+cnt;
		}
		else pu = iter->second;
		addedge(u, pu, 1), addedge(pu, u, 1);
		iter = mp.find(make_pair(v, c));
		if (iter == mp.end())
		{
			cnt++;
			mp.insert(make_pair(make_pair(v, c), n+cnt));
			pv = n+cnt;
		}
		else pv = iter->second;
		addedge(pv, v, 1), addedge(v, pv, 1);
		addedge(pu, pv, 0), addedge(pv, pu, 0);
	}
	dijkstra(1);
}
