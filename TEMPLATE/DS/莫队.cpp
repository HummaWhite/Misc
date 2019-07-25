#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>

using namespace std;

const int L = 200005;

int n, m, a[L], ind[L], tag[L], tmp[L], tot;
int bsize, bnum;
int cnt[L], apd[L], ans[L];

struct q
{
	int l, r, id, bl;
	bool operator < (const q x) const
	{
		if (bl == x.bl) return r < x.r;
		return bl < x.bl;
	}
} q[L];

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
	tot = 1;
	int inc = 1, cur = 0;
	while (inc <= n)
	{
		if (tmp[inc] != cur) tag[tot++] = tmp[inc], cur = tmp[inc];
		inc++;
	}
	for (int s = 1; s <= n; s++)
	{
		int l = 1, r = tot;
		while (l != r)
		{
			int m = (l+r) >> 1;
			if (a[s] <= tag[m]) r = m;
			else l = m+1;
		}
		ind[s] = l;
	}
}

void init()
{
	bsize = sqrt(n);
	bnum = n / bsize;
	if (bsize * bnum < n) bnum++;
}

int sum;

void add(int id)
{
	if (cnt[id] == sum) sum++;
	apd[cnt[id]]--, cnt[id]++, apd[cnt[id]]++;
}

void del(int id)
{
	if (cnt[id] == sum && apd[cnt[id]] == 1) sum--;
	apd[cnt[id]]--;
	if (cnt[id] > 0) cnt[id]--;
	apd[cnt[id]]++;
}

int main()
{
	n = rd(), m = rd();
	for (int s = 1; s <= n; s++) a[s] = tmp[s] = rd();
	sort(tmp+1, tmp+n+1);
	uniq();
	init();
	for (int s = 0; s < m; s++)
		q[s].l = rd(), q[s].r = rd(), q[s].id = s, q[s].bl = q[s].l / bsize;
	sort(q, q + m);
	memset(cnt, 0, sizeof cnt);
	memset(apd, 0, sizeof apd);
	apd[0] = tot;
	int l = 1, r = 0; sum = 0;
	for (int s = 0; s < m; s++)
	{
		//printf("%d %d\n", l, r);
		while (r < q[s].r) r++, add(ind[r]);
		while (r > q[s].r) del(ind[r]), r--;
		while (l < q[s].l) del(ind[l]), l++;
		while (l > q[s].l) l--, add(ind[l]);
		ans[q[s].id] = sum;
	}
	for (int s = 0; s < m; s++) printf("%d\n", ans[s]);
}
