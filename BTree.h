#pragma once
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
struct Node
{
    T data;
    Node<T> *left, *right, *parent;
    Node(const T& _data, Node<T>* _left = nullptr, Node<T>* _right = nullptr);
};

template<typename T>
class BTree
{
private:
    Node<T>* root;
    size_t size;

    bool isLeaf(Node<T>* crr)const;

    Node<T>* copyNode(Node<T>* crr);
    void copy(const BTree<T>& other);
    void deleteSubTree(Node<T>* crr);
    void erase();

    void printNode(Node<T>* crr);
    void printLeafNode(Node<T>* crr);

    void pushDataInVector(Node<T>* crr, vector<T>& vec);
    void pushLeafsInVector(Node<T>* crr, vector<T>& vec);

    void removeByDataHelper(Node<T>* crr, T _data);
    Node<T>* topLeft(Node<T>* crr);
    Node<T>* topRight(Node<T>* crr);
    void swapData(Node<T>* fst, Node<T>* snd);

    bool containsHelper(Node<T>* crr, T _data);
public:
    BTree();
    BTree(const BTree<T>& other);
    BTree<T>& operator=(const BTree<T>& other);
    ~BTree();

    size_t getSize()const;
    bool isEmpty()const;
    bool contains(T _data);
    T head()const;
    Node<T>* getRoot()const;

    void insert(T element);
    void removeByData(T _data);
    void removeAllByData(T _data);
    void clear();

    void print();
    void printLeafs();
    void printRoot()const;

    vector<T> getDataInVector();
    vector<T> getLeafsInVector();
};



template<typename T>
Node<T>::Node(const T& _data, Node<T>* _left, Node<T>* _right)
         : data(_data), left(_left), right(_right){parent = nullptr;}



template<typename T>
bool BTree<T>::isLeaf(Node<T>* crr)const
{
    return crr->left == nullptr && crr->right == nullptr;
}

template<typename T>
Node<T>* BTree<T>::copyNode(Node<T>* crr)
{
    if(crr == nullptr)
    {
        return nullptr;
    }
    Node<T>* tmp = new Node<T>(crr->data, copyNode(crr->left), copyNode(crr->right));
    if(tmp->left)
    {
        tmp->left->parent = tmp;
    }
    if(tmp->right)
    {
        tmp->right->parent = tmp;
    }
    return tmp;
}

template<typename T>
void BTree<T>::copy(const BTree<T>& other)
{
    size = other.size;
    root = copyNode(other.root);
}

template<typename T>
void BTree<T>::deleteSubTree(Node<T>* crr)
{
    if(!crr)
    {
        return;
    }
    deleteSubTree(crr->left);
    deleteSubTree(crr->right);
    delete crr;
}

template<typename T>
void BTree<T>::erase()
{
    deleteSubTree(root);
    root = nullptr;
    size = 0;
}

template<typename T>
void BTree<T>::printNode(Node<T>* crr)
{
    if(crr)
    {
        cout << crr->data << " ";
        printNode(crr->left);
        printNode(crr->right);
    }
}

template<typename T>
void BTree<T>::printLeafNode(Node<T>* crr)
{
    if(crr)
    {
        if(isLeaf(crr))
        {
            cout << crr->data << " ";
        }
        printLeafNode(crr->left);
        printLeafNode(crr->right);
    }
}

template<typename T>
void BTree<T>::pushDataInVector(Node<T>* crr, vector<T>& vec)
{
    if(crr)
    {
        vec.push_back(crr->data);
        pushDataInVector(crr->left, vec);
        pushDataInVector(crr->right, vec);
    }
}

template<typename T>
void BTree<T>::pushLeafsInVector(Node<T>* crr, vector<T>& vec)
{
    if(crr)
    {
        if(isLeaf(crr))
        {
            vec.push_back(crr->data);
        }
        pushLeafsInVector(crr->left, vec);
        pushLeafsInVector(crr->right, vec);
    }
}

