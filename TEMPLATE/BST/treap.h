#ifndef TREAP_H
#define TREAP_H

#include "bst.h"

template<class T> struct Treap:
	BST<T>
{
	Treap();
	void insertNode(BSTnode<T>* &k, T v);
	void removeNode(BSTnode<T>* &k, T v);
};

template<class T> Treap<T>::Treap():
	BST<T>()
{
}

template<class T> void Treap<T>::insertNode(BSTnode<T>* &k, T v)
{
	if (k == nullptr) k = new BSTnode<T>(v);
	else
	{
		insertNode(k->ch[v > k->val], v);
		if (k->ch[v > k->val]->rnd < k->rnd) this->rotate(k, v <= k->val);
	}
	k->maintainNode();
}

template<class T> void Treap<T>::removeNode(BSTnode<T>* &k, T v)
{
	if (k == nullptr) return;
	if (v != k->val) this->removeNode(k->ch[v > k->val], v);
	else
	{
		if (k->ch[0] != nullptr && k->ch[1] != nullptr)
		{
			int dir = k->ch[0]->rnd < k->ch[1]->rnd;
			this->rotate(k, dir);
			removeNode(k->ch[dir], v);
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
