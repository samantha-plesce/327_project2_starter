/*
 * functionstocomplete.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: keith
 *
 *      Project 2: Samantha Plesce
 *
 */

//============================================================================
//	TODO add necessary includes here
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "constants.h"
#include "utilities.h"
#include "array_functions.h"
using namespace std;
//============================================================================

//============================================================================
//	stuff you will need
//============================================================================
//TODO define a structure to track words and number of times they occur
struct entry {
	string word;
	int numberOccurences;
};
//TODO add a global array of entry structs (global to this file)
entry myGlobalArray [constants::MAX_WORDS];
//TODO add variable to keep track of next available slot in array
int nextAvailable = 0;
//TODO define all functions in header file
void clearArray(){
	for (int i = 0; i < constants::MAX_WORDS; i++){
		myGlobalArray[i].word = "";
		myGlobalArray[i].numberOccurences = 0;
	}
	nextAvailable = 0;
}

//how many unique words are in array
int getArraySize(){
	return nextAvailable;
}

//get data at a particular location
string getArrayWordAt(int i){
	string word = "";
	if (nextAvailable > i && i >= 0) {
		word = myGlobalArray[i].word;
		return word;
	}
	return constants::TEST_DATA_NON_EXISTANT;
}
int getArrayWord_NumbOccur_At(int i){
	int numOccur = 0;
		if (nextAvailable > i && i >= 0) {
			numOccur = myGlobalArray[i].numberOccurences;
			return numOccur;
		}
	return constants::FAIL_NO_ARRAY_DATA;
}

/*loop through whole file, one line at a time
 * call processLine on each line
 * returns false: myfstream is not open
 *         true: otherwise*/
bool processFile(fstream &myfstream){
	bool status = true;
	if (!myfstream.is_open()){
		status = false;
		return status;
	}
	string line;
	while(!myfstream.eof()){
		getline(myfstream,line);
		processLine(line);
	}
	closeFile(myfstream);
	return status;
}

/*take 1 line and extract all the tokens from it
feed each token to processToken for recording*/
void processLine(string &myString){
	stringstream ss(myString);
	string token = "";
	while(getline(ss, token, constants::CHAR_TO_SEARCH_FOR)){
		processToken(token);
	}
}

/*Keep track of how many times each token seen*/
void processToken(string &token){
	if (strip_unwanted_chars(token)){
		for (int i=0; i < constants::MAX_WORDS; i++){
			string t = token;
			string w = myGlobalArray[i].word;
			toUpper(t);
			toUpper(w);
			if (t == w) {
				myGlobalArray[i].numberOccurences++;
				return;
			}
		}
		myGlobalArray[nextAvailable].word = token;
		myGlobalArray[nextAvailable].numberOccurences++;
		nextAvailable++;
	}
}

/*if you are debugging the file must be in the project parent directory
  in this case Project2 with the .project and .cProject files*/
bool openFile(std::fstream& myfile, const std::string& myFileName,
		std::ios_base::openmode mode){
	myfile.open(myFileName.c_str(), fstream::in);
	if (!myfile.is_open()){
		bool closed = false;
		return closed;
	}
	bool open = true;
	return open;
}

/*iff myfile is open then close it*/
void closeFile(fstream& myfile){
	if(myfile.is_open()){
		myfile.close();
	}
}

/* serializes all content in myEntryArray to file outputfilename
 * returns  FAIL_FILE_DID_NOT_OPEN if cannot open outputfilename
 * FAIL_NO_ARRAY_DATA if there are 0 entries in myEntryArray
 * SUCCESS if all data is written and outputfilename closes OK
 * */
int writeArraytoFile(const string &outputfilename){
	ofstream file;
	file.open(outputfilename.c_str(), ios_base::out);
	if (!file.is_open()){
		return constants::FAIL_FILE_DID_NOT_OPEN;
	}
	if (nextAvailable == 0){
		return constants::FAIL_NO_ARRAY_DATA;
	}
	for (int i = 0; i < getArraySize(); i++){
		file << myGlobalArray[i].word << " " << myGlobalArray[i].numberOccurences << endl;
	}
	file.close();
	return constants::SUCCESS;
}

/*
 * Sort myEntryArray based on so enum value.
 * You must provide a solution that handles alphabetic sorting (A-Z)
 * The presence of the enum implies a switch statement based on its value
 */
void sortArray(constants::sortOrder so){
	switch(so) {
	case constants::NONE:
		break;
	case constants::ASCENDING:
		for(int i = 0; i< getArraySize(); i++) {
				for(int j = i+1; j<getArraySize(); j++) {
					if(myGlobalArray[j].word < myGlobalArray[i].word){
						entry temp = myGlobalArray[i];
						myGlobalArray[i] = myGlobalArray[j];
						myGlobalArray[j] = temp;
					}
				}
		}
		break;
	case constants::DESCENDING:
		for(int i=0; i< getArraySize(); i++){
			for(int j=i+1; j<getArraySize(); j++){
				if(myGlobalArray[i].word <myGlobalArray[j].word) {
						entry temp = myGlobalArray[i];
						myGlobalArray[i] = myGlobalArray[j];
						myGlobalArray[j]=temp;
				}
			}
		}
		break;
	case constants::NUMBER_OCCURRENCES:
		for(int i=0; i< getArraySize(); i++){
			for(int j=i+1; j<getArraySize(); j++){
				if(myGlobalArray[j].numberOccurences< myGlobalArray[i].numberOccurences) {
					entry temp = myGlobalArray[i];
					myGlobalArray[i] = myGlobalArray[j];
					myGlobalArray[j] = temp;
				}
			}
		}
		break;
	}
}
//TODO look in utilities.h for useful functions, particularly strip_unwanted_chars!
