#include<iostream>
using std::cout;
using std::endl;
//used for output

#include<string>
using std::string;
//used for the word

#include<fstream>
using std::fstream;
using std::ofstream;
//used to read the dict/book and write out the misspelled words

#include<future>
using std::future;
using std::async;
//used for multithreaded searching

#include<mutex>
using std::mutex;
//used to prevent multiple strings from accessing (modifying) a var

#include<vector>
using std::vector;
//used to hold the future vars of the threads

#include<cctype>
//used for character comparisons

#include "myList.h"
#include "timer.h"

//prevent threads from accessing something at the same time
//any time locker.lock() and locker.unlock() is seen
mutex locker;
//holds future objects
static vector<future<void>> threads;
//dictionary list of words
static myList<string> dict;
//list of words spelled wrong
static myList<string> misspelled;
//used to break the look, while waiting for threads to finish
static unsigned int threadsDone=0;

const int CHUNK=10000;

float tnc=0;        //Total Number of Compares
float tccw=0;       //Total Compares Correct Words
float tcww=0;       //Total Compares Wrong words
int wsc=0;          //Words Spelled Correctly
int wsw=0;          //Words Spelled Wrong

//array of occurances of words correct encoded by ascii chars
static int oc[99999999]={0}; //4 char long, initialized all to zero

//array of occurances of words misspelled encoded by ascii chars
static int ow[99999999]={0}; //4 char long, initialized all to zero

//cleans up the word
string clean(string str);

//searches for the word in the dictionary
static void search(myList<string>* list, int id);

//converts a string to a ascii encoded number
static int convert(string word);

int main() {
    //initializing variables
    fstream file;   //input file stream
    ofstream ofile; //output file stream
    Timer timer;    //timer var
    string word;    //word string
    int tw=0;       //Total Words
    int ws=0;       //words skipped
    double ttc;     //Time To Check

    //start loading the dictionary
    //just assume file is there, or ask user for file name/location
    file.open("dict.txt");
int i=0;
    while(!file.eof()) {
        //word by word pulled from the file
        file>>word;
        //clean the word
        word=clean(word);
        //insert(dict, word);//used for an unordered list
        //add it to the dictionary list, which will auto sort it
        dict.put(word);
if(i%10000==0) {
cout<<i<<endl;
}
i++;
    }
cout<<"Dictionary loaded"<<endl;
    //close the file
    file.close();
    //START TIMING
    timer.Start();
    //open the book
    file.open("book.txt");
    //go through the book word by word
//myList<string>* list=new myList<string>;//remove when threads
    for(int i=0; !file.eof(); i++) {
        //tmp list to hand off to a thread
        myList<string>* list=new myList<string>;
        //collect a portion of the entire book to add to the list
        for(int ii=0; ((ii<CHUNK)&&(!file.eof())); ii++) {
            //get the word from the file
            file>>word;
            //add one to the total word count
            tw++;
            //clean the word
            word=clean(word);
            //check if word is empty, if so skip it
            if((word.length()==0)) {
                //keeps thread chunks to their size
                ii--;
                //skip to the next word
                continue;
            }
            //if the word starts with something non alphabetical skip it
            if(isalpha(word[0])==0) {
                //add 1 to words skipped
                ws++;
                //keeps thread chunks to their size
                ii--;
                //skip to the next word
                continue;
            }
            //prevent the eof char from being added to the list
            if(!file.eof()) {
                (*list).insert(word);
            }
        }
        //create and start the thread given a list, and the funciton search
        threads.push_back(async(std::launch::async, search, list, i));
    }
//search(list, 0);
    //wait for the amount of completed threads to equal the amount of threads
    //that exist
    while(threadsDone<threads.size()) {
        //do nothing, but wait
    }

    //STOP TIMING
    timer.Stop();
    //collect the total time it took
    ttc=timer.Time();
    //write mispelled words to file
    ofile.open("misspelled.txt");
    //just dumping from list
    while(misspelled.getSize()>0) {
        //add word and new line
        ofile<<misspelled.front()<<"\n";
        //remove word from list
        misspelled.remove();
    }
    //close the file
    ofile.close();

    //print and handle outputs
    cout<<"dictionary size "<<dict.getSize()<<endl
        <<"Done Checking and these are the results"<<endl
        <<"finished in time: "<<ttc<<endl
        <<"There are "<<wsc<<" words found in the dictionary "
        <<tccw<<" compares. Average: "<<(tccw/static_cast<float>(wsc))<<endl
        <<"There are "<<wsw<<" words NOT found in the dictionary "
        <<tcww<<" compares. Average: "<<(tcww/static_cast<float>(wsw))<<endl
        <<"There are "<<ws<<" words not checked."<<endl;
}

