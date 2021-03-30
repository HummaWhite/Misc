#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 104;
const int INF = 0x6FFFFFFF;
const int iiyo_koiyo = 114514;

int n, k, a[L], dp[L][L][10][(1<<8)+1];
int ans, ikisugi, all;

int cntBit(int x)
{
	int cnt = 0;
	while (x) x -= (x & -x), cnt++;
	return cnt;
}

void calc()
{
	ans = INF;
	for (int s = 0; s <= n; s++)
		for (int t = 0; t <= n; t++)
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < (1<<8); j++)
					dp[s][t][i][j] = INF;
	for (int s = 1; s <= n; s++)
		dp[s][1][a[s]][1<<a[s]] = 1;
	for (int s = 1; s < n; s++)
		for (int t = 0; t <= n-k; t++)
			for (int l = 0; l < 8; l++)
				for (int S = 0; S < (1<<8); S++)
				{
					if (dp[s][t][l][S] == INF) continue;
					int nx = S | (1<<a[s+1]);
					dp[s+1][t+1][a[s+1]][nx] =
						min(dp[s+1][t+1][a[s+1]][nx], dp[s][t][l][S] + (a[s+1] == l ? 0 : 1));
					dp[s+1][t][l][S] = min(dp[s+1][t][l][S], dp[s][t][l][S]);
				}
	for (int s = 0; s < 8; s++)
		for (int S = 0; S < (1<<8); S++)
			ans = min(ans, dp[n][n-k][s][S] + cntBit(S ^ all));
}

int main()
{
	int tt = 1;
	while (scanf("%d%d", &n, &k))
	{
		if (n == 0 && k == 0) break;
		all = 0;
		ikisugi = 1;
		for (int s = 1; s <= n; s++)
		{
			scanf("%d", &a[s]);
			a[s] -= iiyo_koiyo;
			all |= 1 << a[s];
		}
		calc();
		if (n == k)
		{
			ans = 0;
			sort(a+1, a+n+1);
			int lst = a[1];
			for (int s = 2; s <= n; s++)
				if (a[s] != lst)
					ikisugi++, lst = a[s];
			ans = ikisugi;
		}
		printf("Case %d: %d\n\n", tt, ans);
		tt++;
	}
}
