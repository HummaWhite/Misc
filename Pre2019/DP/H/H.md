# H

树形dp，感觉其实照着题意dfs一遍就好了。（甚至dp数组都可以省掉）

想了一下，选下来的路径一定是树的一条链，而是谁选只跟当前点深度有关。可以用dp[i]表示以i为结点的子树中两者的最优决策……然后树形dp标准操作

```C++
node dfs(int x, int cur)
{
	node ret;
	if (head[x] == -1)
	{
		if (cur == eom) ret.eom = val[x], ret.moe = 0;
		else ret.eom = 0, ret.moe = val[x];
		return ret;
	}
	if (cur == eom) ret.eom = INF, ret.moe = -INF;
	else ret.eom = -INF, ret.moe = INF;
	for (int s = head[x]; ~s; s = a[s].next)
	{
		int v = a[s].v;
		node tmp;
		tmp = dfs(v, cur^1);
		if (cur == eom)
		{
			if (ret.eom == tmp.eom && ret.moe < tmp.moe) ret = tmp;
			if (ret.eom > tmp.eom) ret = tmp;
		}
		else
		{
			if (ret.eom == tmp.eom && ret.moe > tmp.moe) ret = tmp;
			if (ret.eom < tmp.eom) ret = tmp;
		}
	}
	if (cur == eom) ret.eom += val[x];
	else ret.moe += val[x];
	return ret;
}
```

