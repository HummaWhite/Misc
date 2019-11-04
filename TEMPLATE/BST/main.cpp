#include "bst.h"
#include "avl.h"
#include "sbt.h"
#include "treap.h"

void test(BST<int>* tree)
{
	for (int i = 0; i < 10000; i++)
	{
		int opt = rand() & 1;
		if (opt) tree->insert(rand());
		else tree->remove(rand());
	}
	cout << tree->height() << endl;
}

int main()
{
	srand(time(NULL));
	string a = "aaaa", b = "bbbbb", c = "ccc", d = "abcd";
	AVL<string> A;
	A.insert(a);
	A.insert(b);
	A.insert(c);
	A.insert(d);
	cout << A.size() << " " << A.height() << endl;
	cout << A.findKth(2) << endl;
	cout << A.getRank(b) << endl;
	cout << A.prev(b) << endl;
	cout << A.succ(b) << endl;
	A.printTree();
	A.remove(b);
	cout << A.size() << " " << A.height() << endl;
	AVL<int> C;
	test(&C);
	Treap<int> D;
	test(&D);
	BST<int> E;
	test(&E);
	//C.printTree();
}
