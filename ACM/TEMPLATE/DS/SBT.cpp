//#pragma GCC optimize(3,"Ofast","inline")
#include<iostream>
#include<cstring>
#include<cstdio>
#include<ctime>

using namespace std;

struct node
{
	int val, size, cnt;
	node* ch[2];
	node(int x) { ch[0] = ch[1] = NULL, val = x, size = cnt = 1; }
	int cmp(int x)
	{
		return x == val ? -1 : x > val;
	}
	int sz()
	{
		return this == NULL ? 0 : size;
	}
	void maintain()
	{
		size = ch[0]->sz() + ch[1]->sz() + 1;
	}
} *rt;

  
void rotate(node* &k, int d)
{
	node* tmp = k->ch[d^1];
	k->ch[d^1] = tmp->ch[d];
	tmp->ch[d] = k;
	k->maintain();
	tmp->maintain();
	k = tmp;
}

void rebalance(node* &k, int d)
{
	if (k == NULL) return;
	if (k->ch[d] == NULL) return;
	if (k->ch[d]->ch[d]->sz() > k->ch[d^1]->sz()) rotate(k, d^1);
	else if (k->ch[d]->ch[d^1]->sz() > k->ch[d^1]->sz()) rotate(k->ch[d], d), rotate(k, d^1);
	else return;
	rebalance(k->ch[d], 0);
	rebalance(k->ch[d^1], 1);
	rebalance(k, 0), rebalance(k, 1);
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
	rebalance(k, x >= k->val);
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
				int dd = k->ch[0]->sz() < k->ch[1]->sz();
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
	printf("%d %d %d %d\n", k->val, k->sz(), k->ch[0]->sz(), k->ch[1]->sz());
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