template<typename T>
void BTree<T>::removeByDataHelper(Node<T>* crr, T _data)
{
    if(crr->data == _data)
    {
        if(crr->right)
        {
            Node<T>* toswap = topLeft(crr->right);
            swapData(crr, toswap);
            Node<T>* save = toswap;
            if(toswap->right)
            {
                toswap->parent->right = copyNode(toswap->right);
            }
            else
            {
                toswap->parent->right = nullptr;
            }
            delete save;
        }
        else if(crr->left)
        {
            Node<T>* toswap = topRight(crr->left);
            swapData(crr, toswap);
            Node<T>* save = toswap;
            if(toswap->left)
            {
                toswap->parent->left = copyNode(toswap->left);
            }
            else
            {
                toswap->parent->left = nullptr;
            }
            delete save;
        }
        else
        {
            if(crr->parent)
            {
                if(crr->parent->left->data == crr->data)
                {
                    crr->parent->left = nullptr;
                }
                else
                {
                    crr->parent->right = nullptr;
                }
            }
            delete crr;
        }
        return;
    }
    else
    {
        if(crr->left)
        {
            removeByDataHelper(crr->left, _data);
        }
        if(crr->right)
        {
            removeByDataHelper(crr->right, _data);
        }
    }
}

template<typename T>
Node<T>* BTree<T>::topLeft(Node<T>* crr)
{
    while(crr->left)
    {
        crr = crr->left;
    }
    return crr;
}

template<typename T>
Node<T>* BTree<T>::topRight(Node<T>* crr)
{
    while(crr->right)
    {
        crr = crr->right;
    }
    return crr;
}

template<typename T>
void BTree<T>::swapData(Node<T>* fst, Node<T>* snd)
{
    T save = fst->data;
    fst->data = snd->data;
    snd->data = save;
}

template<typename T>
bool BTree<T>::containsHelper(Node<T>* crr, T _data)
{
    if(crr->data == _data)
    {
        return true;
    }
    else if(crr->left && crr->right)
    {
        return containsHelper(crr->left, _data) || containsHelper(crr->right, _data);
    }
    else if(crr->left)
    {
        return containsHelper(crr->left, _data);
    }
    else if(crr->right)
    {
        return containsHelper(crr->right, _data);
    }
    else
    {
        return false;
    }
}

template<typename T>
BTree<T>::BTree()
{
    root = nullptr;
    size = 0;
}

template<typename T>
BTree<T>::BTree(const BTree<T>& other)
{
    copy(other);
}

template<typename T>
BTree<T>& BTree<T>::operator=(const BTree<T>& other)
{
    if(this != &other)
    {
        erase();
        copy(other);
    }
    return *this;
}

template<typename T>
BTree<T>::~BTree()
{
    erase();
}

template<typename T>
size_t BTree<T>::getSize()const
{
    return size;
}

template<typename T>
bool BTree<T>::isEmpty()const
{
    return size == 0;
}

template<typename T>
bool BTree<T>::contains(T _data)
{
    return containsHelper(root, _data);
}

template<typename T>
T BTree<T>::head()const
{
    return root->data;
}

template<typename T>
Node<T>* BTree<T>::getRoot()const
{
    return root;
}

template<typename T>
void BTree<T>::insert(T element)
{
    if(size == 0)
    {
        root = new Node<T>(element);
        root->parent = nullptr;
        size++;
        return;
    }
    Node<T>* crr = root;
    Node<T>* prev;
    while(crr)
    {
        prev = crr;
        if(element < crr->data)
        {
            crr = crr->left;
        }
        else
        {
            crr = crr->right;
        }
    }
    if(element < prev->data)
    {
        prev->left = new Node<T>(element);
        prev->left->parent = prev;
    }
    else
    {
        prev->right = new Node<T>(element);
        prev->right->parent = prev;
    }
    size++;
}

template<typename T>
void BTree<T>::removeByData(T _data)
{
    if(size == 0)
    {
        throw "empty tree";
    }
    removeByDataHelper(root, _data);
    size--;
}

template<typename T>
void BTree<T>::removeAllByData(T _data)
{
    while(contains(_data))
    {
        removeByData(_data);
    }
}

template<typename T>
void BTree<T>::clear()
{
    erase();
}

template<typename T>
void BTree<T>::print()
{
    printNode(root);
}

template<typename T>
void BTree<T>::printLeafs()
{
    printLeafNode(root);
}

template<typename T>
void BTree<T>::printRoot()const
{
    if(root)
    {
        cout << root->data;
    }
}

template<typename T>
vector<T> BTree<T>::getDataInVector()
{
    vector<T> vec;
    pushDataInVector(root, vec);
    return vec;
}

template<typename T>
vector<T> BTree<T>::getLeafsInVector()
{
    vector<T> vec;
    pushLeafsInVector(root, vec);
    return vec;
}
