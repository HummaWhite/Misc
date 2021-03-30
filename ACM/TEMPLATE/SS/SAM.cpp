#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
typedef long long LL;

using namespace std;

const int L = 1000;

char tmp[L];

int n, ch[L][26], f[L], tot = 1, ll;
LL dp[L][L], val[L];

void insert(int v)
{
	int len = strlen(tmp), inc = 0;
	for (int s = 0; s < len; s++)
	{
		int u = tmp[s] - 'a';
		if (!ch[inc][u]) ch[inc][u] = tot++;
		inc = ch[inc][u];
	}
	val[inc] += v;
}

void getnext()
{
	queue<int> Q;
	f[0] = 0;
	for (int s = 0; s < 26; s++)
	{
		int u = ch[0][s];
		if (u) Q.push(u), f[u] = 0;
	}
	while (!Q.empty())
	{
		int cur = Q.front(); Q.pop();
		val[cur] += val[f[cur]];
		for (int s = 0; s < 26; s++)
		{
			int u = ch[cur][s];
			if (!u) ch[cur][s] = ch[f[cur]][s];
			else
			{
				int inc = f[cur];
				while (inc && !ch[inc][s]) inc = f[inc];
				f[u] = ch[inc][s];
				Q.push(u);
			}
		}
	}
}

void init()
{
	memset(f, 0, sizeof f);
	memset(ch, 0, sizeof ch);
	memset(val, 0, sizeof val);
}

int main()
{
	scanf("%d", &n);
	for (int s = 0; s < n; s++)
	{
		int v;
		scanf("%s%d", tmp, &v);
		insert(v);
	}
	scanf("%d", &ll);
	getnext();
	memset(dp, -1, sizeof dp);
	dp[0][0] = 0;
	LL ans = 0;
	for (int s = 0; s <= ll; s++)
	{
		for (int t = 0; t < tot; t++)
		{
			if (dp[s][t] == -1) continue;
			for (int k = 0; k < 26; k++)
			{
				int u = ch[t][k];
				dp[s+1][u] = max(dp[s+1][u], dp[s][t] + val[u]);
			}
		}
	}
	for (int s = 0; s <= ll; s++)
		for (int t = 0; t < tot; t++)
			ans = max(ans, dp[s][t]);
	printf("%lld\n", ans);
}