//Search function each thread calls
static void search(myList<string>* list, int id) {
int i=0;
    string word;
    //go through the list
    while((*list).getSize()>0) {
        //grab the word from the list
        word=(*list).front();
        //word could have already been found
        if(word.length()<=4) {
            //get the encoded value
            int asciiword=convert(word);
            //check if its been found already in the book
            if(oc[asciiword]>=0) {
                locker.lock();
                oc[asciiword]++;
                wsc++;
                (*list).remove();
                locker.unlock();
                //found it, no need to check anything else with this word
                continue;
            }
            //check if its been misspelled
            else if(ow[asciiword]>=0) {
                locker.lock();
                ow[asciiword]++;
                misspelled.insert(word);
                wsw++;
                (*list).remove();
                locker.unlock();
                //found it, no need to check anything else with this word
                continue;
            }
        }
        //counter of compares
        float counter=0;
        //find returns true if its there, and counter is passed by reference
        //so the counter receives total compares
        if(dict.find(word, counter)) {
            //stop other threads from access anything until unlocked
            locker.lock();
            if(word.length()<=4) {
                oc[convert(word)]++;
            }
            //add the amount of compares to the total compares for correct words
            tccw+=counter;
            //add one to the total correct words counter
            wsc=wsc+1;
        }
        //Word is not in dictionary
        else {
            //stop other threads from access anything until unlocked
            locker.lock();
            if(word.length()<=4) {
                oc[convert(word)]++;
            }
            //add the amount of compares to the total compares for wrong words
            tcww+=counter;
            //add one to the total words spelled wrong counter
            wsw=wsw+1;
            //add the misspelled word to the list of misspelled words (notFound)
            misspelled.insert(word);
        }
        //add the amount of compares to the total number of compares counter
        tnc+=counter;
        //unlock threads from accessing
        locker.unlock();
        //reset tmp counter
        counter=0;
        //remove the word from the list
        (*list).remove();

    }
    //once complete, this thread is done, add one to the thread done counter
    locker.lock();
    threadsDone++;
    locker.unlock();
i++;
if(i%100000==0) {
cout<<i<<endl;
}
}

//Clean function cleans the string
string clean(string str) {
    //check each char of the string
    for(unsigned int i=0; i<str.length(); i++) {
        //pull the i'th char
        char c=str[i];
        //if its an alphabetical char
        if(isalpha(c)) {
            //make it lower
            str[i]=tolower(c);
            //no need to go through the rest of this iteration
            //since we know what it is
            continue;
        }
        //if its a number, move on
        else if(isdigit(c)) {
            //no need to go through the rest of this iteration
            //since we know what it is
            continue;
        }
        //if its an apostrophe move on
        else if(c=='\'') {
            //no need to go through the rest of this iteration
            //since we know what it is
            continue;
        }
        //otherwise erase it from the string
        else {
            //remove the unknown char from the string
            str=str.erase(i, 1);
            //decrement the counter to account for the removed char
            i--;
        }
    }
    //return the cleaned string
    return str;
}

static int convert(string word) {
    unsigned int val=0;
    for(unsigned int i=0; i<word.length(); i++) {
        unsigned int tmpval;
        char c=word[i];
        tmpval=(int)c;
        for(unsigned int ii=0; ii<i; ii++) {
            tmpval*=100;
        }
        val+=tmpval;
    }
    switch(word.length()) {
        case 1:
            val*=1000000;//move three chars over
            break;
        case 2:
            val*=10000;//move two chars over
            break;
        case 3:
            val*=100;//move one char over
        default:
            //4 chars long, ignore
            break;
    }
    return val;
}
