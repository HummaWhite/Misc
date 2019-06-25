#include<iostream>
#include<cstring>
#include<cstdio>
#include<map>
typedef long long LL;

using namespace std;

const int L = 30;
const int N = 600000;

LL a[L], b[L];
LL n, k, S, m, ans = 0;

map<LL, int> A[N];

int cc = 0;

void dfs1(int inc, int cur, LL sum)
{
	if (inc == m)
	{
		A[cur][sum]++;
		return;
	}
	dfs1(inc+1, cur, sum);
	if (sum + a[inc] <= S) dfs1(inc+1, cur, sum+a[inc]);
	if (cur < k && sum + b[inc] <= S) dfs1(inc+1, cur+1, sum+b[inc]);
}

void dfs2(int inc, int cur, int sum)
{
	if (inc == n)
	{
		for (int s = 0; s + cur <= k; s++)
			if (A[s].count(S-sum))
				ans += A[s][S-sum];
		return;
	}
	dfs2(inc+1, cur, sum);
	if (sum + a[inc] <= S) dfs2(inc+1, cur, sum+a[inc]);
	if (cur < k && sum + b[inc] <= S) dfs2(inc+1, cur+1, sum+b[inc]);
}

int main()
{
	scanf("%lld%lld%lld", &n, &k, &S);
	m = n >> 1;
	for (int s = 0; s < n; s++)
	{
		scanf("%lld", &a[s]);
		b[s] = (a[s] * (a[s]+1)) / 2;
	}
	dfs1(0, 0, 0);
	dfs2(m, 0, 0);
	printf("%lld\n", ans);
}
