#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 31;

LL n, a[L], ans[L], tot = 0;

void pr(LL k)
{
	for (int s = n-1; s >= 0; s--) printf("%lld", (k>>s) & 1);
	printf("\n");
}

int cnt = 0, mp[1000006];

LL lb(LL k) { return k & -k; }

int dfs(int dep, int inc, LL st)
{
	cnt++;
	//printf("%d %d ", dep, inc), pr(st);
	if (inc == dep+1) return st == (1LL << n) - 1;
	for (int s = 0; s <= inc-1; s++)
	{
		for (int t = 0; t < n; t++)
		{
			if (st & (1LL << t)) continue;
			LL cur = ans[s] + a[t];
			if (cur <= ans[inc-1] || cur > a[n-1]) continue;
			ans[inc] = cur;
			//printf("# %d\n", cur); for (int k = 0; k <= inc; k++) printf("%lld ", ans[k]); printf("$\n");
			LL nex = st;
			for (int k = 0; k < inc; k++)
			{
				if (cur - ans[k] > a[n-1]) continue;
				if (cur - ans[k] < a[0]) break;
				int p = mp[cur - ans[k]];
				if (p) nex |= 1LL << (p-1);
			}
			if (lb(nex) != 1) continue; //ÃÛÖ­¼ôÖ¦
			if (dfs(dep, inc+1, nex)) return 1;
		}
	}
	return 0;
}

int main()
{
	scanf("%lld", &n);
	for (int s = 0; s < n; s++) scanf("%lld", &a[s]);
	sort(a, a+n);
	n = unique(a, a+n) - a;
	memset(mp, 0, sizeof mp);
	for (int s = 0; s < n; s++) mp[a[s]] = s+1;
	memset(ans, 0, sizeof ans);
	for (int s = 0; s <= n*n; s++)
	{
		ans[0] = 0;
		if (dfs(s, 1, 0))
		{
			printf("%d\n", s+1);
			for (int t = 0; t <= s; t++) printf("%lld ", ans[t]);
			printf("\n");
			break;
		}
		//printf("%d\n", cnt);
	}
}
