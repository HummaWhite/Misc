#ifndef BSTNODE_H
#define BSTNODE_H

#include <iostream>
#include <ctime>
using namespace std;

#define LEFT  0
#define RIGHT 1
#define _h(node) ((node) == nullptr ? 0 : (node)->h)
#define _size(node) ((node) == nullptr ? 0 : (node)->size)

template<class T> struct BSTnode
{
	T val;
	int h, size, rnd;
	BSTnode<T> *ch[2];
	BSTnode(T v);
	void maintainNode();
};

template<class T> BSTnode<T>::BSTnode(T v)
{
	val = v;
	h = size = 1;
	rnd = rand() * rand();
	ch[0] = ch[1] = nullptr;
}

template<class T> void BSTnode<T>::maintainNode()
{
	h = max(_h(ch[0]), _h(ch[1])) + 1;
	size = _size(ch[0]) + _size(ch[1]) + 1;
}

#endif
