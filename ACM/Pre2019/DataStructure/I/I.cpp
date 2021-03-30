#include<algorithm>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<ctime>
typedef long long LL;

using namespace std;

const LL base = 1000000000LL;
const int L = 100005;

int n, m;
LL a[L];

struct node
{
	node *ch[2];
	LL val, rnd, cnt, size;
	node(LL x) {rnd = rand(), size = cnt = 1, ch[0] = ch[1] = NULL; val = x;}
	int cmp(LL x)
	{
		if (x == val) return -1;
		return (x > val ? 1 : 0);
	}
	void maintain()
	{
		size = cnt;
		if (ch[0] != NULL) size += ch[0]->size;
		if (ch[1] != NULL) size += ch[1]->size;
	}
};

void rotate(node* &k, int d)
{
	node *son = k->ch[d^1];
	k->ch[d^1] = son->ch[d];
	son->ch[d] = k;
	k->maintain(), son->maintain();
	k = son;
}

void insert(node* &k, LL x)
{
	if (k == NULL) k = new node(x);
	else
	{
		int d = k->cmp(x);
		if (d == -1) {k->cnt++;}
		else
		{
			insert(k->ch[d], x);
			if (k->ch[d]->rnd < k->rnd) rotate(k, d^1);
		}
	}
	k->maintain();
}

void remove(node* &k, LL x)
{
	//printf("%lld %d %d %d\n", k->val, k->rnd, k->cnt, k->size);
	int d = k->cmp(x);
	if (d != -1) remove(k->ch[d], x);
	else
	{
		if (k->cnt > 1) {k->cnt--;}
		else
		{
			if (k->ch[0] != NULL && k->ch[1] != NULL)
			{
				int dd = (k->ch[0]->rnd < k->ch[1]->rnd ? 1 : 0);
				rotate(k, dd);
				remove(k->ch[dd], x);
			}
			else
			{
				if (k->ch[0] == NULL && k->ch[1] == NULL) k = NULL;
				else if (k->ch[0] == NULL) k = k->ch[1];
				else if (k->ch[1] == NULL) k = k->ch[0];
			}
		}
	}
	if (k != NULL) k->maintain();
}

LL rnk(node *k, LL x)
{
	LL ret = 0;
	while (k != NULL)
	{
		int d = k->cmp(x);
		if (k->ch[1] != NULL && d != 1) ret += k->ch[1]->size;
		if (d == -1) return ret;
		if (d == 0) ret += k->cnt;
		k = k->ch[d];
	}
}

int main()
{
	srand(time(NULL));
	scanf("%d%d", &n, &m);
	for (int s = 0; s <= n; s++) a[s] = base - 1;
	node *root = new node(base - 1);
	root->rnd = -1;
	insert(root, base - 1);
	while (m--)
	{
		LL t, p;
		scanf("%lld%lld", &t, &p);
		if (a[t] != base - 1) remove(root, a[t]);
		a[t] += base - p;
		insert(root, a[t]);
		printf("%lld\n", rnk(root, a[1]) + 1);
	}
}
