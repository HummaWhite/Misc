#include<iostream>
#include<cstring>
#include<cstdio>
#define pl printf("\n")
#define ps putchar(' ')
typedef long long LL;

using namespace std;

const int L = (1 << 16) + 1;

LL f[L], a[L], Q[L][2];
int n, top[2], tail[2], vis[L];
char tmp[20];

void pr(LL k)
{
	for (int s = 0; s < n; s++) printf("%lld", (k>>s) & 1);
}

int main()
{
	int T; scanf("%d", &T);
	for (int tt = 1; tt <= T; tt++)
	{
		scanf("%d", &n);
		LL ini = 0;
		memset(a, 0, sizeof a);
		for (int s = 0; s <= n; s++)
		{
			scanf("%s", tmp);
			LL k = 0;
			for (int t = 0; t < n; t++) k += ((tmp[t]-'0') & 1) << t;
			if (s == 0) ini = k;
			else a[s-1] = k;
		}
		int inc = 0;
		top[0] = top[1] = tail[0] = tail[1] = 0;
		memset(f, 0, sizeof f);
		memset(vis, 0, sizeof vis);
		for (int s = 0; s < n; s++)
			if ((1<<s) & ini)
			{
				Q[tail[inc]++][inc] = 1<<s;
				f[1<<s] = 1;
			}
		for (int s = 0; s < n-1; s++)
		{
			top[inc^1] = tail[inc^1] = 0;
			while (top[inc] != tail[inc])
			{
				LL k = Q[top[inc]++][inc];
				for (int t = 0; t < n; t++)
				{
					if ((1<<t) & k)
					{
						for (int i = 0; i < n; i++)
						{
							if ((((1<<i) & (a[t] | ini)) != 0) && (((1<<i) & k) == 0))
							{
								LL nx = (1<<i) | k;
								if (!vis[nx])
								{
									Q[tail[inc^1]++][inc^1] = nx;
									f[nx] += f[k];
									vis[nx] = k;
								}
								else
								{
									if (vis[nx] != k)
									{
										f[nx] += f[k];
										vis[nx] = k;
									}
								}
							}
						}
					}
				}
			}
			inc ^= 1;
		}
		printf("Case %d: %lld\n", tt, f[(1<<n)-1]);
	}
}
