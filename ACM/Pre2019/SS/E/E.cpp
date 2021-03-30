#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>

using namespace std;

const int L = 1000006;

char ch[L], tmp[L];

int n, tr[L][26], lst[L], f[L], nd[L], pre[L], tot = 1;
int cnt[L];

void insert(int id)
{
	int len = strlen(tmp), inc = 0;
	for (int s = 0; s < len; s++)
	{
		int u = tmp[s] - 'a';
		//printf("%d\n", inc);
		if (!tr[inc][u]) tr[inc][u] = tot++;
		inc = tr[inc][u];
	}
	if (!nd[inc]) nd[inc] = id;
	else pre[id] = nd[inc];
}

void getnext()
{
	queue<int> Q;
	f[0] = 0;
	for (int s = 0; s < 26; s++)
	{
		int u = tr[0][s];
		if (u) Q.push(u), lst[u] = 0, f[u] = 0;
	}
	while (!Q.empty())
	{
		int cur = Q.front(); Q.pop();
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
				lst[u] = nd[f[u]] ? f[u] : lst[f[u]];
				Q.push(u);
			}
		}
	}
}

void count(int x)
{
	while (x)
	{
		cnt[nd[x]]++;
		x = lst[x];
	}
}

void match()
{
	int len = strlen(ch), inc = 0;
	for (int s = 0; s < len; s++)
	{
		inc = tr[inc][ch[s]-'a'];
		if (nd[inc]) count(inc);
		else if (lst[inc]) count(lst[inc]);
	}
}

void init()
{
	memset(f, 0, sizeof f);
	memset(tr, 0, sizeof tr);
	memset(nd, 0, sizeof nd);
	memset(cnt, 0, sizeof cnt);
	for (int s = 0; s <= n; s++) pre[s] = s;
}

void dfs(int x)
{
	for (int s = 0; s < 26; s++)
		if (tr[x][s])
		{
			printf("#%d %c\n", tr[x][s], s + 'a');
			dfs(tr[x][s]);
		}
}

int main()
{
	scanf("%s", ch);
	scanf("%d", &n);
	init();
	for (int s = 0; s < n; s++)
	{
		scanf("%s", tmp);
		insert(s+1);
		//dfs(0);
	}
	getnext();
	match();
	for (int s = 1; s <= n; s++) printf("%d\n", cnt[pre[s]]);
}

