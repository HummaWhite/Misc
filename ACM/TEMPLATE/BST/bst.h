#ifndef BST_H
#define BST_H

#include "bstnode.h"

template<class T> struct BST
{
	BST<T>();
	~BST<T>();
	BSTnode<T> *rt;
	int size();
	int height();
	void rotate(BSTnode<T>* &k, int dir);
	virtual void rebalanceNode(BSTnode<T>* &k);
	virtual void insertNode(BSTnode<T>* &k, T v);
	virtual void removeNode(BSTnode<T>* &k, T v);
	BSTnode<T>* findNode(BSTnode<T>* k, T v);
	T kth(BSTnode<T>* k, int x);
	int rnk(BSTnode<T>* k, T v);
	T pre(BSTnode<T>* k, T v);
	T suc(BSTnode<T>* k, T v);
	void clearTree(BSTnode<T>* &k);
	void insert(T v);
	void remove(T v);
	bool find(T v);
	int count(T v);
	T findKth(int x);
	int getRank(T v);
	T prev(T v);
	T succ(T v);
	void clear();
	void dfs(BSTnode<T>* k);
	void printTree();
};

template<class T> BST<T>::BST()
{
	rt = nullptr;
}

template<class T> BST<T>::~BST()
{
	clear();
}

template<class T> int BST<T>::size()
{
	return _size(rt);
}

template<class T> int BST<T>::height()
{
	return _h(rt);
}

template<class T> void BST<T>::rotate(BSTnode<T>* &k, int dir)
{
	BSTnode<T>* tmp = k->ch[dir ^ 1];
	k->ch[dir ^ 1] = tmp->ch[dir];
	tmp->ch[dir] = k;
	k->maintainNode();
	tmp->maintainNode();
	k = tmp;
}

template<class T> void BST<T>::rebalanceNode(BSTnode<T>* &k)
{}

template<class T> void BST<T>::insertNode(BSTnode<T>* &k, T v)
{
	if (k == nullptr) k = new BSTnode<T>(v);
	else insertNode(k->ch[v > k->val], v);
	k->maintainNode();
	rebalanceNode(k);
}

template<class T> void BST<T>::removeNode(BSTnode<T>* &k, T v)
{
	if (k == nullptr) return;
	if (v != k->val) removeNode(k->ch[v > k->val], v);
	else
	{
		if (k->ch[0] != nullptr && k->ch[1] != nullptr)
		{
			int dir = rand() & 1;
			BSTnode<T>* tmp = k->ch[dir];
			while (tmp->ch[dir ^ 1] != nullptr) tmp = tmp->ch[dir ^ 1];
			k->val = tmp->val;
			removeNode(k->ch[dir], tmp->val);
		}
		else
		{
			BSTnode<T> *tmp = k->ch[0] == nullptr ? k->ch[1] : k->ch[0];
			delete k;
			k = tmp;
		}
	}
	if (k != nullptr) k->maintainNode();
}

template<class T> BSTnode<T>* BST<T>::findNode(BSTnode<T>* k, T v)
{
	if (k == nullptr) return nullptr;
	if (v == k->val) return k;
	findNode(k->ch[v > k->val], v);
}

template<class T> T BST<T>::kth(BSTnode<T>* k, int x)
{
	int lsize = _size(k->ch[0]);
	if (x <= lsize) return kth(k->ch[0], x);
	x -= lsize + 1;
	if (x <= 0) return k->val;
	else return kth(k->ch[1], x);
}

template<class T> int BST<T>::rnk(BSTnode<T>* k, T v)
{
	int ret = 0;
	if (k == nullptr) return 1;
	if (v <= k->val) ret = rnk(k->ch[0], v);
	else ret = _size(k->ch[0]) + 1 + rnk(k->ch[1], v);
	return ret;
}

template<class T> T BST<T>::pre(BSTnode<T>* k, T v)
{
	if (k == nullptr) return v;
	T ret;
	if (v <= k->val) ret = pre(k->ch[0], v);
	else
	{
		ret = pre(k->ch[1], v);
		if (v == ret) ret = k->val;
	}
	return ret;
}

template<class T> T BST<T>::suc(BSTnode<T>* k, T v)
{
	if (k == nullptr) return v;
	T ret;
	if (v >= k->val) ret = suc(k->ch[1], v);
	else
	{
		ret = suc(k->ch[0], v);
		if (v == ret) ret = k->val;
	}
	return ret;
}

template<class T> void BST<T>::clearTree(BSTnode<T>* &k)
{
	if (k == nullptr) return;
	if (k->ch[0] == nullptr && k->ch[1] == nullptr)
	{
		delete k;
		k = nullptr;
		return;
	}
	if (k->ch[0] != nullptr) clearTree(k->ch[0]);
	if (k->ch[1] != nullptr) clearTree(k->ch[1]);
}

template<class T> void BST<T>::insert(T v)
{
	insertNode(rt, v);
}

template<class T> void BST<T>::remove(T v)
{
	removeNode(rt, v);
}

template<class T> bool BST<T>::find(T v)
{
	return findNode(rt, v) != nullptr;
}

template<class T> int BST<T>::count(T v)
{
	BSTnode<T>* node = findNode(rt, v);
	if (node == nullptr) return 0;
	int ret = 0;
	while (node->val == v && node->ch[0] != nullptr)
	{
		node = node->ch[0];
		ret++;
	}
	return ret + 1;
}

template<class T> T BST<T>::findKth(int x)
{
	return kth(rt, x);
}

template<class T> int BST<T>::getRank(T v)
{
	return rnk(rt, v);
}

template<class T> T BST<T>::prev(T v)
{
	return pre(rt, v);
}

template<class T> T BST<T>::succ(T v)
{
	return suc(rt, v);
}

template<class T> void BST<T>::clear()
{
	clearTree(rt);
	if (rt != nullptr)
	{
		delete rt;
		rt = nullptr;
	}
}

template<class T> void BST<T>::dfs(BSTnode<T>* k)
{
	if (k == nullptr) return;
	dfs(k->ch[0]);
	cout << k->val << endl;
	dfs(k->ch[1]);
}

template<class T> void BST<T>::printTree()
{
	dfs(rt);
}

#endif
