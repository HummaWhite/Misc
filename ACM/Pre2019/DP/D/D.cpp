#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 1000006;

int a[L], ind[L], tmp[L], tag[L], f[L][2], c[L];
int n, tot;

void uniq()
{
	tot = 1, tag[0] = tmp[0];
	int cur = tmp[0];
	for (int s = 1; s < n; s++)
		if (tmp[s] != cur)
			tag[tot++] = tmp[s], cur = tmp[s];
	for (int s = 0; s < n; s++)
	{
		int l = 0, r = tot-1;
		while (l != r)
		{
			int m = (l+r) >> 1;
			if (a[s] <= tag[m]) r = m;
			else l = m+1;
		}
		ind[s] = l+1;
	}
}

int lb(int k)
{
	return k & -k;
}

void update(int k, int num)
{
	for (int s = k; s <= n; s += lb(s)) c[s] = max(c[s], num);
}

int query(int k)
{
	int ret = 0;
	for (int s = k; s; s -= lb(s)) ret = max(ret, c[s]);
	return ret;
}

int main()
{
	scanf("%d", &n);
	for (int s = 0; s < n; s++)
	{
		scanf("%d", &a[s]);
		tmp[s] = a[s];
	}
	sort(tmp, tmp+n);
	uniq();
	/*for (int s = 0; s < n; s++) printf("%d ", ind[s]);
	printf("\n");*/
	memset(c, 0, sizeof c);
	for (int s = 0; s < n; s++)
	{
		f[s][0] = query(ind[s]-1) + 1;
		update(ind[s], f[s][0]);
	}
	memset(c, 0, sizeof c);
	for (int s = n-1; s >= 0; s--)
	{
		f[s][1] = query(ind[s]-1) + 1;
		update(ind[s], f[s][1]);
	}
	int ans = 0;
	for (int s = 0; s < n; s++)
		ans = max(ans, min(f[s][0], f[s][1]));
	printf("%d\n", ans*2 - 1);
}
