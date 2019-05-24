#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 1005;
const int INF = 1000;

int a[L][L], dp[L][26], n, k, tot, cnt[L], vis[L][26];
char c[L];

int main()
{
	int T; scanf("%d", &T);
	while (T--)
	{
		scanf("%d", &k);
		memset(c, 0, sizeof c);
		memset(a, 0, sizeof a);
		memset(cnt, 0, sizeof cnt);
		memset(vis, 0, sizeof vis);
		scanf("%s", c);
		n = strlen(c);
		tot = n / k;
		for (int s = 0; s < tot; s++)
			for (int t = 0; t < 26; t++)
				dp[s][t] = INF;
		for (int s = 0; s < n; s++)
			if (!vis[s/k][c[s]-'a'])
				vis[s/k][c[s]-'a'] = 1, a[s/k][cnt[s/k]++] = c[s]-'a';
		for (int s = 0; s < cnt[0]; s++) dp[0][a[0][s]] = cnt[0];
		for (int s = 0; s < tot; s++)
		{
			for (int t = 0; t < cnt[s]; t++)
			{
				for (int k = 0; k < cnt[s+1]; k++)
				{
					if (cnt[s+1] == 1)
					{
						dp[s+1][a[s+1][k]] = min(dp[s+1][a[s+1][k]], dp[s][a[s][t]] + (a[s+1][k] == a[s][t] ? cnt[s+1]-1 : cnt[s+1]));
						continue;
					}
					for (int i = 0; i < cnt[s+1]; i++)
						if (i != k)
							dp[s+1][a[s+1][i]] = min(dp[s+1][a[s+1][i]], dp[s][a[s][t]] + (a[s+1][k] == a[s][t] ? cnt[s+1]-1 : cnt[s+1]));
				}
			}
		}
		int ans = INF;
		for (int s = 0; s < 26; s++) ans = min(ans, dp[tot-1][s]);
		printf("%d\n", ans);
	}
}
