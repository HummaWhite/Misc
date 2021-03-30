#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 1000006;

LL hsh[L], p = 998244353, mod = 1000000007;
LL pown[L];
char a[L];

inline int cc(int x) { return x - 'a'; }

LL get(int l, int r)
{
	return (hsh[r] - (hsh[l-1]*pown[r-l+1] % mod) + mod*2) % mod;
}

int main()
{
	scanf("%s", a);
	int len = strlen(a);
	pown[0] = 1;
	for (int s = 1; s <= len; s++) pown[s] = (pown[s-1] * p) % mod;
	hsh[0] = cc(a[0]);
	for (int s = 1; s < len; s++) hsh[s] = (hsh[s-1]*p + cc(a[s])) % mod;
	for (int s = 1; s <= len; s++)
		if (get(0, len-s-1) == get(s, len-1))
			printf("%d ", s);
	printf("\n");
}
