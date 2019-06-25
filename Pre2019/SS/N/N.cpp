#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
#include<set>
typedef long long LL;

using namespace std;

const int L = 2005;

struct pos
{
	int x, y, l, r;
};

queue<pos> Q;

char tmp[L][L];

int n, m, sx, sy, ll, rr, a[L][L], vis[L][L], ans = 0;

inline void psh(int x, int y, int l, int r)
{
	Q.push(pos{x, y, l, r});
	vis[y][x] = 1;
	ans++;
}

void search()
{
	memset(vis, 0, sizeof vis);
	psh(sx, sy, ll, rr);
	for (int s = sy-1; s >= 1 && !a[s][sx]; s--) psh(sx, s, ll, rr);
	for (int s = sy+1; s <= n && !a[s][sx]; s++) psh(sx, s, ll, rr);
	while (!Q.empty())
	{
		pos tmp = Q.front(); Q.pop();
		int x = tmp.x, y = tmp.y;
		int l = tmp.l, r = tmp.r;
		vis[y][x] = 1;
		if (!vis[y][x-1] && !a[y][x-1] && l)
		{
			psh(x-1, y, l-1, r);
			for (int s = y-1; s >= 1 && !vis[s][x-1] && !a[s][x-1]; s--) psh(x-1, s, l-1, r);
			for (int s = y+1; s <= n && !vis[s][x-1] && !a[s][x-1]; s++) psh(x-1, s, l-1, r);
		}
		if (!vis[y][x+1] && !a[y][x+1] && r)
		{
			psh(x+1, y, l, r-1);
			for (int s = y-1; s >= 1 && !vis[s][x+1] && !a[s][x+1]; s--) psh(x+1, s, l, r-1);
			for (int s = y+1; s <= n && !vis[s][x+1] && !a[s][x+1]; s++) psh(x+1, s, l, r-1);
		}
	}
	printf("%d\n", ans);
}

int main()
{
	scanf("%d%d", &n, &m);
	scanf("%d%d", &sy, &sx);
	scanf("%d%d", &ll, &rr);
	for (int s = 0; s < n; s++) scanf("%s", tmp[s]);
	for (int s = 0; s < n; s++)
		for (int t = 0; t < m; t++)
			a[s+1][t+1] = tmp[s][t] == '*' ? 1 : 0;
	for (int s = 0; s < n+2; s++)
		a[s][0] = a[s][m+1] = a[0][s] = a[n+1][s] = 1;
	search();
}
