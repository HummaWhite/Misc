
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 5000005;
const int ms = 31;

struct trie
{
	int u, v, cnt;
	trie() {u = v = -1, cnt = 0;}
};

trie a[L][2];

int tot = 0, n;

void insert(LL k)
{
	int inc = 0;
	for (int s = ms; s >= 0; s--)
	{
		int u = (k >> s) & 1;
		if (a[inc][u].v == -1) a[inc][u].v = ++tot;
		a[inc][u].cnt++;
		inc = a[inc][u].v;
	}
}

void del(LL k)
{
	int inc = 0;
	for (int s = ms; s >= 0; s--)
	{
		int u = (k >> s) & 1;
		a[inc][u].cnt--;
		inc = a[inc][u].v;
	}
}

LL queryMax(LL k)
{
	int ans = 0, inc = 0;
	for (int s = ms; s >= 0; s--)
	{
		LL u = (k >> s) & 1;
		if (a[inc][u^1].cnt != 0)
		{
			ans += 1 << s;
			inc = a[inc][u^1].v;
		}
		else inc = a[inc][u].v;
	}
	return ans;
}

LL queryMin(LL k)
{
	LL ans = 0, inc = 0;
	for (int s = ms; s >= 0; s--)
	{
		LL u = (k >> s) & 1;
		if (a[inc][u].cnt != 0) inc = a[inc][u].v;
		else
		{
			ans += 1 << s;
			inc = a[inc][u^1].v;
		}
	}
	return ans;
}

int main()
{
	scanf("%d", &n);
	while (n--)
	{
		int opt; LL k;  scanf("%d%lld", &opt, &k);
		if (opt == 1) insert(k);
		else if (opt == 2) del(k);
		else printf("%lld %lld\n", queryMin(k), queryMax(k));
	}
}

