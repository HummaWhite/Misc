# L

由于最后询问的是最长等差数列的长度，并且每次的操作都是加等差数列上去，我们差分一下，就转化成了询问区间最长连续相同的值。修改就变成了区间加和单点加（因为还有一个$a$，需要在差分的区间头尾打单点标记）。

于是可以用线段树维护区间左右端点的值$lv$、$rv$和这个值从左右端点向中间延伸的最长长度$lm$、$rm$、区间内最长连续长度$mm$，以及延迟标记。具体的pushup、pushdown是这样的

```C++
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
}  //pushup，返回结构体是方便在建树、修改和查询共用这一个函数

void pushdown(int k)
{
	if (a[k].d == 0) return;
	LL d = a[k].d;
	a[ls].lv += d, a[ls].rv += d;
	a[rs].lv += d, a[rs].rv += d;
	a[ls].d += d, a[rs].d += d;
	a[k].d = 0;
}  //对区间加上一个数不影响区间内最大连续长度，但要更新一下端点的值
```



查询的时候要注意因为是差分数组，求得的长度+1不一定是最后的答案：如果mm的真实区间紧贴查询区间的左端，那么答案就是当前查询的长度。这就需要比较查询所得区间内的最长连续长度是不是从左起的最长连续长度转移过来的。然后发现如果两个相等的话只用再查询一次$[l+1, r]$，输出结果+1即可。

>之前考虑过差分数组的问题，想到区间左端的影响，但思路一直转不过来，卡在如何比较mm的真实区间左端是否为1的情况，WA十多发……多亏了cxmm的指点