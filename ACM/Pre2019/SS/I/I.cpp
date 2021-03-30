#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
typedef long long LL;

using namespace std;

const int L = 202;
const LL mod = 1000000007;

char tmp[L];

int n, ch[L][26], f[L], tot = 1;
LL dp[L][L], val[L], ll;

void insert()
{
	int len = strlen(tmp), inc = 0;
	for (int s = 0; s < len; s++)
	{
		int u = tmp[s] - 'a';
		if (!ch[inc][u]) ch[inc][u] = tot++;
		inc = ch[inc][u];
	}
	val[inc] = 1;
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
		val[cur] |= val[f[cur]];
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

LL a[2][L][L], b[2][L][L], tmpA[2][L][L], tmpB[2][L][L];
int tc = 0, inc = 0;

void qpowA(LL k)
{
	memset(a, 0, sizeof a);
	memset(tmpA, 0, sizeof tmpA);
	tmpA[tc][0][0] = tmpA[tc][1][0] = 26, tmpA[tc][0][1] = 0, tmpA[tc][1][1] = 1;
	a[inc][0][0] = a[inc][1][1] = 1;
	while (k)
	{
		if (k & 1)
		{
			for (int s = 0; s < 2; s++)
				for (int t = 0; t < 2; t++)
					a[inc^1][s][t] = 0;
			for (int s = 0; s < 2; s++)
				for (int t = 0; t < 2; t++)
					for (int k = 0; k < 2; k++)
						a[inc^1][s][t] = (a[inc^1][s][t] + (a[inc][s][k]*tmpA[tc][k][t] % mod)) % mod;
			inc ^= 1;
		}
		for (int s = 0; s < 2; s++)
			for (int t = 0; t < 2; t++)
				tmpA[tc^1][s][t] = 0;
		for (int s = 0; s < 2; s++)
			for (int t = 0; t < 2; t++)
				for (int k = 0; k < 2; k++)
					tmpA[tc^1][s][t] = (tmpA[tc^1][s][t] + (tmpA[tc][s][k]*tmpA[tc][k][t] % mod)) % mod;
		tc ^= 1, k >>= 1;
	}
}

void qpowB(LL k)
{
	inc = tc = 0;
	memset(b, 0, sizeof b);
	memset(tmpB, 0, sizeof tmpB);
	for (int s = 0; s <= tot; s++) tmpB[tc][s][tot] = b[inc][s][s] = 1;
	for (int s = 0; s < tot; s++)
		for (int t = 0; t < tot; t++)
			tmpB[tc][s][t] += dp[s][t];
	while (k)
	{
		if (k & 1)
		{
			for (int s = 0; s <= tot; s++)
				for (int t = 0; t <= tot; t++)
					b[inc^1][s][t] = 0;
			for (int s = 0; s <= tot; s++)
				for (int t = 0; t <= tot; t++)
					for (int k = 0; k <= tot; k++)
						b[inc^1][s][t] = (b[inc^1][s][t] + (b[inc][s][k]*tmpB[tc][k][t] % mod)) % mod;
			inc ^= 1;
		}
		for (int s = 0; s <= tot; s++)
			for (int t = 0; t <= tot; t++)
				tmpB[tc^1][s][t] = 0;
		for (int s = 0; s <= tot; s++)
			for (int t = 0; t <= tot; t++)
				for (int k = 0; k <= tot; k++)
					tmpB[tc^1][s][t] = (tmpB[tc^1][s][t] + (tmpB[tc][s][k]*tmpB[tc][k][t] % mod)) % mod;
		tc ^= 1, k >>= 1;
	}
}

int main()
{
	memset(f, 0, sizeof f);
	memset(ch, 0, sizeof ch);
	scanf("%d", &n);
	for (int s = 0; s < n; s++)
	{
		scanf("%s", tmp);
		insert();
	}
	scanf("%lld", &ll);
	getnext();
	for (int s = 0; s < tot; s++)
	{
		for (int t = 0; t < 26; t++)
		{
			int u = ch[s][t];
			if (!val[u]) dp[s][u]++;
		}
	}
	qpowA(ll);
	LL res1 = a[inc][1][0];
	qpowB(ll+1);
	LL res2 = b[inc][0][tot] - 1;
	//printf("%lld %lld\n", res1, res2);
	printf("%lld\n", (res1 - res2 + mod) % mod);
}
