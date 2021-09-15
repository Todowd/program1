#ifndef MYLIST_H
#define MYLIST_H

#include<iostream>
using std::cout;
using std::endl;
#include "node.h"

template<class T>
class myList {
    public:
        //constructor
        myList() {
            head=nullptr;
            size=0;
        };
        //constructor where head starts with node n
        myList(node<T>* n) {
            head=n;
            //count how many nodes are already there
            while(n!=nullptr) {
                size++;
                n=(*n).next;
            }
        }
        //adds items to front of list
        void insert(T item) {
            node<T>* tmpPtr=new node<T>(item);
            (*tmpPtr).next=head;
            head=tmpPtr;
            size++;
            return;
        };
        //gives the head node
        node<T>* getHead() {
            return head;
        }
        //removes the item at the front
        void remove() {
            if(head!=nullptr) {
                head=(*head).next;
                size--;
            }
            return;
        };
        //gives the data of the first item in the list
        T front() {
            return (*head).data;
        };
        //prints the list from front to last, or says its empty if its empty
        void print() {
            if(head==nullptr) {
                cout<<"myList is Empty";
                return;
            }
            node<T>* n=head;
            while((*n).next!=nullptr) {
                cout<<(*n).data<<", ";
                n=(*n).next;
            }
            cout<<(*n).data<<"\n";
            return;
        };
        //returns true if an item is in the list
        bool find(T item, int& compares) {
            node<T>* n=head;
            while(n!=nullptr) {
                if(item.compare((*n).data)==0) {
                //if((*n).data==item) {//left here for later code use
                    return true;
                }
                compares++;
                n=(*n).next;
            }
            return false;
        };
        //recursive find. Returns true if an item is in the list

        bool find(node<T>* n, T item, int& compares){
            if((*n).next==nullptr) {
                return false;
            }
            //if(item==(*n).data) {
            if(item.compare((*n).data)==0) {
                compares++;
                return true;
            }
            compares++;
            return find((*n).next, item, compares);
        };
        //returns true if the list is empty
        bool isEmpty() {
            return size==0;
        }
        //returns the size of the list
        int getSize() {
            return size;
        };
        //increases size by 1
        void upSize() {
            size++;
        };
    private:
        node<T>* head;
        int size;
};
#endif
