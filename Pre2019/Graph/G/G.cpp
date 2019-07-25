#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 500005;

LL pre[L], n, m, k;

struct edge
{
	LL u, v, w;
	bool operator < (const edge x) const
	{
		return w < x.w;
	}
} a[L];

inline LL rd()
{
	LL ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

int find(int x)
{
	if (x != pre[x]) pre[x] = find(pre[x]);
	return pre[x];
}

void kruskal()
{
	LL ans = 0, cnt = n;
	for (int s = 0; s <= n; s++) pre[s] = s;
	sort(a, a+m);
	for (int s = 0; s < m; s++)
	{
		if (cnt == k) break;
		int x = find(a[s].u);
		int y = find(a[s].v);
		if (x != y)
		{
			pre[y] = x;
			ans += a[s].w;
			cnt--;
		}
	}
	printf("%lld\n", ans);
}

int main()
{
	n = rd(), m = rd(), k = rd();
	for (int s = 0; s < m; s++) a[s].u = rd(), a[s].v = rd(), a[s].w = rd();
	kruskal();
}
