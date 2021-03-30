#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 1000020;

char S[L], T[L], tm[L];
int lenS, lenT;

struct PAM
{
	PAM()
	{
		memset(f, 0, sizeof f);
		memset(ch, 0, sizeof ch);
	}
	int ch[L][26], f[L], len[L], lst, tot;
	LL num[L], cnt[L];
	int find(int s, int x)
	{
		while (S[s-len[x]-1] != S[s]) x = f[x];
		return x;
	}
	void build()
	{
		memset(num, 0, sizeof num);
		f[0] = 1, f[1] = 0;
		len[0] = 0, len[1] = -1;
		tot = 1, lst = 0;
		for (int s = 1; s <= lenS; s++)
		{
			int u = S[s] - 'a';
			int inc = find(s, lst);
			if (!ch[inc][u])
			{
				len[++tot] = len[inc] + 2;
				int pre = find(s, f[inc]);
				f[tot] = ch[pre][u];
				ch[inc][u] = tot;
				num[tot] = num[f[tot]] + 1;
			}
			lst = ch[inc][u];
			cnt[s] = num[lst];
		}
	}
} pam;

LL nex[L], ext[L];

void getnext()
{
	nex[1] = lenT;
	int inc = 1, mr = 2;
	while (inc < lenT && T[inc] == T[inc+1]) inc++;
	nex[2] = inc - 1;
	for (int s = 3; s <= lenT; s++)
	{
		int p = mr + nex[mr] - 1, q = nex[s-mr+1];
		if (q < p-s+1) nex[s] = q;
		else
		{
			inc = max(p-s+1, 0);
			while (s + inc <= lenT && T[s+inc] == T[inc+1]) inc++;
			nex[s] = inc, mr = s;
		}
	}
}

void exkmp()
{
	getnext();
	int mr = 1, mlen = min(lenS, lenT);
	while (mr <= mlen && S[mr] == T[mr]) mr++;
	ext[1] = mr-1, mr = 1;
	for (int s = 2; s <= lenS; s++)
	{
		int p = mr + ext[mr] - 1, q = nex[s-mr+1];
		if (q < p-s+1) ext[s] = q;
		else
		{
			int inc = max(p-s+1, 0);
			while (s + inc <= lenS && inc+1 <= lenT && S[s+inc] == T[inc+1]) inc++;
			ext[s] = inc, mr = s;
		}
	}
}

int main()
{
	scanf("%s%s", tm+1, T+1);
	lenS = strlen(tm+1), lenT = strlen(T+1);
	for (int s = 1; s <= lenS; s++) S[s] = tm[lenS-s+1];
	memset(nex, 0, sizeof nex);
	memset(ext, 0, sizeof ext);
	pam.build();
	getnext(), exkmp();
	LL ans = 0;
	//for (int s = 1; s <= lenS; s++) printf("%d\n", pam.cnt[s-1]);
	for (int s = 1; s <= lenS; s++) ans += (LL)((LL)ext[s] * pam.cnt[s-1]);
	printf("%lld\n", ans);
}
