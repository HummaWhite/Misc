#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;
typedef double D;

using namespace std;

const int L = 102;
const LL mod = 1000000007;

LL dp[L][L][2], tmp[L][L][2], res = 1, base = 1000;

int n, m, inc = 0, cur = 0;
D x, y;

void qpow(int b)
{
	while (b)
	{
		if (b & 1)
		{
			for (int s = 0; s < m; s++)
				for (int t = 0; t < m; t++)
					dp[s][t][cur^1] = 0;
			for (int s = 0; s < m; s++)
				for (int t = 0; t < m; t++)
					for (int k = 0; k < m; k++)
						dp[s][t][cur^1] = (dp[s][t][cur^1] + (dp[s][k][cur] * tmp[k][t][inc] % mod)) % mod;
			res = (res*base) % mod;
			cur ^= 1;
		}
		for (int s = 0; s < m; s++)
			for (int t = 0; t < m; t++)
				tmp[s][t][inc^1] = 0;
		for (int s = 0; s < m; s++)
			for (int t = 0; t < m; t++)
				for (int k = 0; k < m; k++)
					tmp[s][t][inc^1] = (tmp[s][t][inc^1] + (tmp[s][k][inc] * tmp[k][t][inc] % mod)) % mod;
		base = (base*base) % mod;
		b >>= 1, inc ^= 1;
	}
}

int main()
{
	scanf("%d%d", &n, &m);
	scanf("%lf%lf", &x, &y);
	memset(dp, 0, sizeof dp);
	memset(tmp, 0, sizeof tmp);
	if (n == 0)
	{
		printf("0\n");
		return 0;
	}
	for (int s = 0; s < m; s++)
	{
		dp[s][s][cur] = 1;
		tmp[s][s][inc] = (LL)(1000 - 1000*x - (m-s)*(LL)(1000*y));
		for (int t = s-1; t < s; t++)
			if (t >= 0)
				tmp[s][t][inc] = (LL)((m-t) * (LL)(1000*y));
	}
	qpow(n);
	LL ans = 0;
	for (int s = 0; s < m; s++) ans = (ans+dp[s][0][cur]) % mod;
	ans = (res - ans + mod * 10) % mod;
	printf("%lld\n", ans);
}
