# O

QAQ题解选手

起先做这题的时候对数据的印象还停留在1e16，于是拿个map去记录离散化后的数值对应标号，结果可想而知……

去了map后还是T，只能考虑猛男剪枝

```C++
LL nex = st;
			for (int k = 0; k < inc; k++)
			{
				if (cur - ans[k] > a[n-1]) continue;
				if (cur - ans[k] < a[0]) break;
				int p = mp[cur - ans[k]];
				if (p) nex |= 1LL << (p-1);
			}
			if (lb(nex) != 1) continue; //蜜汁剪枝
			if (dfs(dep, inc+1, nex)) return 1;
```

lb为lowbit。剪了这些状态数变成四分之一，时间减半，就过了。（我觉得还可以剪