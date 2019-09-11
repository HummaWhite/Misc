//#pragma GCC optimize(3,"Ofast","inline")
#include<iostream>
#include<cstring>
#include<cstdio>
#include<ctime>
#include<set>
#include<map>

using namespace std;

set<int> st;

void linear_test(int n)
{
	for (int s = 1; s < n; s++) st.insert(s);
	for (int s = 1; s < n; s++) st.find(s);
	for (int s = 1; s < n; s++) st.erase(s);
}

void random_test(int n)
{
	for (int s = 0; s < n; s++)
	{
		int opt = rand() % 3, id = rand();
		if (opt == 0) st.insert(id);
		else if (opt == 1) st.find(id);
		else st.erase(id);
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
	st.insert(0);
	test(16777215);
}
