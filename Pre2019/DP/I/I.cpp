#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 5005;

LL dp[L][L], n, m, k, x, y;

inline LL rd()
{
	LL ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

struct mtsu
{
	LL w, v, d;
	bool operator < (const mtsu b) {return d < b.d;}
} a[L];

int main()
{
	n = rd(), m = rd(), k = rd(), x = rd(), y = rd();
	for (int s = 1; s <= n; s++)
	{
		a[s].w = rd();
		LL tb, tc; tb = rd(), tc = rd();
		a[s].v = (tc - tb) * y;
		if (a[s].v < 0) a[s].v = 0;
		a[s].d = (rd() - k) * x;
		if (a[s].d < 0) a[s].d = 0;
	}
	sort(a+1, a+n+1);
	LL ans = 0;
	memset(dp, 0, sizeof dp);
	for (int s = 1; s <= n; s++)
	{
		for (int t = 0; t <= m-a[s].d; t++)
		{
			if (t < a[s].v) dp[s][t] = dp[s-1][t];
			else dp[s][t] = max(dp[s-1][t], dp[s-1][t-a[s].v] + a[s].w);
			ans = max(ans, dp[s][t]);
		}
	}
	printf("%lld\n", ans);
}
