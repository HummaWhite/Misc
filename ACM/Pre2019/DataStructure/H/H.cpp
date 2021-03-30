#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
typedef long long LL;

using namespace std;

const int L = 100005;
const LL INF = 0x6FFFFFFFFFFFLL;

LL sum[L], ans;
int n, m, top, tail, Q[L];

int main()
{
	scanf("%d%d", &n, &m);
	sum[0] = 0;
	for (int s = 1; s <= n; s++)
	{
		LL k; scanf("%lld", &k);
		sum[s] = sum[s-1] + k;
	}
	top = 1, tail = 1;
	ans = sum[1];
	for (int s = 1; s <= n; s++)
	{
		while (top <= tail && Q[top] + m < s) top++;
		while (top <= tail && sum[s] < sum[Q[tail]]) tail--;
		Q[++tail] = s;
		if (s == Q[top]) ans = max(ans, sum[s] - sum[s-1]);
		else ans = max(ans, sum[s] - sum[Q[top]]);
	}
	printf("%lld\n", ans);
}
