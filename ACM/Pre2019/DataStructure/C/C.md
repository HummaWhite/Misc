# C

线段树。

$n$只有$1e5$，那么区间总数也只有这么多，先把所有区间的$l$和$r$离散化。

需要维护的信息为：区间左右端连通性$lm$、$rm$、区间内连通块的个数$cnt$以及表示往下推连通性的延迟标记$d$。需要注意的是当两个区间相邻且不联通的时候，下推标记时不能推到区间的两个端点，需要特判。

```C++
void pushup(int k)
{
	a[k].cnt = a[ls].cnt + a[rs].cnt;
	a[k].lm |= a[ls].lm, a[k].rm |= a[rs].rm;
	if (a[ls].rm == 1 && a[rs].lm == 1) a[k].cnt--; //左右子区间中间可以拼成一段，答案减一
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
		if (r != rr) a[k].rm = 1; //被修改区间的左右端点连通性不能改
		a[k].d = 1;
		return;
	}
	int m = (l+r) >> 1;
	pushdown(k);
	if (ll <= m) update(lson, ll, rr);
	if (rr > m) update(rson, ll, rr);
	pushup(k);
}
```

