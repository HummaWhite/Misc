#include<iostream>
#include<cstring>
#include<cstdio>
#define lson l, m, k<<1
#define rson m+1, r, k<<1|1
typedef long long LL;

using namespace std;

const int L = 4000006;
const LL INF = 0x6FFFFFFFFFFFFFFFLL;

int n, q;

struct tree
{
	LL sum, mmin, mmax, d;
	tree() {sum = mmin = mmax = d = 0;}
} a[L];

inline int rd()
{
	int ret = 0, f = 1; char c = getchar();
	while (c > '9' || c < '0')
	{
		if (c == '-') f = -1;
		c = getchar();
	}
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret * f;
}

void pushup(int k)
{
	a[k].sum = a[k<<1].sum + a[k<<1|1].sum;
	a[k].mmin = min(a[k<<1].mmin, a[k<<1|1].mmin);
	a[k].mmax = max(a[k<<1].mmax, a[k<<1|1].mmax);
}

void pushdown(int k, LL len)
{
	if (!a[k].d) return;
	a[k<<1].sum += a[k].d * (len - (len>>1));
	a[k<<1|1].sum += a[k].d * (len>>1);
	a[k<<1].mmin += a[k].d, a[k<<1|1].mmin += a[k].d;
	a[k<<1].mmax += a[k].d, a[k<<1|1].mmax += a[k].d;
	a[k<<1].d += a[k].d, a[k<<1|1].d += a[k].d;
	a[k].d = 0;
}

void build(int l, int r, int k)
{
	if (l == r)
	{
		a[k].sum = a[k].mmin = a[k].mmax = a[k].d = 0;
		return;
	}
	int m = (l+r) >> 1;
	build(lson), build(rson);
	pushup(k);
}

void update(int l, int r, int k, int ll, int rr, int num)
{
	LL len = r - l + 1;
	if (ll <= l && rr >= r)
	{
		a[k].sum += num * len;
		a[k].mmin += num, a[k].mmax += num;
		a[k].d += num;
		return;
	}
	pushdown(k, len);
	int m = (l+r) >> 1;
	if (ll <= m) update(lson, ll, rr, num);
	if (rr > m) update(rson, ll, rr, num);
	pushup(k);
}

tree query(int l, int r, int k, int ll, int rr)
{
	LL len = r - l + 1;
	if (ll <= l && rr >= r) return a[k];
	pushdown(k, len);
	int m = (l+r) >> 1;
	if (rr <= m) return query(lson, ll, rr);
	else if (ll > m) return query(rson, ll, rr);
	tree ls = query(lson, ll, rr);
	tree rs = query(rson, ll, rr);
	tree tmp;
	tmp.sum = ls.sum + rs.sum;
	tmp.mmin = min(ls.mmin, rs.mmin);
	tmp.mmax = max(ls.mmax, rs.mmax);
	return tmp;
}

int main()
{
	n = rd(), q = rd();
	build(1, n, 1);
	while (q--)
	{
		int opt, l, r, num;
		opt = rd(), l = rd(), r = rd();
		if (opt == 1)
		{
			num = rd();
			update(1, n, 1, l, r, num);
		}
		else
		{
			tree tmp = query(1, n, 1, l, r);
			if (opt == 2) printf("%lld\n", tmp.sum);
			else printf("%lld\n", tmp.mmax - tmp.mmin);
		}
	}
}
