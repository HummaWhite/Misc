#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 1000006;
const LL INF = 0x6FFFFFFFFFFFFFFLL;
const int eom = 0, moe = 1;

struct node
{
	LL eom, moe;
};

int fa[L], val[L], head[L];
int n, tot = 0;
char c[4];

struct edge
{
	int v, next;
} a[L];

void addedge(int u, int v)
{
	a[tot].v = v, a[tot].next = head[u];
	head[u] = tot++;
}

node dfs(int x, int cur)
{
	node ret;
	if (head[x] == -1)
	{
		if (cur == eom) ret.eom = val[x], ret.moe = 0;
		else ret.eom = 0, ret.moe = val[x];
		return ret;
	}
	if (cur == eom) ret.eom = INF, ret.moe = -INF;
	else ret.eom = -INF, ret.moe = INF;
	for (int s = head[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		node tmp;
		tmp = dfs(v, cur^1);
		if (cur == eom)
		{
			if (ret.eom == tmp.eom && ret.moe < tmp.moe) ret = tmp;
			if (ret.eom > tmp.eom) ret = tmp;
		}
		else
		{
			if (ret.eom == tmp.eom && ret.moe > tmp.moe) ret = tmp;
			if (ret.eom < tmp.eom) ret = tmp;
		}
	}
	if (cur == eom) ret.eom += val[x];
	else ret.moe += val[x];
	return ret;
}

int main()
{
	scanf("%s", c);
	scanf("%d", &n);
	for (int s = 1; s <= n; s++) scanf("%d", &val[s]);
	memset(head, -1, sizeof head);
	memset(fa, -1, sizeof fa);
	for (int s = 1; s < n; s++)
	{
		int u, v; scanf("%d%d", &u, &v);
		addedge(u, v);
		fa[v] = u;
	}
	int rt = n;
	while (~fa[rt]) rt = fa[rt];
	node ans = dfs(rt, c[0] == 'e' ? eom : moe);
	printf("%lld %lld\n", ans.eom, ans.moe);
}
