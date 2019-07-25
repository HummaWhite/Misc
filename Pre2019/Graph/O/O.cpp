#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
typedef double D;

const D eps = 0.0000001;

using namespace std;

const int L = 1005;
const D DINF = 100000000.0;

int vis[L], pre[L], n;
D x[L], y[L], z[L], dis[L];

double hori(int a, int b) { return sqrt( (D)(x[a]-x[b])*(x[a]-x[b]) + (D)(y[a]-y[b])*(y[a]-y[b]) ); }

double vert(int a, int b) { return abs(z[a] - z[b]); }

D prim(D r)
{
	memset(vis, 0, sizeof vis);
	for (int s = 1; s <= n; s++)
	{
		dis[s] = vert(1, s) - r*hori(1, s);
		pre[s] = 1;
	}
	dis[1] = 0, vis[1] = 1;
	D sumh = 0, sumv = 0;
	for (int s = 1; s <= n; s++)
	{
		int v = 0; D minval = DINF;
		for (int t = 1; t <= n; t++)
			if (!vis[t] && dis[t] < minval)
				minval = dis[t], v = t;
		vis[v] = 1;
		sumh += hori(pre[v], v);
		sumv += vert(pre[v], v);
		for (int t = 1; t <= n; t++)
		{
			D nw = vert(v, t) - r*hori(v, t);
			if (!vis[t] && dis[t] > nw)
				dis[t] = nw, pre[t] = v;
		}
	}
	return sumv / sumh;
}

void solve()
{
	D r = 0, nr = DINF;
	while (1)
	{
		nr = prim(r);
		//printf("%.14lf %.14lf\n", r, nr);
		if (abs(nr-r) < eps) break;
		r = nr;
	}
	printf("%.3lf\n", r);
}

int main()
{
	scanf("%d", &n);
	for (int s = 1; s <= n; s++) scanf("%lf%lf%lf", &x[s], &y[s], &z[s]);
	solve();
}
