#include<algorithm>
#include<iostream>
#include<cstring>
#include<complex>
#include<cstdio>
#include<vector>
#include<cmath>
typedef long long LL;
typedef double D;

using namespace std;

const int L = 50004;

int n, tot = 0;

struct point
{
	int x, y;
	point() {}
	point(int xx, int yy) { x = xx, y = yy; }
	bool operator < (const point rhs) const
	{
		return x == rhs.x ? y < rhs.y : x < rhs.x;
	}
	bool operator == (const point rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
} p[L], ch[L];

point vec(point a, point b)
{
	return point{b.x-a.x, b.y-a.y};
}

int cross(point a, point b)
{
	return a.x*b.y - b.x*a.y;
}

int distSq(point a, point b)
{
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}

void getConvexHull()
{
	int top = 0;
	for (int s = 0; s < n; s++)
	{
		while (top > 1 && cross(vec(ch[top-2], ch[top-1]), vec(ch[top-1], p[s])) < 0) top--;
		ch[top++] = p[s];
	}
	int cur = top;
	for (int s = n-2; s >= 0; s--)
	{
		while (top > cur && cross(vec(ch[top-1], ch[top-2]), vec(p[s], ch[top-1])) < 0) top--;
		ch[top++] = p[s];
	}
	tot = n > 1 ? top-1 : top;
	ch[tot] = ch[0];
	/*printf("%d\n", tot);
	for (int s = 0; s < tot; s++) printf("%d %d\n", ch[s].x, ch[s].y);*/
}

void rotatingCalipers()
{
	int ans = 0, top = 2 % tot;
	for (int s = 0; s < tot; s++)
	{
		while (cross(vec(ch[s], ch[top]), vec(ch[s], ch[s+1])) > cross(vec(ch[s], ch[top+1]), vec(ch[s], ch[s+1]))) top = (top+1) % tot;
		ans = max(ans, max(distSq(ch[s], ch[top]), distSq(ch[s+1], ch[top])));
	}
	printf("%d\n", ans);
}

int main()
{
	scanf("%d", &n);
	for (int s = 0; s < n; s++) scanf("%d%d", &p[s].x, &p[s].y);
	sort(p, p+n);
	n = unique(p, p+n) - p;
	getConvexHull();
	rotatingCalipers();
}
