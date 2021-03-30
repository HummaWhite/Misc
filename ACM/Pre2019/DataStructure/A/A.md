# A

线段树。

由方差的公式$D(X) = E(X^2) - E^2(X)$，$[l, r]$区间的方差为$\sum_{i=l}^{r} \frac{x_i^2}{r-l+1} - (\sum_{i=l}^r \frac{x_i}{r-l+1})^2$，乘上区间长度的平方后就是$(r-l+1)\sum_{i=l}^r x_i^2 - (\sum_{i=l}^r x_i)^2$

因此只需要维护区间和与区间平方和。

把区间内的数变成k可以看成先全部乘0再加上k，所以只需要加法和乘法两种操作。注意tag的更新顺序有影响，所以操作时乘法加法统一顺序，乘法操作时也要把加法的标记乘了。

二合一的修改

```C++
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
```

