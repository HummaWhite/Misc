//#pragma GCC optimize(3,"Ofast","inline")
#include<iostream>
#include<cstring>
#include<cstdio>
#include<ctime>

using namespace std;

struct node
{
	int val, h, cnt;
	node *ch[2];
	node() { ch[0] = ch[1] = NULL, cnt = 1; }
	node(int x) { val = x, ch[0] = ch[1] = NULL, h = 0, cnt = 1; }
	int cmp(int x)
	{
		return x == val ? -1 : x > val;
	}
	inline int getH()
	{
		return this == NULL ? 0 : h;
	}
	void maintain()
	{
		h = max(ch[0]->getH(), ch[1]->getH()) + 1;
	}
} *rt;

void rotate(node* &k, int d)
{
	node *tmp = k->ch[d^1];
	k->ch[d^1] = tmp->ch[d];
	tmp->ch[d] = k;
	k->maintain();
	tmp->maintain();
	k = tmp;
}

void rebalance(node* &k)
{
	int lh = k->ch[0]->getH();
	int rh = k->ch[1]->getH();
	if (lh + 1 < rh) rotate(k, 0);
	if (rh + 1 < lh) rotate(k, 1);
}

int cnt = 0;

void insert(node* &k, int x)
{
	cnt++;
	if (k == NULL) k = new node(x);
	else
	{
		int d = k->cmp(x);
		if (d == -1) k->cnt++;
		else insert(k->ch[d], x);
	}
	k->maintain();
	rebalance(k);
}

void remove(node* &k, int x)
{
	cnt++;
	if (k == NULL) return;
	int d = k->cmp(x);
	if (d != -1) remove(k->ch[d], x);
	else
	{
		if (k->cnt > 1) k->cnt--;
		else
		{
			if (k->ch[0] != NULL && k->ch[1] != NULL)
			{
				int dd = k->ch[0]->getH() < k->ch[1]->getH();
				node* prev = k->ch[dd];
				while (prev->ch[dd^1] != NULL) prev = prev->ch[dd^1];
				k->val = prev->val, k->cnt = prev->cnt;
				remove(k->ch[dd], prev->val);
			}
			else k = k->ch[0] == NULL ? k->ch[1] : k->ch[0];
		}
	}
	if (k != NULL) k->maintain();
}

int find(node* k, int x)
{
	cnt++;
	if (k == NULL) return 0;
	int d = k->cmp(x);
	if (d == -1) return 1;
	find(k->ch[d], x);
}

void dfs(node* k)
{
	printf("%d %d %d %d\n", k->val, k->getH(), k->ch[0]->getH(), k->ch[1]->getH());
	if (k->ch[0] != NULL)
	{
		printf("L ");
		dfs(k->ch[0]);
	}
	if (k->ch[1] != NULL)
	{
		printf("R ");
		dfs(k->ch[1]);
	}
}

void random_test(int n)
{
	for (int s = 0; s < n; s++)
	{
		int opt = rand() % 3, id = rand();
		if (opt == 0) insert(rt, id);
		else if (opt == 1) find(rt, id);
		else remove(rt, id);
	}
}

void linear_test(int n)
{
	for (int s = 1; s < n; s++) insert(rt, s);
	for (int s = 1; s < n; s++) find(rt, s);
	for (int s = 1; s < n; s++) remove(rt, s);
}

void test(int n)
{
	clock_t st = clock();
	random_test(n);
	printf("%d %d\n", cnt, cnt / n);
	clock_t nd = clock();
	printf("%lf\n", (double)(nd - st) / CLOCKS_PER_SEC);
}

int main()
{
	srand(time(NULL));
	rt = new node(0);
	test(16777215);
	//dfs(rt);
}
