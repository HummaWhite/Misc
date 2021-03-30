#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 1000006;

int ch[L][26], len[L], fa[L], np, lst = 1, tot = 1;

char A[L], B[L];

void add(int c)
{
	int p, q, np, nq;
	p = lst, lst = np = ++tot;
	len[np] = len[p] + 1;
	while (!ch[p][c] && p) ch[p][c] = np, p = fa[p];
	if (!p) fa[np] = 1;
	else
	{
		q = ch[p][c];
		if (len[q] == len[p] + 1) fa[np] = q;
		else
		{
			nq = ++tot;
			len[nq] = len[p] + 1;
			for (int s = 0; s < 26; s++) ch[nq][s] = ch[q][s];
			fa[nq] = fa[q], fa[q] = fa[np] = nq;
			while (ch[p][c] == q) ch[p][c] = nq, p = fa[p];
		}
	}
}

void init()
{
	memset(ch, 0, sizeof ch);
}

int main()
{
	init();
	scanf("%s", A);
	scanf("%s", B);
	int lenA = strlen(A), lenB = strlen(B);
	for (int s = 0; s < lenA; s++) add(A[s] - 'a');
	int ans = 0, inc = 1, tmp = 0;
	for (int s = 0; s < lenB; s++)
	{
		int v = B[s] - 'a';
		if (ch[inc][v])
		{
			tmp++;
			inc = ch[inc][v];
		}
		else
		{
			while (!ch[inc][v] && inc) inc = fa[inc];
			if (!inc) tmp = 0, inc = 1;
			else tmp = len[inc] + 1, inc = ch[inc][v];
		}
		ans = max(ans, tmp);
	}
	printf("%d\n", ans);
}
