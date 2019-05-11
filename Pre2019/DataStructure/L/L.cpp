#include<iostream>
#include<cstring>
#include<cstdio>
#define lson l, m, k<<1
#define rson m+1, r, k<<1|1
#define ls k<<1
#define rs k<<1|1
typedef long long LL;

using namespace std;

const int L = 100005;

LL n, m;

inline LL rd()
{
	LL ret = 0, f = 1; char c = getchar();
	while (c > '9' || c < '0')
	{
		if (c == '-') f = -1;
		c = getchar();
	}
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret * f;
}

struct tree
{
	LL lv, lm, rv, rm, mm, d, len;
	tree() {lv = rv = 0, lm = rm = 0, mm = 0, d = 0;}
	void set(LL x) {lv = rv = x, lm = rm = 1, mm = 1, d = 0;}
} a[L<<2];

tree merge(tree lx, tree rx)
{
	tree ret;
	ret.len = lx.len + rx.len;
	ret.lv = lx.lv, ret.lm = lx.lm;
	ret.rv = rx.rv, ret.rm = rx.rm;
	if (lx.lm == lx.len && lx.lv == rx.lv) ret.lm += rx.lm;
	if (rx.rm == rx.len && rx.rv == lx.rv) ret.rm += lx.rm;

	if (lx.mm > rx.mm) ret.mm = lx.mm;
	else ret.mm = rx.mm;
	if (lx.lm > ret.mm) ret.mm = lx.lm;
	if (rx.rm > ret.mm) ret.mm = rx.rm;
	if (lx.rv == rx.lv)
		if (lx.rm + rx.lm > ret.mm)
			ret.mm = lx.rm + rx.lm;
	if (ret.lv == ret.rv && ret.lm + ret.rm > ret.len)
		ret.lm = ret.rm = ret.len;
	return ret;
}

void pushdown(int k)
{
	if (a[k].d == 0) return;
	LL d = a[k].d;
	a[ls].lv += d, a[ls].rv += d;
	a[rs].lv += d, a[rs].rv += d;
	a[ls].d += d, a[rs].d += d;
	a[k].d = 0;
}

void build(int l, int r, int k)
{
	if (l == r)
	{
		a[k].len = r - l + 1;
		a[k].lm = a[k].rm = a[k].mm = 1;
		return;
	}
	int m = (l+r) >> 1;
	build(lson), build(rson);
	a[k] = merge(a[ls], a[rs]);
}

void add(int l, int r, int k, int p, LL c)
{
	if (l == r)
	{
		a[k].lv += c, a[k].rv += c;
		return;
	}
	int m = (l+r) >> 1;
	pushdown(k);
	if (p <= m) add(lson, p, c);
	else add(rson, p, c);
	a[k] = merge(a[lson], a[rson]);
}

void update(int l, int r, int k, int ll, int rr, LL c)
{
	if (ll <= l && rr >= r)
	{
		a[k].d += c;
		a[k].lv += c, a[k].rv += c;
		return;
	}
	pushdown(k);
	int m = (l+r) >> 1;
	if (ll <= m) update(lson, ll, rr, c);
	if (rr > m) update(rson, ll, rr, c);
	a[k] = merge(a[ls], a[rs]);
}

tree query(int l, int r, int k, int ll, int rr)
{
	if (ll <= l && rr >= r) return a[k];
	pushdown(k);
	int m = (l+r) >> 1;
	if (rr <= m) return query(lson, ll, rr);
	if (ll > m) return query(rson, ll, rr);
	pushdown(k);
	tree lq = query(lson, ll, rr);
	tree rq = query(rson, ll, rr);
	tree ret;
	ret = merge(lq, rq);
	return ret;
}

int main()
{
	n = rd(), m = rd();
	build(1, n, 1);
	while (m--)
	{
		LL opt, l, r, A, k, p;
		opt = rd(), l = rd(), r = rd();
		if (opt == 1)
		{
			if (l == r)
			{
				printf("1\n");
				continue;
			}
			tree res;
			res = query(1, n, 1, l, r);
			if (res.mm == res.lm)
				res = query(1, n, 1, l+1, r);
			printf("%d\n", res.mm+1);
		}
		else
		{
			A = rd(), k = rd(), p = rd();
			add(1, n, 1, l, A);
			if (r != n) add(1, n, 1, r+1, -A);
			update(1, n, 1, l+1, p, k);
			update(1, n, 1, p+1, r, -k);
			if (r != n) add(1, n, 1, r+1, (l+r - (p<<1))*k);
		}
	}
}
