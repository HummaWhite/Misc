#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;
typedef double D;

using namespace std;

const int L = 50004;

LL S[L], a[L], dp[L], Q[L], n, l;

D slope(int x, int y)
{
	return (D)( (dp[x] + (a[x]+l)*(a[x]+l) - dp[y] - (a[y]+l)*(a[y]+l)) / ((a[x] - a[y]) * 2.0) );
}

int main()
{
	scanf("%lld%lld", &n, &l); l++;
	S[0] = 0;
	for (int s = 1; s <= n; s++)
	{
		scanf("%lld", &S[s]);
		S[s] += S[s-1];
		a[s] = S[s] + s;
	}
	memset(dp, 0, sizeof dp);
	int top = 1, tail = 1;
	memset(Q, 0, sizeof Q);
	for (int s = 1; s <= n; s++)
	{
		while (top < tail && slope(Q[top], Q[top+1]) <= a[s]) top++;
		int x = Q[top];
		dp[s] = dp[x] + (a[s]-a[x]-l)*(a[s]-a[x]-l);
		while (top < tail && slope(Q[tail], s) < slope(Q[tail], Q[tail-1])) tail--;
		Q[++tail] = s;
	}
	printf("%lld\n", dp[n]);
}
