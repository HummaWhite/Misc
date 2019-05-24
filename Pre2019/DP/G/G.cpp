#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 20000007;

int n, k, inc = 0;
char a[L];

int main()
{
	scanf("%d%d", &n, &k);
	scanf("%s", a);
	LL f[2][k];
	for (int s = 0; s < n; s++) a[s] -= '0';
	memset(f, 0, sizeof f);
	LL ans = 0;
	for (int s = 0; s < n-1; s++)
	{
		f[inc][a[s]%k]++;
		ans += f[inc][0];
		for (int t = 0; t < k; t++) f[inc^1][t] = 0;
		for (int t = 0; t < k; t++)
			f[inc^1][(10*t + a[s+1]) % k] += f[inc][t];
		inc ^= 1;
	}
	f[inc][a[n-1]%k]++;
	ans += f[inc][0];
	printf("%lld\n", ans);
}
