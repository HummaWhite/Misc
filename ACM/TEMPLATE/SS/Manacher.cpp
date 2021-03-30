#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 11000007;
int p[L<<1], n;
char tg[L<<1], ch[L<<1];

void fill()
{
	int len = strlen(ch);
	int inc = 0;
	tg[inc++] = '[', tg[inc++] = '#';
	for (int s = 0; s < len; s++)
	{
		tg[inc++] = ch[s];
		tg[inc++] = '#';
	}
	tg[inc] = '\0';
}

int manacher()
{
	int ans = 0;
	fill();
	int len = strlen(tg);
	int mr = 0, inc = 0;
	for (int s = 1; s <= len; s++)
	{
		if (mr > s) p[s] = min(mr - s, p[inc*2 - s]);
		else p[s] = 1;
		while (tg[s - p[s]] == tg[s + p[s]]) p[s]++;
		if (p[s] + s > mr)
		{
			mr = p[s] + s;
			inc = s;
		}
	}
	for (int s = 1; s < len; s++)
		if (tg[s] == '#')
			ans = max(ans, p[s] - 1);
	return ans;
}

int main()
{
	scanf("%d%s", &n, ch);
	printf("%d\n", manacher());
}
