#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 200005;

int n, m, pre[L];

struct edge
{
	int u, v, sel; LL w;
	edge() { sel = 0; }
	bool operator < (const edge x) const { return w < x.w; }
} a[L];

int find(int x)
{
	if (x != pre[x]) pre[x] = find(pre[x]);
	return pre[x];
}

LL ans = 0, cnt = 0, maxw = 0;

void kruskal()
{
	for (int s = 1; s <= n; s++) pre[s] = s;
	sort(a, a+m);
	for (int s = 0; s < m; s++)
	{
		int x = find(a[s].u), y = find(a[s].v);
		if (x != y)
		{
			pre[y] = x;
			ans += a[s].w;
			maxw = max(maxw, a[s].w);
		}
	}
	ans -= maxw;
	for (int s = 1; s <= n; s++) pre[s] = s;
	for (int s = 0; s < m; s++)
	{
		if (a[s].w == maxw) break;
		int x = find(a[s].u), y = find(a[s].v);
		if (x != y)
		{
			pre[y] = x;
			a[s].sel = 1;
		}
	}
	for (int s = 0; s < m; s++)
	{
		if (a[s].sel) continue;
		if (find(a[s].u) != find(a[s].v)) cnt++;
	}
	printf("%lld %lld\n", ans, cnt);
}

int main()
{
	scanf("%d%d", &n, &m);
	for (int s = 0; s < m; s++) scanf("%d%d%lld", &a[s].u, &a[s].v, &a[s].w);
	kruskal();
}
