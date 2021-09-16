#ifndef MYLIST_H
#define MYLIST_H

#include<iostream>
using std::cout;
using std::endl;
#include "node.h"

#include<string>
using std::string;

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
        //used to order the list from shortest to longest
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
            //increase the size
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
            //print a nice empty msg
            if(head==nullptr) {
                cout<<"myList is Empty";
                return;
            }
            //go through each node and print it followed by a comma until the end
            node<T>* n=head;
            while((*n).next!=nullptr) {
                cout<<(*n).data<<", ";
                n=(*n).next;
            }
            //add a newline at the end
            cout<<(*n).data<<"\n";
            return;
        };
        //returns true if an item is in the list
        bool find(T item, float& compares) {
            //use compares passed by reference to add accountance for total compares
            node<T>* n=head;
            //go through each node, by node and check if the string is there
            while(n!=nullptr) {
                //a node contains the item being looked for
                if(item.compare((*n).data)==0) {
                    //add one to the compares, returns true
                    compares=compares+1;
                    //return true
                    return true;
                }
                //add one to the compares, did not return true
                compares=compares+1;
                n=(*n).next;
            }
            return false;
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
};
#endif
