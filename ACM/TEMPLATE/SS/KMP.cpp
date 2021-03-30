#include<iostream>
#include<cstring>
#include<cstdio>

using namespace std;

const int L = 1000006;

char ch[L], tem[L];

int f[L], lench, lent;

void getnext()
{
	f[0] = f[1] = 0;
	for (int s = 1; s < lent; s++)
	{
		int inc = f[s];
		while (inc && tem[s] != tem[inc]) inc = f[inc];
		f[s+1] = tem[s] == tem[inc] ? inc+1 : 0;
	}
	//for (int s = 0; s <= lent; s++) printf("%d ", f[s]);
}

void find()
{
	int inc = 0;
	for (int s = 0; s < lench; s++)
	{
		while (inc && ch[s] != tem[inc]) inc = f[inc];
		if (ch[s] == tem[inc]) inc++;
		if (inc == lent) printf("%d ", s-lent+2);
	}
}

int main()
{
	scanf("%s%s", ch, tem);
	lench = strlen(ch), lent = strlen(tem);
	getnext();
	find();
}

