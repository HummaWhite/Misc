#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 1000006;

int pre[L], sum[L];
int n, m;

int find(int x)
{
	if (x != pre[x])
	{
		int fa = pre[x];
		pre[x] = find(pre[x]);
		sum[x] = (sum[x] + sum[fa] + 2) & 1;
	}
	return pre[x];
}

void init()
{
	for (int s = 0; s <= n; s++)
		pre[s] = s, sum[s] = 0;
}

int main()
{
	scanf("%d%d", &n, &m);
	int cnt = 0, flg = 0;
	int tmp = m;
	init();
	while (tmp--)
	{
		int l, r, k; scanf("%d%d", &l, &r);
		char d[5]; scanf("%s", d);
		k = d[0] == 'o' ? 1 : 0;
		int x = find(l-1), y = find(r);
		if (x == y)
		{
			if (((sum[l-1] + k + 2) & 1) != ((sum[r] + 2) & 1)) flg = 1;
		}
		if (x != y)
		{
			pre[y] = x;
			sum[y] = (sum[l-1] - sum[r] + k + 2) & 1;
		}
		if (!flg) cnt++;
	}
	if (cnt < m) printf("%d\n", cnt);
	else printf("ORZQHQH\n");
}
