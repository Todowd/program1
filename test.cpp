#include<iostream>
using std::cout;
using std::endl;

#include<string>
using std::string;

#include<fstream>
using std::fstream;
using std::ofstream;

#include<cctype>

#include "myList.h"
#include "timer.h"

string clean(string str);
void put(myList<string>& dict, string str);

int main() {
    //initializing variables
    myList<string> dict;
    myList<string> notFound;
    myList<string> skipped;
    fstream file;
    ofstream ofile;
    Timer timer;
    string tmp;

    double ttc; //Time To Check
    float tw=0;     //Total Words
    float tnc=0;    //Total Number of Compares

    float wsc=0;    //Words Spelled Correctly
    float tccw=0;   //Total Compares Correct Words

    float wsw=0;    //Words Spelled Wrong
    float tcww=0;   //Total Compares Wrong words

    float ws=0;     //words skipped

    //average compares=compares/words;

    //start loading the dictionary
    //just assume file is there, or ask user for file name/location
    file.open("dict.txt");
int i=0;
    while(!file.eof()) {
        file>>tmp;
        tmp=clean(tmp);
        //insert(dict, tmp);//used for an unordered list
        dict.put(tmp);
        i=i+1;
if(i%10000==0) {
    cout<<i<<endl;
}
    }
cout<<"done sorting"<<endl;
    file.close();

    //START TIMING
    timer.Start();
    file.open("book.txt");
    //go through the book word by word
i=0;
    while(!file.eof()) {
        //add one to total number of words
        tw=tw+1;
        file>>tmp;
        tmp=clean(tmp);
        //if the word is empty, skip it
        if(tmp.length()==0) {
            //add 1 to words skipped
            ws=ws+1;
skipped.insert(tmp);
            continue;
        }
        //check if word starts with a number, if so skip it
        if((isdigit(tmp[0])!=0)||(tmp[0]=='0')) {
            //add one to the words skipped counter
            ws=ws+1;
skipped.insert(tmp);
            continue;
        }
        int counter=0;
        //using recursion
        if(tmp.length()<5) {
            //Word is in the dictionary
            //if(dict.findRR(tmp, counter, 4, 10000)) {
            if(dict.findR(tmp, counter, 5)) {
                //add the amount of compares to the total compares for correct words
                tccw+=counter;
                //add one to the total correct words counter
                wsc=wsc+1;
            }
            //Word is not in dictionary
            else {
                //add the amount of compares to the total compares for wrong words
                tcww+=counter;
                //add one to the total words spelled wrong counter
                wsw=wsw+1;
                //add word to mispelled list
                notFound.insert(tmp);
            }
        }
        //Word is in the dictionary
        else if(dict.find(tmp, counter)) {
            //add the amount of compares to the total compares for correct words
            tccw+=counter;
            //add one to the total correct words counter
            wsc=wsc+1;
        }
        //Word is not in dictionary
        else {
            //add the amount of compares to the total compares for wrong words
            tcww+=counter;
            //add one to the total words spelled wrong counter
            wsw=wsw+1;
            //
            notFound.insert(tmp);
        }
        //add the amount of compares to the total number of compares counter
        tnc+=counter;
        //reset tmp counter
        counter=0;
if(i%15000==0) {
cout<<tw<<endl;
}
i++;
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
    //write mispelled words to file
    ofile.open("misspelled.txt");
    while(notFound.getSize()>0) {
        ofile<<notFound.front()<<"\n";
        notFound.remove();
    }
    ofile.close();

    //print and handle outputs
    cout<<"dictionary size "<<dict.getSize()<<endl
        <<"Done Checking and these are the results"<<endl
        <<"finished in time: "<<ttc<<endl
        <<"There are "<<wsc<<" words found in the dictionary "
        <<tccw<<" compares. Average: "<<(tccw/wsc)<<endl
        <<"There are "<<wsw<<" words NOT found in the dictionary "
        <<tcww<<" compares. Average: "<<(tcww/wsw)<<endl
        <<"There are "<<ws<<" words not checked."<<endl;

    //my outputs:
    /*
    cout<<"\n\nTotal Time of Compares: "<<ttc<<endl;
    cout<<"Total Words: "<<tw<<endl;
    cout<<"Total Number of Compares: "<<tnc<<"\n"<<endl;
    cout<<"Words Spelled Correctly: "<<wsc<<endl;
    cout<<"Total compares of Correct Words: "<<tccw<<"\n"<<endl;
    cout<<"Words Spelled Wrong: "<<wsw<<endl;
    cout<<"Total Compares for Wrong Words: "<<tcww<<endl;
    */
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
