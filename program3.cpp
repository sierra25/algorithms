#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<iostream>
#include<string>
#include <algorithm>
#include<bits/stdc++.h>
#include <queue>
#include<iomanip>
#include<climits>
#include <fstream>
#include <cmath>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <iterator>
#include <set>

using namespace std;

int numberOfFramesAvailable; // will vary number of frames available from 1 to 30 using a for loop
int numberOfHits = 0; // is the number of hits that occur using the algorithm
int numberOfMisses = 0; //is the number of misses that occur using the algorithm. will set this equal to the number of page faults at the end
//set<int> s;// is the set data a structure used to represent the current pages. will use to performAnalysis if a page element is present already
int run = 1; // will increment this as a counter for each run . will have 30 runs per each algorithm call.
int pageReferenceString[100]; // the array will hold all of the randomly generated page elements

////////////////////FIFO
int FIFO(int pageReferenceString[], int numberOfFramesAvailable) {
  set < int > comparisonSet;
  queue < int > queueForStoredPages;

  int numPageFaults = 0;

  for (int i = 0; i < 100; i++) {

    if (comparisonSet.size() < numberOfFramesAvailable) //sees if there is room in the set. This is when the set is just being filled
    {

      if (comparisonSet.find(pageReferenceString[i]) == comparisonSet.end()) {

        comparisonSet.insert(pageReferenceString[i]); // places the current page into the set

        numPageFaults++; // increments the number of page faults

        queueForStoredPages.push(pageReferenceString[i]);
      }
    }

    // After the frames are full, the algorithm will start using FIFO to get rid of pages
    else {

      if (comparisonSet.find(pageReferenceString[i]) == comparisonSet.end()) {

        int element = queueForStoredPages.front();

        queueForStoredPages.pop();
        comparisonSet.erase(element);

        comparisonSet.insert(pageReferenceString[i]);
        queueForStoredPages.push(pageReferenceString[i]);

        numPageFaults++; // increments the number of page faults
      }
    }
  }

  return numPageFaults;
}

////////////////////LRU

class page {
  public:

    int pageReferenceString[100];
  int numberOfPages;
  int numberOfFrames;
  int numPageFaults = 0;
  int position = 0;
  int counter = 1;
  int frequencyOfNeedArray[100];
  int frames[100];
  int location = 0;
  int location2 = 0;
  page(int pageReferenceString[], int a, int b) {
    numberOfPages = a;
    numberOfFrames = b;
    numPageFaults = numberOfFrames;

    for (int i = 0; i < numberOfFrames; i++) {
      frames[i] = -1;
      frequencyOfNeedArray[i] = 0;
    }
  }

  void fillFramesInitially(int pageReferenceString[]) {
    for (int i = 0; i < numberOfFrames; i++) {
      frames[i] = pageReferenceString[i];
      for (int j = 0; j <= i; j++) {
        frequencyOfNeedArray[j] += 1;
      }

    }
  }
  bool performAnalysis(int i1) {
    for (int i = 0; i < numberOfFrames; i++) {
      if (frames[i] == pageReferenceString[i1]) {

        location2 = i;
        return true;
      }
    }
    return false;
  }
  void removeElement(int i1) {

    frequencyOfNeedArray[i1] = 0;

  }
  int greatest() {
    int mostOften = frequencyOfNeedArray[0];
    for (int i = 0; i < numberOfFrames; i++) {
      if (mostOften <= frequencyOfNeedArray[i]) {
        mostOften = frequencyOfNeedArray[i];
        location = i;
      }
    }

    return location;
  }

  int lru() {
    for (int i = numberOfFrames; i < numberOfPages; i++) {

      if (performAnalysis(i) == true) {

        removeElement(location2);

        for (int j = 0; j < numberOfFrames; j++) {
          if (location2 == j) {
            continue;
          }
          frequencyOfNeedArray[j] += 1;
        }

        continue;

      } else {
        position = greatest();

        frames[position] = pageReferenceString[i];
        removeElement(position);
        for (int j = 0; j < numberOfFrames; j++) {
          if (position == j) {
            continue;
          }
          frequencyOfNeedArray[j] += 1;
        }
        numPageFaults++;

      }

    }

    return numPageFaults;
  }

};

////////////////////OPT

bool search(int lookFor, vector < int > & frameArray) {
  for (int i = 0; i < frameArray.size(); i++)
    if (frameArray[i] == lookFor)
      return true;
  return false;
}

//looks into the future to decide which page element will not be used soon
int futurePrediction(int pageReferenceString[], vector < int > & frameArray, int pageElements, int location) {

  int result = -1;
  int farthestInTheFuture = location;
  for (int i = 0; i < frameArray.size(); i++) {
    int j;
    for (j = location; j < pageElements; j++) {
      if (frameArray[i] == pageReferenceString[j]) {
        if (j > farthestInTheFuture) {
          farthestInTheFuture = j;
          result = i;
        }
        break;
      }
    }

    if (j == pageElements)
      return i;
  }

  if (result == -1) {
    return 0;
  } else {
    return result;
  }

}

