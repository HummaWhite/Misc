#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
typedef long long LL;

using namespace std;

const LL INF = 0x6FFFFFFFFFFFFFFFLL;

LL dp[140000][20], n, st;
LL x[20], y[20];

inline LL dist(LL a, LL b)
{
	return abs(x[a]-x[b]) + abs(y[a]-y[b]);
}

void init()
{
	for (int s = 0; s < 140000; s++)
		for (int t = 0; t < 20; t++)
			dp[s][t] = INF;
}

int main()
{
	scanf("%lld%lld", &n, &st);
	for (int s = 0; s < n; s++) scanf("%lld%lld", &x[s], &y[s]);
	init();
	st--;
	LL ini = 1 << st;
	dp[ini][st] = 0;
	for (int s = 0; s < (1<<n); s++)
		for (int t = 0; t < n; t++)
			if ((1<<t) & s)
				for (int k = 0; k < n; k++)
					if ((1<<k) & s && k != t)
						dp[s][t] = min(dp[s][t], dp[(1<<t) ^ s][k] + dist(t, k));
	LL ans = INF;
	for (int s = 0; s < n; s++)
		if (s != st)
			ans = min(ans, dp[(1<<n)-1][s]);
	printf("%lld\n", ans);
}
