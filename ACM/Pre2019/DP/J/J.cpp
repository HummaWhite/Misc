#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>

using namespace std;

const int INF = 100;

int pow5[9] = {1, 5, 25, 125, 625, 3125, 15625, 78125, 390625};
int banned[9] = {1020102, 10201022, 10201102, 10220102, 11020102, 10020102, 10200102, 10201002, 10201020};
int n, m, a[82], st, nd;
int f[82][400005], minst[400005], d[1000], id[400005], cnt = 1;
int bin[257][257], head[400005], tot = 0;
char ch[82][82];

int cntBit(int k) { int cnt = 0; while (k) k -= k&-k, cnt++; return cnt; }

struct edge
{
	int v, w, next;
} e[400005];

void addedge(int u, int v, int w)
{
	e[tot].v = v, e[tot].w = w;
	e[tot].next = head[u], head[u] = tot++;
}

void getminst(int k, int cur, int wis, int last, int seq, int mmax)
{
	if (wis == m)
	{
		if (minst[cur] == -1) return;
		minst[cur] = k;
		if (id[cur]) return;
		id[cur] = cnt, d[cnt++] = cur;
		return;
	}
	mmax = max(mmax, last);
	if ((k>>(m-wis-1)) & 1) getminst(k, cur*5, wis+1, last, 0, mmax);
	else
	{
		if (!last) getminst(k, cur*5+1, wis+1, 1, 1, mmax);
		else
		{
			getminst(k, cur*5 + last, wis+1, last, 1, mmax);
			if (!seq)
				for (int s = 1; s <= mmax+1; s++)
					getminst(k, cur*5 + s, wis+1, s, 1, mmax);
		}
	}
}

int chk[2][8], vhk[2][8], vf[5], vt[5];

inline int mod5(int x, int k) { return (x / pow5[k]) % 5; }

void check(int x, int y, int cur)
{
	vhk[y][x] = 1;
	int now = chk[y][x];
	chk[y][x] = cur;
	if (y == 1)
	{
		if (x > 0 && chk[y][x-1] != 0 && vhk[y][x-1] == 0) check(x-1, y, cur);
		if (x < m-1 && chk[y][x+1] != 0 && vhk[y][x+1] == 0) check(x+1, y, cur);
	}
	else
	{
		for (int s = x; s >= 0; s--)
			if (chk[0][s] == now && vhk[0][s] == 0) check(s, 0, cur);
	}
	if (chk[y^1][x] != 0 && vhk[y^1][x] == 0) check(x, y^1, cur);
}

int flg[8];

int cmp(int fr, int to)
{
	int ret = 1;
	for (int s = m-1; s >= 0; s--)
	{
		chk[0][s] = mod5(fr, s);
		chk[1][s] = mod5(to, s);
		vhk[0][s] = vhk[1][s] = 0;
	}
	int cf = 0;
	memset(vf, 0, sizeof vf);
	memset(vt, 0, sizeof vt);
	memset(flg, 0, sizeof flg);
	for (int s = m-1; s >= 0; s--)
	{
		int cur = chk[0][s];
		cf = max(cf, cur);
		if (chk[1][s] != 0) vf[cur] = 1;
	}
	for (int s = m-1; s >= 0; s--)
	{
		int cur = chk[0][s];
		if (cur != 0 && chk[1][s] != 0) check(s, 0, cur);
	}
	int add = 1;
	for (int s = m-1; s >= 0; s--)
	{
		int cur = chk[1][s], v = vhk[1][s];
		if (cur == 0) continue;
		if (flg[s]) continue;
		if (v == 0)
		{
			for (int t = s; t >= 0; t--)
			{
				if (chk[1][t] == 0) break;
				if (chk[1][t] == cur && flg[t] == 0 && vhk[1][t] == 0)
				{
					chk[1][t] = add;
					vhk[1][t] = 1;
					flg[t] = 1;
				}
			}
			add++;
		}
		else
		{
			for (int t = s; t >= 0; t--)
			{
				if (chk[1][t] == cur && flg[t] == 0 && vhk[1][t] != 0)
				{
					chk[1][t] = add;
					vhk[1][t] = 1;
					flg[t] = 1;
				}
			}
			add++;
		}
	}
	for (int s = 1; s <= cf; s++) if (!vf[s]) ret = 0;
	int ud = 0;
	for (int s = m-1; s >= 0; s--) ud += chk[1][s] * pow5[s];
	if (ud != to) ret = 0;
	return ret;
}

