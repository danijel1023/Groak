#pragma once

template<typename T>
struct Node {
    T Data;
    Node<T>* Previous = nullptr;
};
