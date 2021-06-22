template<typename T>
GQueue<T>::~GQueue() {
    Flush();

    while (m_DFirst) {
        Node<T>* Nex_Node = m_DFirst->Previous;
        delete m_DFirst;
        m_DFirst = Nex_Node;
    }
}



template<typename T>
Node<T>* GQueue<T>::Get_Node() {
    if (m_DSize == 0) {
        Node<T>* Block = new Node<T>;
        return Block;
    }

    else {
        m_DSize--;

        Node<T>* New_Node = m_DFirst;
        m_DFirst = m_DFirst->Previous;
        New_Node->Previous = nullptr;

        return New_Node;
    }
}

template<typename T>
void GQueue<T>::Insert(Node<T>* Elm) {
    if (m_Size == 0) {
        m_First = Elm;
        m_Last = Elm;
    }

    else {
        m_Last->Previous = Elm;
        m_Last = Elm;
    }

    m_Size++;
}



template<typename T>
void GQueue<T>::Pop() {
    if (m_Size == 0) {
        std::cout << "Called Pop() on empty queue" << std::endl;
    }

    else {
        m_First->Data.~T();

        Node<T>* First = m_First->Previous;
        m_Size--;

        if (m_DSize == 0) {
            m_DFirst = m_First;
            m_DLast = m_First;
        } else {
            m_DLast->Previous = m_First;
            m_DLast = m_First;
        }
        m_DLast->Previous = nullptr;

        m_DSize++;
        m_First = First;
    }
}


template<typename T>
void GQueue<T>::Flush() {
    while (m_First) Pop();
}



template<typename T>
inline T GQueue<T>::Peek_Front() {
    return m_First->Data;
}

template<typename T>
inline T GQueue<T>::Peek_Back() {
    return m_Last->Data;
}



template<typename T>
inline bool GQueue<T>::Empty() {
    return !m_Size;
}

template<typename T>
inline int GQueue<T>::Size() {
    return m_Size;
}

template<typename T>
inline int GQueue<T>::True_Size() {
    return m_Size + m_DSize;
}
