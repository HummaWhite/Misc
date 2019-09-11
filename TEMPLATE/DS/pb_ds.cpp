//#pragma GCC optimize(3,"Ofast","inline")
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/hash_policy.hpp>
#include<cstring>
#include<cstdio>
#include<ctime>

using namespace __gnu_pbds;
using namespace std;

tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> tr;

void linear_test(int n)
{
	for (int s = 1; s < n; s++) tr.insert(s);
	for (int s = 1; s < n; s++) tr.find(s);
	for (int s = 1; s < n; s++) tr.erase(s);
}

void random_test(int n)
{
	for (int s = 0; s < n; s++)
	{
		int opt = rand() % 3, id = rand();
		if (opt == 0) tr.insert(id);
		else if (opt == 1) tr.find(id);
		else tr.erase(id);
	}
}

void test(int n)
{
	clock_t st = clock();
	random_test(n);
	clock_t nd = clock();
	printf("%lf\n", (double)(nd - st) / CLOCKS_PER_SEC);
}

int main()
{
	srand(time(NULL));
	tr.insert(0);
	test(16777215);
}
