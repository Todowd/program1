#include<iostream>
using std::cout;
using std::endl;

#include<string>
using std::string;

#include<fstream>
using std::fstream;

#include<cctype>

#include "myList.h"
#include "timer.h"

string clean(string str);
void put(myList<string>& dict, string str);

int main() {
    //initializing variables
    myList<string> dict;
    fstream file;
    Timer timer;
    char c;
    string tmp;

    double ttc; //Time To Check
    int tw=0;     //Total Words
    float tnc=0;    //Total Number of Compares

    int wsc=0;    //Words Spelled Correctly
    float tccw=0;   //Total Compares Correct Words

    int wsw=0;    //Words Spelled Wrong
    float tcww=0;   //Total Compares Wrong words

    int ws=0;     //words skipped

    //average compares=compares/words;

    //start loading the dictionary
    //just assume file is there, or ask user for file name/location
    file.open("dict.txt");
    int i=0;
    while(!file.eof()) {
        file>>tmp;
        tmp=clean(tmp);
        //insert(dict, tmp);//used for an unordered list
        put(dict, tmp);
        i++;
        if(i%10000==0) {
            cout<<i<<endl;
        }
    }
    cout<<"Dictionary sort complete size: "<<dict.getSize();
    file.close();

    //START TIMING
    timer.Start();
    file.open("book.txt");
    //go through the book word by word
    while(!file.eof()) {
        //add one to total number of words
        tw++;
        file>>tmp;
        tmp=clean(tmp);
        //if the word is empty, skip it
        if(tmp.length()==0) {
            //add 1 to words skipped
            ws++;
            continue;
        }
        //check if word starts with a number, if so skip it
        if((isdigit(tmp[0])!=0)||(tmp[0]=='0')) {
            //add one to the words skipped counter
            ws++;
            continue;
        }
        int counter=0;
        //Word is in the dictionary
        if(dict.find(tmp, counter)) {
            //add the amount of compares to the total compares for correct words
            tccw+=counter;
            //add one to the total correct words counter
            wsc++;
        }
        //Word is not in dictionary
        else {
            //add the amount of compares to the total compares for wrong words
            tcww+=counter;
            //add one to the total words spelled wrong counter
            wsw++;
        }
        //add the amount of compares to the total number of compares counter
        tnc+=counter;
        //reset tmp counter
        counter=0;
        if(tw%10000==0) {
            cout<<tw<<endl;
        }
    }

    //for thread use var outside to keep track of how many total
    //in the thread compare out to half as theyre being loaded int
    //start second one after done being loaded in

    //or
    //pull X amount of words out, give to a thread to check
    //once all threads are done THEN add numbers together to prevent double access

    //STOP TIMING
    timer.Stop();
    ttc=timer.Time();
    //print and handle outputs
    cout<<"\n\nTotal Time of Compares: "<<ttc<<endl;
    cout<<"Total Words: "<<tw<<endl;
    cout<<"Total Number of Compares: "<<tnc<<"\n"<<endl;
    cout<<"Words Spelled Correctly: "<<wsc<<endl;
    cout<<"Total compares of Correct Words: "<<tccw<<"\n"<<endl;
    cout<<"Words Spelled Wrong: "<<wsw<<endl;
    cout<<"Total Compares for Wrong Words: "<<tcww<<endl;
}

//used to order the list
void put(myList<string>& dict, string str) {
    node<string>* n=dict.getHead();
    node<string>* nprev=nullptr;
    node<string>* item=new node<string>(str);
    //if the list is empty, add it as the first item
    if(n==nullptr) {
        dict.insert(str);
        return;
    }
    //if the first item in the list is longer than the item
    //put it in the list
    if(str.length()<(*n).data.length()) {
        dict.insert(str);
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
    dict.upSize();
    return;
}

string clean(string str) {
    //check each char of the string
    for(int i=0; i<str.length(); i++) {
        char c=str[i];
        //if its an alphabetical char
        if(isalpha(c)) {
            //make it lower
            str[i]=tolower(c);
            continue;
        }
        //if its a number, move on
        else if(isdigit(c)) {
            continue;
        }
        //if its an apostrophe move on
        else if(c=='\'') {
            continue;
        }
        //otherwise erase it from the string
        else {
            str=str.erase(i, 1);
            i--;
        }
    }
    return str;
}
