# B

> 货车运输（确信），年轻时做这道题居然暴力LCA水过了

题意即求树上两点间的路径上边权的最小值。把路径拆成以两点最近公共祖先区分的两段，就转化为链上最小值的问题。于是就可以链剖或者倍增LCA来做了。

至于什么是链剖和倍增LCA就不多说了。

LCA的话，预处理祖先数组的时候可以用相同的思路处理某点到其第 $2^k$ 个父亲的最小边权，然后最小值可以在LCA函数提升两个点的时候计算出来。也可以先求LCA再分别算两条路的最小值。

$$lm[s][t] = min ( lm[s][t-1],lm[fa[s-1][t-1]][t-1]) ) $$

$$fa[s][t] = fa[fa[s-1][t-1]][t-1]$$

复杂度 $$n\log n + q\log n$$

链剖的话，处理出来的链用线段树维护。没写。
