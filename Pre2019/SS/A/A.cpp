#include<iostream>
#include<cstring>
#include<cstdio>
typedef long long LL;

using namespace std;

const int L = 1000006;
const int ms = 31;

struct trie
{
	int u, v, cnt;
	trie() {u = v = -1, cnt = 0;}
};

trie a[L][26], b[L][26];
char ch[L];
int tot = 0, cnt = 0, n;

void insertA(int len)
{
	int inc = 0;
	for (int s = 0; s < len; s++)
	{
		int u = ch[s] - 'a';
		if (a[inc][u].v == -1) a[inc][u].v = ++tot;
		a[inc][u].cnt++;
		inc = a[inc][u].v;
	}
}

void insertB(int len)
{
	int inc = 0;
	for (int s = len-1; s >= 0; s--)
	{
		int u = ch[s] - 'a';
		if (b[inc][u].v == -1) b[inc][u].v = ++cnt;
		b[inc][u].cnt++;
		inc = b[inc][u].v;
	}
}

int queryA(int len)
{
	int inc = 0, ans = 0;
	for (int s = 0; s < len; s++)
	{
		int u = ch[s] - 'a';
		if (inc == -1) break;
		ans = a[inc][u].cnt;
		inc = a[inc][u].v;
	}
	return ans;
}

int queryB(int len)
{
	int inc = 0, ans = 0;
	for (int s = len-1; s >= 0; s--)
	{
		int u = ch[s] - 'a';
		if (inc == -1) break;
		ans = b[inc][u].cnt;
		inc = b[inc][u].v;
	}
	return ans;
}

int main()
{
	scanf("%d", &n);
	while (n--)
	{
		scanf("%s", ch);
		int len = strlen(ch);
		printf("%d %d\n", queryA(len), queryB(len));
		insertA(len), insertB(len);
	}
}
