#pragma GCC optimize(3, "Ofast", "inline")
#include "bst.h"
#include "avl.h"
#include "sbt.h"
#include "treap.h"
#include <ctime>

struct TreeInfo
{
	 int totLeaf, sumDepth, sumLeafDeapth;
};

template<class T> TreeInfo getTreeInfo(BSTnode<T>* k, int dep)
{
	if (k == nullptr) return TreeInfo{0, 0, 0};
	if (k->ch[0] == nullptr && k->ch[1] == nullptr) return TreeInfo{1, dep - 1, dep - 1};
	TreeInfo ls = getTreeInfo(k->ch[0], dep + 1), rs = getTreeInfo(k->ch[1], dep + 1);
	return TreeInfo{
				ls.totLeaf + rs.totLeaf,
				ls.sumDepth + rs.sumDepth + dep - 1,
				ls.sumLeafDeapth + rs.sumLeafDeapth};
}

template<class T> void test(BST<T>* tree)
{
	clock_t st = clock();
	for (int i = 0; i < 1048576; i++) tree->insert(rand() * rand());
	clock_t nd = clock();
	TreeInfo res = getTreeInfo(tree->rt, 1);
	cout << "time: " << (double)(nd - st) / 1000 <<endl;
	cout << "size: " << tree->size() << endl;
	cout << "leaves: " << res.totLeaf << endl;
	cout << "max depth: " << tree->height() << endl;
	cout << "average depth: " << (double)res.sumDepth / tree->size() << endl;
	cout << "average leaf depth: " << (double)res.sumLeafDeapth / res.totLeaf << endl;
	cout << "leaf/tot rate: " << (double)res.totLeaf / tree->size() << endl << endl;
}

void testAVL(int bf)
{
	cout << "maxLRSubTreeHeightDifference: " << bf << endl;
	AVL<int> C(bf);
	test(&C);
}

int main()
{
	freopen("test2.txt", "w", stdout);
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
	for (int i = 0; i <= 32; i++) testAVL(i);
	Treap<int> H;
	test(&H);
	//C.printTree();
}
