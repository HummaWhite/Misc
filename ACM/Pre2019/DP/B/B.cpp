#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 505;

int n, m, f[L][L], R[L][L], B[L][L];

int main()
{
	scanf("%d%d", &n, &m);
	memset(R, 0, sizeof R);
	memset(B, 0, sizeof B);
	memset(f, 0, sizeof f);
	for (int s = 1; s <= n; s++)
		for (int t = 1; t <= m; t++)
		{
			scanf("%d", &R[s][t]);
			R[s][t] += R[s][t-1];
		}
	for (int s = 1; s <= n; s++)
		for (int t = 1; t <= m; t++)
		{
			scanf("%d", &B[s][t]);
			B[s][t] += B[s-1][t];
		}
	for (int s = 1; s <= n; s++)
		for (int t = 1; t <= m; t++)
			f[s][t] = max(f[s-1][t] + R[s][t], f[s][t-1] + B[s][t]);
	printf("%d\n", f[n][m]);
}
