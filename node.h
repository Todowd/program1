#ifndef NODE_H
#define NODE_H

//node class for list
template<class T> struct node {
    T data;
    node<T>* next;
    node() {next=nullptr;}
    node(T item) {
        data=item;
        next=nullptr;
    }
};
#endif
