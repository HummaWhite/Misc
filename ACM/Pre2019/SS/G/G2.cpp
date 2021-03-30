#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 1000006;

char S[L];

int ch[L][10], f[L], len[L], lenS, lst, tot;
LL sum[L], p10[L], val[L], T;

LL getsum(int l, int r)
{
	return (sum[r] - (sum[l-1] * p10[r-l+1] % T) + T) % T;
}

int find(int s, int x)
{
	while (S[s-len[x]-1] != S[s]) x = f[x];
	return x;
}

void buildPAM()
{
	f[0] = 1, f[1] = 0;
	len[0] = 0, len[1] = -1;
	val[0] = val[1] = 0;
	tot = 2, lst = 0;
	for (int s = 1; s <= lenS; s++)
	{
		int u = S[s] - '0';
		int inc = find(s, lst);
		if (!ch[inc][u])
		{
			len[tot] = len[inc] + 2;
			int pre = find(s, f[inc]);
			f[tot] = ch[pre][u];
			ch[inc][u] = tot;
			val[tot] = u;
			tot++;
		}
		lst = ch[inc][u];
	}
}

LL ans = 0;

void dfs(int x, LL curval)
{
	if (len[x] == 1) curval = val[x];
	else curval = (curval*10 + val[x] + (val[x]*p10[len[x]-1] % T)) % T;
	//printf("%d %d %d\n", x, len[x], curval);
	ans = (ans + curval) % T;
	for (int s = 0; s < 10; s++)
		if (ch[x][s])
			dfs(ch[x][s], curval);
}

void init()
{
	S[0] = '#';
	lenS = strlen(S)-1;
	sum[0] = S[1] - '0', p10[0] = 1;
	for (int s = 1; s < lenS; s++)
	{
		sum[s] = (sum[s-1]*10 + S[s+1] - '0') % T;
		p10[s] = (p10[s-1] * 10) % T;
	}
	memset(f, 0, sizeof f);
	memset(ch, 0, sizeof ch);
	memset(len, 0, sizeof len);
}

int main()
{
	scanf("%s%lld", S+1, &T);
	init();
	buildPAM();
	dfs(0, 0);
	dfs(1, 0);
	printf("%lld\n", ans);
}