void init()
{
	int inc = 0; st = 0;
	while (mod5(a[st], inc) == 0) { inc++; if (inc == m) inc = 0, st++; }
	inc = 0, nd = n-1;
	while (mod5(a[nd], inc) == 0) { inc++; if (inc == m) inc = 0, nd--; }
	memset(bin, -1, sizeof bin);
	memset(minst, 0, sizeof minst);
	for (int s = 0; s < (1<<m); s++)
	{
		for (int t = 0; t < (1<<m); t++)
			if ((t & s) == t && s != t)
				bin[s][t] = cntBit(s ^ t);
		bin[s][s] = 0;
	}
	memset(id, 0, sizeof id);
	for (int s = 0; s < 9; s++)
	{
		int k = 0;
		for (int t = 0; t < 8; t++) k += ((int)(banned[s] / pow((int)10, (int)t)) % 10) * pow5[t];
		minst[k] = -1;
	}
	for (int s = 0; s < (1<<m); s++) getminst(s, 0, 0, 0, 1, 0);
	memset(head, -1, sizeof head);
	for (int s = 0; s < cnt; s++)
	{
		for (int t = 0; t < cnt; t++)
		{
			if (~bin[minst[d[s]]][minst[d[t]]])
			{
				addedge(d[s], d[t], bin[minst[d[s]]][minst[d[t]]]);
			}
		}
		addedge(d[s], d[s], 0);
	}
}

void dp()
{
	for (int s = 0; s < n; s++)
		for (int t = 1; t < cnt; t++)
			f[s][d[t]] = INF;
	int ans = INF;
	int ss = 0;
	for (int s = head[ss]; ~s; s = e[s].next)
	{
		int v = e[s].v;
		if (~bin[minst[a[st]]][minst[v]])
			if (cmp(ss, v))
			{
				f[st][v] = bin[minst[a[st]]][minst[v]];
			}
	}
	for (int s = st+1; s <= nd; s++)
	{
		for (int t = 1; t < cnt; t++)
		{
			int u = d[t];
			for (int k = head[a[s]]; ~k; k = e[k].next)
			{
				int v = e[k].v;
				if (~bin[minst[a[s]]][minst[v]])
				{
					if (cmp(u, v))
						f[s][v] = min(f[s][v], f[s-1][u] + e[k].w);
				}
			}
		}
	}
	for (int s = 1; s < cnt; s++)
	{
		if (d[s] == 0) continue;
		int p = 1;
		for (int t = m-1; t >= 0; t--)
			if (mod5(d[s], t) != 1 && mod5(d[s], t) != 0)
				p = 0;
		if (p) ans = min(ans, f[nd][d[s]]);
	}
	printf("%d\n", ans);
}

int main()
{
	scanf("%d%d", &n, &m);
	int fl = n < m;
	memset(a, 0, sizeof a);
	for (int s = 0; s < n; s++) scanf("%s", ch[s]);
	if (!fl)
	{
		for (int s = 0; s < n; s++)
		{
			int lst = 1, shft = ch[s][0] == '0' ? 1 : 0;
			for (int t = 0; t < m; t++)
			{
				if (ch[s][t] == '1')
				{
					lst = 0;
					continue;
				}
				if (!lst) shft++;
				lst = 1;
				a[s] += shft * pow5[m-1-t];
			}
		}
	}
	else
	{
		for (int s = 0; s < m; s++)
		{
			int lst = 1, shft = ch[0][s] == '0' ? 1 : 0;
			for (int t = 0; t < n; t++)
			{
				if (ch[t][s] == '1')
				{
					lst = 0;
					continue;
				}
				if (!lst) shft++;
				lst = 1;
				a[s] += shft * pow5[n-1-t];
			}
		}
		swap(n, m);
	}
	init();
	dp();
}
