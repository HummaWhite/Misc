#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 100005;

LL a[L], b[L], c[L], n, q;

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

LL lb(int k)
{
	return k & -k;
}

LL add(int k, int num)
{
	while (k <= n)
	{
		c[k] += num;
		k += lb(k);
	}
}

LL query(int k)
{
	LL ret = 0;
	while (k)
	{
		ret += c[k];
		k -= lb(k);
	}
	return ret;
}

int main()
{
	n = rd();
	for (int s = 1; s <= n; s++) a[s] = rd(), add(s, a[s]);
	for (int s = 1; s < n; s++) b[s] = rd();
	q = rd();
	while (q--)
	{
		LL opt, l, r;
		opt = rd(), l = rd(), r = rd();
		if (opt == 1)
		{
			add(l, r), a[l] += r;
			int inc = l;
			while (inc < n)
			{
				LL d = a[inc] + b[inc] - a[inc+1];
				if (d > 0)
				{
					a[inc+1] += d;
					add(inc+1, d);
					inc++;
				}
				else break;
			}
		}
		else
		{
			LL ans = query(r) - query(l-1);
			printf("%lld\n", ans);
		}
	}
}
