# D

选出长度为k的字典序最大的子串。

开个$16n$的数组，读入时把当前数字的下标压进对应数字的数组里，数组里的下标一定是升序的。

然后可以贪心了。设选了$s$个数字、上次找到的下标为$linc$。每次从最大的数字开始，二分找每个数组里可行的下标$cinc$。$cinc$满足$cinc > linc$且$cinc + k - s < m$。

复杂度$k\log{n}$？

```C++
#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 100005;

int n, k, a[16][L], tot[18], mm[18];

int ind(char x)
{
	if (x >= 'a' && x <= 'z') return x - 'a' + 10;
	return x - '0';
}

int reind(int x)
{
	if (x >= 10 && x <= 15) return x - 10 + 'a';
	return x + '0';
}

bool cmp(int x, int y)
{
	return x > y;
}

int main()
{
	while (scanf("%d%d", &n, &k) != EOF)
	{
		memset(a, 0, sizeof a);
		memset(tot, 0, sizeof tot);
		memset(mm, 0, sizeof mm);
		for (int s = 0; s < n; s++)
		{
			char tmp[2]; scanf("%s", tmp);
			int d = ind(tmp[0]);
			a[d][tot[d]++] = s;
		}
		int linc = -1;
		for (int s = 1; s <= k; s++)
		{
			for (int t = 15; t >= 0; t--)
			{
				if (tot[t] == 0) continue;
				if (linc > a[t][tot[t]-1]) continue;
				int l = 0, r = tot[t] - 1;
				while (l < r)
				{
					int m = (l+r) >> 1;
					if (linc >= a[t][m]) l = m + 1;
					else r = m;
				}
				int cinc = a[t][r];
				if (cinc > linc && cinc + k - s < n)
				{
					printf("%c", reind(t));
					linc = cinc;
					break;
				}
			}
		}
		printf("\n");
	}
}

```

