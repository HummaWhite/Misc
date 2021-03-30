#include<iostream>
#include<cstring>
#include<cstdio>
#define lson l, m, k<<1
#define rson m+1, r, k<<1|1
typedef long long LL;

using namespace std;

const int L = 400005;
const LL mod = 1000000007;

LL n, q;

struct tree
{
	LL sm, sq, mul, add;
	tree() {sm = sq = add = 0, mul = 1;}
} a[L];

inline LL rd()
{
	LL ret = 0; char c = getchar();
	while (c > '9' || c < '0') c = getchar();
	while (c >= '0' && c <= '9')
		ret = ret*10 + c - '0', c = getchar();
	return ret;
}

void pushup(int k)
{
	a[k].sm = (a[k<<1].sm + a[k<<1|1].sm) % mod;
	a[k].sq = (a[k<<1].sq + a[k<<1|1].sq) % mod;
}

void pushdown(int k, LL len)
{
	LL mul = a[k].mul, add = a[k].add;
	if (mul != 1)
	{
		a[k<<1].mul = (a[k<<1].mul * mul) % mod;
		a[k<<1|1].mul = (a[k<<1|1].mul * mul) % mod;
		a[k<<1].add = (a[k<<1].add * mul) % mod;
		a[k<<1|1].add = (a[k<<1|1].add * mul) % mod;
		a[k<<1].sq = (a[k<<1].sq * ((mul*mul) % mod)) % mod;
		a[k<<1|1].sq = (a[k<<1|1].sq * ((mul*mul) % mod)) % mod;
		a[k<<1].sm = (a[k<<1].sm * mul) % mod;
		a[k<<1|1].sm = (a[k<<1|1].sm * mul) % mod;
		a[k].mul = 1;
	}
	if (add != 0)
	{
		a[k<<1].sq = (a[k<<1].sq + (2 * (add * a[k<<1].sm % mod) % mod) + ((add * add % mod) * (len-(len>>1)) % mod)) % mod;
		a[k<<1|1].sq = (a[k<<1|1].sq + (2 * (add * a[k<<1|1].sm % mod) % mod) + ((add * add % mod) * (len>>1) % mod)) % mod;
		a[k<<1].sm = (a[k<<1].sm + (add * (len-(len>>1)) % mod)) % mod;
		a[k<<1|1].sm = (a[k<<1|1].sm + (add * (len>>1)) % mod) % mod;
		a[k<<1].add = (a[k<<1].add + add) % mod;
		a[k<<1|1].add = (a[k<<1|1].add + add) % mod;
		a[k].add = 0;
	}
}

void build(int l, int r, int k)
{
	if (l == r)
	{
		a[k].sm = rd();
		a[k].sq = (a[k].sm * a[k].sm) % mod;
		a[k].mul = 1, a[k].add = 0;
		return;
	}
	int m = (l+r) >> 1;
	build(lson), build(rson);
	pushup(k);
}

void update(int l, int r, int k, int ll, int rr, LL num, int tp)
{
	LL len = r - l + 1;
	if (ll <= l && rr >= r)
	{
		if (tp == 1)
		{
			a[k].add = (a[k].add + num) % mod;
			a[k].sq = (a[k].sq + ((2 * (num*a[k].sm % mod)) % mod) + (((num*len) % mod) * num) % mod) % mod;
			a[k].sm = (a[k].sm + (num * len % mod)) % mod;
			return;
		}
		else
		{
			a[k].mul = (a[k].mul * num) % mod;
			a[k].add = (a[k].add * num) % mod;
			a[k].sm = (a[k].sm * num) % mod;
			a[k].sq = (a[k].sq * ((num * num) % mod)) % mod;
			return;
		}
	}
	pushdown(k, len);
	int m = (l+r) >> 1;
	if (ll <= m) update(lson, ll, rr, num, tp);
	if (rr > m) update(rson, ll, rr, num, tp);
	pushup(k);
}

tree query(int l, int r, int k, int ll, int rr)
{
	if (ll <= l && rr >= r) return a[k];
	pushdown(k, r-l+1);
	int m = (l+r) >> 1;
	if (rr <= m) return query(lson, ll, rr);
	else if (ll > m) return query(rson, ll, rr);
	tree ls = query(lson, ll, rr);
	tree rs = query(rson, ll, rr);
	tree tmp;
	tmp.sm = (ls.sm + rs.sm) % mod;
	tmp.sq = (ls.sq + rs.sq) % mod;
	pushup(k);
	return tmp;
}

int main()
{
	n = rd(), q = rd();
	build(1, n, 1);
	while (q--)
	{
		LL opt, l, r, k;
		opt = rd(), l = rd(), r = rd();
		if (opt == 4)
		{
			tree res = query(1, n, 1, l, r);
			res.sq %= mod, res.sm %= mod;
			LL ans = (((res.sq * (r-l+1)) % mod) - ((res.sm * res.sm) % mod) + mod * 5) % mod;
			printf("%lld\n", ans);
			continue;
		}
		k = rd();
		if (opt == 1) update(1, n, 1, l, r, k, 1);
		if (opt == 2) update(1, n, 1, l, r, k, 2);
		if (opt == 3) update(1, n, 1, l, r, 0, 2), update(1, n, 1, l, r, k, 1);
	}
}
