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

float tnc=0;        //Total Number of Compares
float tccw=0;       //Total Compares Correct Words
float tcww=0;       //Total Compares Wrong words
int wsc=0;          //Words Spelled Correctly
int wsw=0;          //Words Spelled Wrong

//chunk size for the threads
const int CHUNK=50000;

string clean(string str);
//void put(myList<string>& dict, string str);

static void search(myList<string>* list, int id);

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
    while(!file.eof()) {
        //word by word pulled from the file
        file>>word;
        //clean the word
        word=clean(word);
        //insert(dict, word);//used for an unordered list
        //add it to the dictionary list, which will auto sort it
        dict.put(word);
    }
    //close the file
    file.close();

    //START TIMING
    timer.Start();
    //open the book
    file.open("book.txt");
    //go through the book word by word
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
                ii--;
                continue;
            }
            //if the word starts with something non alphabetical skip it
            if(isalpha(word[0])==0) {
                //add 1 to words skipped
                ws++;
                ii--;
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
    //wait for the amount of completed threads to equal the amount of threads
    //that exist
    while(threadsDone<threads.size()) {
        //do nothing, but wait
    }

    //add array of ints, and use the address as the ascii correllation
    //to an int, if the int is 0, then it hasnt been seen yes,
    //and must be looked for in the dictionary, otherwise it has,
    //add one to that count and move onto the next word, no compares needed.
    //This would require a funciton that converts the string to the ascii
    //numbers. Not just individual characters, but a encoding of the entire word

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
    string word;
    //go through the list
    while((*list).getSize()>0) {
        //grab the word from the list
        word=(*list).front();
        //counter of compares
        float counter=0;
        //find returns true if its there, and counter is passed by reference
        //so the counter receives total compares
        if(dict.find(word, counter)) {
            //stop other threads from access anything until unlocked
            locker.lock();
            //add the amount of compares to the total compares for correct words
            tccw+=counter;
            //add one to the total correct words counter
            wsc=wsc+1;
        }
        //Word is not in dictionary
        else {
            //stop other threads from access anything until unlocked
            locker.lock();
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
