#ifndef MACHINES_H
#define MACHINES_H

#include "disk_drive.h"
#include "definitions.h"

//computer class
class Computer
{
protected:
    diskDrive* ptr;
    char name[20];
	int maximum_number_of_files;
	int findFile(char* fname, fileDescriptor* fd);
	void printOneFile(fileDescriptor fd);
    
public:
    int isThisMyName(char* n);
    virtual void print();
    Computer(char* a, int size);
	int findFreeFileDescriptor();
	void getFileDescriptor(int n, fileDescriptor* fd);
	void putFileDescriptor(int n, fileDescriptor* fd);
	int createFile(char* n, int len, char* ff);
	void printDirectory();
	void printFiles(int argc, char* argv[]);
	//~Computer();
};

//PC class
class PC : public Computer
{
private:
    char pc_owner[32];
    
public:
    PC(char* name, int num_bytes, char* owner_name);
    //~PC();
    virtual void print();
};


//printer class
class printer : public Computer
{
private:
    int num;
    
public:
    printer(char* name, int num_bytes, int num_inp);
    //~printer();
    virtual void print();
};

//server class
class server : public Computer
{
private:
    char location[16];
    
public:
    server(char* name, int num_bytes, char* location_inp);
    //~server();
    virtual void print();
    
};

 
#endif