int OPT(int pageReferenceString[], int pageElements, int numberOfFramesAvailable) {
  vector < int > frameArray;

  int hit = 0;
  for (int i = 0; i < pageElements; i++) { // moves through the page reference string array and performs analysis. It counts the number of misses and hits.

    if (search(pageReferenceString[i], frameArray)) {
      hit++;
      continue;
    }

    if (frameArray.size() < numberOfFramesAvailable)
      frameArray.push_back(pageReferenceString[i]);

    else {
      int j = futurePrediction(pageReferenceString, frameArray, pageElements, i + 1);
      frameArray[j] = pageReferenceString[i];
    }
  }
  //outputs the number of hits and misses
  cout << "Number of hits: " << hit << endl;
  cout << "Number of misses: " << pageElements - hit << endl;
}

int main() {

  ofstream myfile;
      myfile.open ("myexcel.csv");


  int pageElements = 100;

  /* EARLY TESTING

  int selection;

  // outputting menu
  cout <<"Virtual-Memory Page Replacement Algorithm Options: "<<endl;
  cout<<"1. First-In, First-Out (FIFO)"<<endl;
  cout<<"2. Least Recently Used (LRU)"<<endl;
  cout<<"3. Optimal (OPT)"<<endl;
  cout<<"4. Exit Program"<<endl;
  cout << "Select a Virtual-Memory Page Replacement Algorithm Option from the list(1-4): "<<endl;
  cin >> selection; //user selects what algorithm to perform
  cout << endl;
  */
  /*CODE FOR INPUT

  //allowing the number of page frames to be passed in as a n argument
  //getting the number of page frames that are available
  cout<<"Enter the number of page frames available: "<<endl;
  cin>>numberOfFramesAvailable;

  //cout<<numberOfFramesAvailable<<endl;

  */

  //creates a randomly generated page reference string that has numbers ranging from 0-49
  srand(time(NULL));

  cout << "Page Reference String: ";

  for (int i = 0; i < 100; i++) {
    int number; // a variable that will hold the randomly generated number
    number = rand() % 50;

    cout << number << " ";

    pageReferenceString[i] = number; //placing each of the randomly generated string/numbers as elements in the array pageReferenceString

  }

  cout << endl;

//creating and formatting excel sheet
  myfile << "Number Of Frames Available,FIFO,LRU,OPT\n";

  int counter2 = 1;

  for (int i = 0; i < 30; i++) {


      myfile << counter2;
      myfile<< "\n";
      counter2++;

  }


  numberOfFramesAvailable = 1;
  myfile<< ",";

  for (int i = 0; i < 30; i++) {

    cout << "FIFO" << endl;
    cout << "Calculations: " << endl;
    cout << "The number of page faults for run #" << run << " with " << numberOfFramesAvailable << " physical-memory frames available" << ": " << FIFO(pageReferenceString, numberOfFramesAvailable) << endl;

    cout << "----------------------------------------------------------------------" << endl;

      myfile<< FIFO(pageReferenceString, numberOfFramesAvailable);
      myfile<< "\n";


   myfile<< ",";

    numberOfFramesAvailable++;
    run++;
  }

  myfile<< ",";
  numberOfFramesAvailable = 1;

  for (int i = 0; i < 30; i++) {

    cout << "LRU" << endl;
    cout << "Calculations: " << endl;
    page myPages(pageReferenceString, pageElements, numberOfFramesAvailable);
    myPages.fillFramesInitially(pageReferenceString);
    int myLRUFunctionOutput = myPages.lru();
    cout << "The number of page faults for run #" << run << " with " << numberOfFramesAvailable << " physical-memory frames available" << ": " << myLRUFunctionOutput << endl;

    cout << "----------------------------------------------------------------------" << endl;
     myfile<< myLRUFunctionOutput;
     myfile<< "\n";

    numberOfFramesAvailable++;
    run++;
    myfile<< ",,";
  }

  myfile<< ",";
  numberOfFramesAvailable = 1;
  for (int i = 0; i < 30; i++) {

    cout << "OPT" << endl;
    cout << "Calculations: " << endl;
    int optimalFunctionResults = OPT(pageReferenceString, pageElements, numberOfFramesAvailable);
    cout << "The number of page faults for run #" << run << " with " << numberOfFramesAvailable << " physical-memory frames available" << ": " << optimalFunctionResults << endl;
    cout << "----------------------------------------------------------------------" << endl;
    myfile<< optimalFunctionResults;
    myfile<< "\n";




    numberOfFramesAvailable++;
    run++;
    myfile<< ",,,";
  }


   myfile.close();
  return 0;

}
