#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>

using namespace std;

const int L = 200005;

int n, m, a[L], pre[L], nex[L], vis[L];

struct b
{
	int id, val;
	bool operator < (const b x) const
	{
		return val < x.val;
	}
};

priority_queue<b> Q;

int main()
{
	scanf("%d%d", &n, &m);
	for (int s = 0; s < n; s++)
	{
		scanf("%d", &a[s]);
		pre[s] = (s-1+n) % n;
		nex[s] = (s+1) % n;
	}
	if ((n>>1) < m)
	{
		printf("Error!\n");
		return 0;
	}
	for (int s = 0; s < n; s++)
	{
		b tmp; tmp.id = s;
		tmp.val = a[s];
		Q.push(tmp);
	}
	int cnt = 0, ans = 0;
	memset(vis, 0, sizeof vis);
	while (cnt < m)
	{
		b tp = Q.top();
		Q.pop();
		int id = tp.id;
		int pr = pre[id], nx = nex[id];
		int inc = nex[id];
		if (vis[id]) continue;
		ans += tp.val;
		vis[pr] = vis[nx] = 1;
		pre[id] = pre[pr], nex[id] = nex[nx];
		pre[nex[nx]] = id, nex[pre[pr]] = id;
		tp.val = a[pr] + a[nx] - a[id];
		a[id] = a[pr] + a[nx] - a[id];
		Q.push(tp);
		cnt++;
	}
	printf("%d\n", ans);
}
