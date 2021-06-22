#pragma once
#include <iostream>
#include <mutex>
#include "Node.h"


template<typename T>
class GTQueue {
public:
    ~GTQueue();

    Node<T>* Get_Node();

    void Insert(Node<T>* Elm);
    void Pop();

    void Flush();

    T Peek_Front();
    T Peek_Back();

    bool Empty();
    int Size();
    int True_Size();
    
private:
    Node<T>* m_First = nullptr;
    Node<T>* m_Last = nullptr;
    int m_Size = 0;

    //Deleted list
    Node<T>* m_DFirst = nullptr;
    Node<T>* m_DLast = nullptr;
    int m_DSize = 0;

    std::mutex m_Mutex;
};

#include "GTQueue.inl"
