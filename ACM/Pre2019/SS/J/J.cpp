#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>

using namespace std;

const int L = 1000006;

char ch[L], tmp[L];

int n, tr[L][26], lst[L], f[L], pre[L], nd[L], tot = 1;
int cnt[L], head[L], tt = 0;

struct edge
{
	int v, next;
} a[L<<1];

void addedge(int u, int v)
{
	a[tt].v = v, a[tt].next = head[u];
	head[u] = tt++;
}

void insert(int id)
{
	int len = strlen(tmp), inc = 0;
	for (int s = 0; s < len; s++)
	{
		int u = tmp[s] - 'a';
		//printf("%d\n", inc);
		if (!tr[inc][u]) tr[inc][u] = tot++;
		cnt[inc]++;
		inc = tr[inc][u];
	}
	cnt[inc]++;
	nd[id] = inc;
}

void getnext()
{
	queue<int> Q;
	f[0] = 0;
	for (int s = 0; s < 26; s++)
	{
		int u = tr[0][s];
		if (u) Q.push(u), f[u] = 0;
	}
	while (!Q.empty())
	{
		int cur = Q.front(); Q.pop();
		addedge(f[cur], cur);
		//printf("%d\n", cur);
		for (int s = 0; s < 26; s++)
		{
			int u = tr[cur][s];
			if (!u) tr[cur][s] = tr[f[cur]][s];
			else
			{
				int inc = f[cur];
				while (inc && !tr[inc][s]) inc = f[inc];
				f[u] = tr[inc][s];
				Q.push(u);
			}
		}
	}
}

void init()
{
	memset(f, 0, sizeof f);
	memset(tr, 0, sizeof tr);
	memset(nd, 0, sizeof nd);
	memset(cnt, 0, sizeof cnt);
	memset(head, -1, sizeof head);
}

void dfs(int x)
{
	for (int s = head[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		dfs(v);
		cnt[x] += cnt[v];
	}
}

int main()
{
	scanf("%d", &n);
	init();
	for (int s = 0; s < n; s++)
	{
		scanf("%s", tmp);
		insert(s+1);
	}
	getnext();
	dfs(0);
	for (int s = 1; s <= n; s++) printf("%d\n", cnt[nd[s]]);
}
