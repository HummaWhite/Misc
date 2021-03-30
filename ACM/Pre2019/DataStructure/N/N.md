# N

线段树区间更新模板题。查询时可以像下面这样返回节点的一个结构体，代替分写的三个查询函数，省了不少代码。

```C++
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
```

