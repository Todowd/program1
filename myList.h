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
            size=-1;
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
        //used to order the list
        void put(string str) {
            node<string>* n=head;
            node<string>* nprev=nullptr;
            node<string>* item=new node<string>(str);
            //if the list is empty, add it as the first item
            if(n==nullptr) {
                insert(str);
                return;
            }
            //if the first item in the list is longer than the item
            //put it in the list
            if(str.length()<(*n).data.length()) {
                insert(str);
                return;
            }
            //finds the item that is shorter in length than the word
            //to put into the list, then puts the item just after it
            while(str.length()>=(*n).data.length()) {
                nprev=n;
                n=(*n).next;
                //found end of list, add it there
                if(n==nullptr) {
                    break;
                }
            }
            //if its not the end of the list, put the trailing nodes
            //onto the item's node, acting as a psuedo head node
            if(n!=nullptr) {
                (*item).next=n;
            }
            //add the back side of the list of nodes onto this node
            (*nprev).next=item;
            size++;
            return;
        };
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
                compares=compares+1;
                n=(*n).next;
            }
            return false;
        };
        //recursive find. Returns true if an item is in the list
        bool findR(T item, int& compares, int length) {
            return find(head, item, compares, length);
        };
        //returns true if the list is empty
        bool isEmpty() {
            return size==0;
        }
        //returns the size of the list
        int getSize() {
            return size;
        };
    private:
        node<T>* head;
        int size;
        //private side of the recursive find
        bool find(node<T>* n, T item, int& compares, int length) {
            if((*n).next==nullptr) {
                return false;
            }
            else if((*((*n).next)).data.length()>length) {
                return false;
            }
            if(item.compare((*n).data)==0) {
                compares=compares+1;
                return true;
            }
            compares=compares+1;
            return find(((*n).next), item, compares, length);
        }
};
#endif
