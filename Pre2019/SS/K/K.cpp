#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 2000006;

int ch[L][26], len[L], fa[L], cnt[L], np, lst = 1, tot = 1;
int r[L], head[L], tt = 0;

char A[L];

struct edge
{
	int v, next;
} a[L<<1];

void addedge(int u, int v)
{
	a[tt].v = v, a[tt].next = head[u];
	head[u] = tt++;
}

void add(int c)
{
	int p, q, np, nq;
	p = lst, lst = np = ++tot;
	r[np] = 1;
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

void dfsT(int x)
{
	//printf("%d\n", x);
	for (int s = head[x]; ~s; s = a[s].next)
	{
		dfsT(a[s].v);
		r[x] += r[a[s].v];
	}
}

void init()
{
	memset(ch, 0, sizeof ch);
	memset(head, -1, sizeof head);
	memset(r, 0, sizeof r);
}

int main()
{
	init();
	scanf("%s", A);
	int lenA = strlen(A);
	for (int s = 0; s < lenA; s++) add(A[s] - 'a');
	for (int s = 2; s <= tot; s++) addedge(fa[s], s);
	dfsT(1);
	for (int s = 1; s <= tot; s++) cnt[len[s]] = max(cnt[len[s]], r[s]);
	for (int s = lenA; s > 1; s--) cnt[s-1] = max(cnt[s-1], cnt[s]);
	for (int s = 1; s <= lenA; s++) printf("%d ", cnt[s]);
}
