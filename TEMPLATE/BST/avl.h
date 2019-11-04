#ifndef AVL_H
#define AVL_H

#include "bst.h"

template<class T> struct AVL:
	BST<T>
{
	AVL();
	void rebalanceNode(BSTnode<T>* &k);
	void removeNode(BSTnode<T>* &k, T v);
};

template<class T> AVL<T>::AVL():
	BST<T>()
{
}

template<class T> void AVL<T>::rebalanceNode(BSTnode<T>* &k)
{
	int lh = _h(k->ch[0]);
	int rh = _h(k->ch[1]);
	if (lh + 1 < rh) this->rotate(k, LEFT);
	if (rh + 1 < lh) this->rotate(k, RIGHT);
}

template<class T> void AVL<T>::removeNode(BSTnode<T>* &k, T v)
{
	if (k == nullptr) return;
	if (v != k->val) removeNode(k->ch[v > k->val], v);
	else
	{
		if (k->ch[0] != nullptr && k->ch[1] != nullptr)
		{
			int dir = _h(k->ch[0]) < _h(k->ch[1]);
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

#endif
