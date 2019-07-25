#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>

using namespace std;

const int L = 40004;
const int B = 205;

int n, m, a[L], ind[L], tag[L], tmp[L], tot = 0;
int sum[B][L], st[B], nd[B], f[B][B];
int bsize, bnum;

inline int rd()
{
	int ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

void uniq()
{
	int inc = 0, cur = 0;
	while (inc < n)
	{
		if (tmp[inc] != cur) tag[tot++] = tmp[inc], cur = tmp[inc];
		inc++;
	}
	for (int s = 0; s < n; s++)
	{
		int l = 0, r = tot;
		while (l != r)
		{
			int m = (l+r) >> 1;
			if (a[s] <= tag[m]) r = m;
			else l = m+1;
		}
		ind[s] = l;
	}
}

int query(int l, int r)
{
	int ans = 0, cnt[tot];
	memset(cnt, 0, sizeof cnt);
	int ll = l / bsize, rr = r / bsize;
	int mass = 0, mcnt = 0;
	if (rr - ll < 2)
	{
		for (int s = l; s <= r; s++)
		{
			cnt[ind[s]]++;
			mcnt = max(mcnt, cnt[ind[s]]);
		}
		return mcnt;
	}
	mass = f[ll+1][rr-1], mcnt = sum[rr][mass] - sum[ll+1][mass];
	for (int s = l; s <= nd[ll]; s++)
	{
		int id = ind[s];
		cnt[id]++;
		if (cnt[id] + sum[rr][id] - sum[ll+1][id] > mcnt)
		{
			mcnt = cnt[id] + sum[rr][id] - sum[ll+1][id];
			mass = id;
		}
	}
	for (int s = st[rr]; s <= r; s++)
	{
		int id = ind[s];
		cnt[id]++;
		if (cnt[id] + sum[rr][id] - sum[ll+1][id] > mcnt)
		{
			mcnt = cnt[id] + sum[rr][id] - sum[ll+1][id];
			mass = id;
		}
	}
	return mcnt;
}

void init()
{
	bsize = sqrt(n);
	bnum = n / bsize;
	for (int s = 0; s < bnum; s++)
	{
		st[s] = s * bsize, nd[s] = s * bsize + bsize - 1;
	}
	if (bsize * bnum < n)
	{
		st[bnum] = bsize * bnum, nd[bnum] = n-1;
		bnum++;
	}
}

int main()
{
	n = rd(), m = rd();
	for (int s = 0; s < n; s++) a[s] = tmp[s] = rd();
	sort(tmp, tmp+n);
	uniq();
	init();
	memset(sum, 0, sizeof sum);
	for (int s = 0; s < n; s++)
	{
		int ps = s / bsize, num = ind[s];
		sum[ps+1][num]++;
	}
	for (int s = 1; s <= bnum; s++)
		for (int t = 0; t < tot; t++)
			sum[s][t] += sum[s-1][t];
	for (int s = 0; s < bnum; s++)
	{
		int mass = 0, cnt[tot];
		memset(cnt, 0, sizeof cnt);
		for (int t = s * bsize; t < n; t++)
		{
			int id = ind[t];
			cnt[id]++;
			if (cnt[mass] < cnt[id]) mass = id;
			if (t / bsize != (t+1) / bsize) f[s][t / bsize] = mass;
		}
	}
	int x = 0;
	while (m--)
	{
		int l, r;
		l = rd(), r = rd();
		l = (l+x-1) % n + 1, r = (r+x-1) % n + 1;
		if (l > r) swap(l, r);
        x = query(l-1, r-1);
		printf("%d\n", x);
	}
}
