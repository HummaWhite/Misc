#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
#include<cmath>

using namespace std;

const int L = 65537;
const int INF = 0x6fffffff;

int n, m, k, all;
int st[L], stt[L], cnt = 0;
int x[17], y[17];

void pr(int d)
{
	for (int s = 0; s <= n; s++)
		printf("%d", (d>>s) & 1);
	printf("\n");
}

int cntBit(int d)
{
	int ret = 0;
	while (d) d -= d&-d, ret++;
	return ret;
}

struct edge
{
	int v, w, next;
} a[L<<1];

int head[L], tot = 0;

void addedge(int u, int v, int w)
{
	a[tot].v = v, a[tot].w = w;
	a[tot].next = head[u], head[u] = tot++;
}

int check(int d)
{
	for (int s = 0; s < m; s++)
		if ((d & (1<<x[s])) && (d & (1<<y[s])))
			return 0;
	return 1;
}

int dis[L], vis[L];

void SPFA(int x)
{
	memset(vis, 0, sizeof vis);
	for (int s = 0; s < L; s++) dis[s] = INF;
	vis[x] = 1, dis[x] = 0;
	queue<int> Q;
	Q.push(x);
	while (!Q.empty())
	{
		int u = Q.front(); Q.pop();
		for (int s = head[u]; ~s; s = a[s].next)
		{
			int v = a[s].v;
			if (dis[v] > dis[u] + a[s].w)
			{
				dis[v] = dis[u] + a[s].w;
				if (!vis[v])
				{
					vis[v] = 1;
					Q.push(v);
				}
			}
		}
	}
}

int main()
{
	scanf("%d%d%d", &n, &m, &k);
	all = 1 << (n+1), all--;
	for (int s = 0; s < m; s++) scanf("%d%d", &x[s], &y[s]);
	memset(head, -1, sizeof head);
	memset(stt, 0, sizeof stt);
	for (int s = 0; s < 1 << (n+1); s++)
	{
		if (s & 1)
		{
			if (!check(all-s)) continue;
		}
		else if (!check(s)) continue;
		st[cnt++] = s, stt[s] = 1;
	}
	//for (int s = 0; s < cnt; s++) pr(st[s]);
	//printf("%d\n", cnt);
	for (int s = 0; s < cnt; s++)
		for (int t = 0; t < cnt; t++)
		{
			int d = st[s] ^ st[t];
			if (!(d & 1)) continue;
			if (cntBit(d) > k) continue;
			if (abs(st[s] - st[t]) != d) continue;
			addedge(st[s], st[t], cntBit(d));
			addedge(st[t], st[s], cntBit(d));
		}
	SPFA(all);
	if (dis[0] == INF) printf("mole\n");
	else printf("%d\n", dis[0]);
}
