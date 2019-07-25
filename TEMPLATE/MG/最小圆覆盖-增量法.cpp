#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<cmath>
typedef double D;

using namespace std;

const int L = 100005;
const D eps = 1e-9;

int n;

struct point
{
	D x, y;
} p[L];

struct line
{
	D A, B, C;
	line(point E, point F)
	{
		A = E.y - F.y, B = F.x - E.x;
		C = E.x*F.y - E.y*F.x;
	}
	line(D AA, D BB, D CC)
	{
		A = AA, B = BB, C = CC;
	}
};

D dist(point A, point B)
{
	return sqrt((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y));
}

point mid(point A, point B)
{
	return point{(A.x+B.x) / 2, (A.y+B.y) / 2};
}

point joint(line e, line f)
{
	D x = (e.B*f.C - f.B*e.C) / (e.A*f.B - f.A*e.B);
	D y = (e.A*f.C - f.A*e.C) / (f.A*e.B - e.A*f.B);
	return point{x, y};
}

point outerCentre(point E, point F, point G)
{
	line H(E, F);
	line I(F, G);
	point M1 = mid(E, F);
	point M2 = mid(F, G);
	line X1(H.B, -H.A, H.A*M1.y - H.B*M1.x);
	line X2(I.B, -I.A, I.A*M2.y - I.B*M2.x);
	point C = joint(X1, X2);
	return C;
}

void minCircleCover()
{
	D curR = 0;
	point CP = p[0];
	for (int s = 1; s < n; s++)
	{
		if (dist(CP, p[s]) <= curR) continue;
		CP = p[s], curR = 0;
		for (int t = 0; t < s; t++)
		{
			if (dist(CP, p[t]) <= curR) continue;
			CP = mid(p[s], p[t]);
			curR = dist(p[s], p[t]) / 2;
			for (int k = 0; k < t; k++)
			{
				if (dist(CP, p[k]) <= curR) continue;
				CP = outerCentre(p[s], p[t], p[k]);
				curR = dist(CP, p[k]);
			}
		}
	}
	printf("%.3lf\n", curR);
	printf("%.3lf %.3lf\n", CP.x, CP.y);
}

int main()
{
	while (scanf("%d", &n) != EOF)
	{
		for (int s = 0; s < n; s++) scanf("%lf%lf", &p[s].x, &p[s].y);
		random_shuffle(p, p+n);
		minCircleCover();
	}
}
