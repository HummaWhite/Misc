#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>

using namespace std;

const int L = 2000006;

struct edge
{
	int u, v, next, vis;
} a[L<<1];

struct p
{
	int u, v;
	bool operator < (const p x) const
	{
		if (u == x.u) return v > x.v;
		else return u > x.u;
	}
} tmp[L];

int head[L], tot = 0, n, m;
int in[L], out[L], ans[L], st = L+1, cnt = 0;

void addedge(int u, int v)
{
	in[v]++, out[u]++;
	a[tot].u = u, a[tot].v = v, a[tot].vis = 0;
	a[tot].next = head[u], head[u] = tot++;
}

inline int rd()
{
	int ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

int used = 0, stk[L<<3], pre[L], top = 0;

void dfs(int x)
{
	stk[++top] = x;
	//printf("# %d\n", x);
	for (int &s = head[x]; ~s; s = a[s].next)
	{
		if (!a[s].vis)
		{
			a[s].vis = 1;
			used++;
			dfs(a[s].v);
			break;
		}
	}
}

void fleury(int x)
{
	stk[++top] = x;
	while (top > 0)
	{
		int flg = 0;
		int u = stk[top];
		//printf("%d\n", u);
		for (int &s = head[u]; ~s; s = a[s].next)
		{
			if (!a[s].vis)
			{
				flg = 1;
				break;
			}
		}
		if (!flg) ans[cnt++] = stk[top--];
		else dfs(stk[top--]);
	}
}

int main()
{
	n = rd(), m = rd();
	for (int s = 0; s < m; s++) tmp[s].u = rd(), tmp[s].v = rd();
	sort(tmp, tmp+m);
	memset(in, 0, sizeof in);
	memset(out, 0, sizeof out);
	memset(head, -1, sizeof head);
	for (int s = 0; s < m; s++) addedge(tmp[s].u, tmp[s].v);
	int cnt0 = 0, cnt1 = 0;
	for (int s = 1; s <= n; s++)
	{
		cnt0 += in[s] == out[s];
		cnt1 += abs(in[s] - out[s]) == 1;
	}
	if ( (cnt1 != 0 && (cnt1 != 2 || cnt0 != n-2) ) || (cnt1 == 0 && cnt0 != n) )
	{
		printf("What a shame!\n");
		return 0;
	}
	if (cnt1 != 0)
	{
		for (int s = 1; s <= n; s++)
			if (out[s] - in[s] == 1)
				st = min(st, s);
	}
	else
	{
		for (int s = 0; s < m; s++)
			st = min(st, min(a[s].u, a[s].v));
	}
	fleury(st);
	if (used != m)
	{
		printf("What a shame!\n");
		return 0;
	}
	for (int s = cnt-1; s >= 0; s--) printf("%d ", ans[s]);
}
