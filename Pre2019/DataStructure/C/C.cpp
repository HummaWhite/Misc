#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
#define lson l, m, k<<1
#define rson m+1, r, k<<1|1
#define ls k<<1
#define rs k<<1|1

using namespace std;

const int L = 200005;

int n, seg[L][2], tmp[L<<1], tag[L<<1], tt = 0, tot = 0;

inline int rd()
{
	int ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret * 10 + c - '0', c = getchar();
	return ret;
}

struct tree
{
	int cnt, lm, rm, d;
	tree() {cnt = lm = rm = d = 0;}
} a[L<<2];

void pushup(int k)
{
	a[k].cnt = a[ls].cnt + a[rs].cnt;
	a[k].lm |= a[ls].lm, a[k].rm |= a[rs].rm;
	if (a[ls].rm == 1 && a[rs].lm == 1) a[k].cnt--;
}

void pushdown(int k)
{
	if (!a[k].d) return;
	a[ls].rm = a[rs].lm = 1;
	a[ls].cnt = a[rs].cnt = 1;
	a[ls].d = a[rs].d = 1;
	a[k].d = 0;
}

void update(int l, int r, int k, int ll, int rr)
{
	if (ll <= l && rr >= r)
	{
		a[k].cnt = 1;
		if (l != ll) a[k].lm = 1;
		if (r != rr) a[k].rm = 1;
		a[k].d = 1;
		return;
	}
	int m = (l+r) >> 1;
	pushdown(k);
	if (ll <= m) update(lson, ll, rr);
	if (rr > m) update(rson, ll, rr);
	pushup(k);
}

int query(int l, int r, int k, int ll, int rr)
{
	int ret = 0;
	if (ll <= l && rr >= r) return a[k].cnt;
	int m = (l+r) >> 1;
	pushdown(k);
	if (ll <= m) ret += query(lson, ll, rr);
	if (rr > m) ret += query(rson, ll, rr);
	return ret;
}

void uniq()
{
	int inc = 0, cur = 0;
	while (inc < tt)
	{
		if (tmp[inc] != cur) tag[tot++] = tmp[inc], cur = tmp[inc];
		inc++;
	}
}

int find(int k)
{
	int l = 0, r = tot;
	while (l != r)
	{
		int m = (l+r) >> 1;
		if (k < tag[m]) r = m;
		else l = m+1;
	}
	return l;
}

int main()
{
	n = rd();
	for (int s = 0; s < n; s++)
	{
		seg[s][0] = rd(), seg[s][1] = rd();
		tmp[tt++] = seg[s][0], tmp[tt++] = seg[s][1];
	}
	sort(tmp, tmp + tt);
	uniq();
	for (int s = 0; s < n; s++)
	{
		int l = find(seg[s][0]), r = find(seg[s][1]);
		//printf("%d %d\n", l, r);
		update(1, tot+1, 1, l, r);
		printf("%d ", query(1, tot+1, 1, 1, tot+1));
	}
}
