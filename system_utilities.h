#ifndef SYSTEM_UTILITIES_H
#define SYSTEM_UTILITIES_H

#include <iostream>

//declare system_status
void systemStatus();

//declare add network node
int addNetworkNode(char* toks[], int numberToks);

//declare delete network node
int deleteNetworkNode (char* toks[], int numberToks);

//declare string converter
int str_convert(char* a);

//declare find Node
int findNode(char* n);

//declare parse command line
int parseCommandLine(char line[], char* tokens[]);

//declare fillCommandList
void fillCommandList();

//declare get command number
int getCommandNumber(char* s);

//declare wait function
void wait();

//declare print error function
void printError(int err);

//function to select computer from network array and create a file on that computer
int selectCompAndCreateFile(int pos, char* name, int length, char* ff);

//function to select computer from network array and prints its file descriptors
void selectCompAndListDescriptors(int pos);

//select computer and print files
void selectCompAndPrintFiles(int pos, int argc, char** argv);

#endif
