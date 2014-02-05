#include <iomanip>
#include <cmath>
#include <iostream>
using namespace std;

#include "disk_drive.h"
#include "definitions.h"
#include "system_utilities.h"

//diskDrive class functions

//constructor
diskDrive::diskDrive(int a)
{
	//declare input variable
	int input;
    int bitmap_size;
    
	//set input to a
	input = a;
    
	//determine number of blocks
	num_blocks = (input + 63) / 64;
    
    //determine bitmap size
	if (num_blocks > 7)
    {
		bitmap_size = num_blocks / 8;
	}
	else if (num_blocks < 8)
	{
		bitmap_size = 1;
	}

	//allocate for array
	bitmap_ptr = (char*) malloc(bitmap_size * sizeof(char));
	storage_ptr = (char*) malloc(num_blocks * 64);
    
	//initialize array values to 0
	memset(bitmap_ptr, 0, bitmap_size);
    
}

//destructor
diskDrive::~diskDrive()
{
	//free arrays
	free(bitmap_ptr);
	free(storage_ptr);
}

//print
void diskDrive::print()
{
	//declare variable
	int count;

	//set count
	if (num_blocks < 7)
	{
		count = 1;
	}
	else 
	{
		count = num_blocks / 8;
	}

    cout << "The disk drive size is: " << num_blocks * 64 << endl; 
	cout << "The number of blocks is: " << num_blocks << endl;
	cout << "The bit map is: \n" << endl;
	cout << hex;
	for(int i = 0; i < count; i++)
	{
		//declare variables
		int b;

		//set b to first value in bitmap
		b = bitmap_ptr[i];
		b = b & 0xff;

		//print out b
		cout << b << " "; 
	}
    cout << dec << " \n";
}

//allocate block
int diskDrive::allocateBlock(int a)
{
	//declare input variable
	int input, byte, position;
    
	//set input to a
	input = a ;
    
    //determine which byte we are in
    byte = input / 8;
    
    //determine which bit position we are in
    position = input - (byte * 8);
    
	//check to see if valid block and set block to 1
	if (num_blocks - 1 < input || input < 0)
	{
		return BAD_BLOCK_NUMBER;

	}
	else if (0 <= input && input < num_blocks)
	{
		bitmap_ptr[byte] |= (1 << position);
        return 0;
    }
    
	
}

//freeBlock
int diskDrive::freeBlock(int a)
{
	//declare input variable
	int input, position, byte;
    
	//set input to a - 1
	input = a ;
    
    //determine byte
    byte = input / 8;
    
    //determine position
    position = input - (byte * 8);
    
	//check to see if valid block and set block to 0
	if(num_blocks < input || input < 0)
	{
		return BAD_BLOCK_NUMBER;
	}
	else
	{
		bitmap_ptr[byte] &= ~(1 << position);
        return 0;
	}
    
}

//is block free?
int diskDrive::isBlockFree(int a)
{
	//declare input variables
	int input, byte, position;

	//set input to a - 1
	input = a;
    
    //determine byte
    byte = input / 8;
    
    //determine position
    position = input - (byte * 8);

	//check to see if valid block
	if (num_blocks < input || input < 0)
	{
		return BAD_BLOCK_NUMBER;
	}
	else if ((bitmap_ptr[byte] & (1 << position)))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

//storeBlock
int diskDrive::storeBlock(char* d,int blk)
{
	if(blk < 0 || blk > num_blocks)
	{
		return BAD_BLOCK_NUMBER;
	}

	else
	{

		memcpy(storage_ptr + (64 * blk), d, 64);
		return 0;
	}
}

//retrieve block
int diskDrive::retrieveBlock(char* d, int blk)
{
	if(blk < 0 || blk > num_blocks)
	{
		return BAD_BLOCK_NUMBER;
	}
	else
	{
		memcpy(d, storage_ptr + 64 * blk, 64);
		return 0;
	}
}

//format drive
void diskDrive::formatDrive(int num_files)
{
	//declare variables
	int count;
	fileDescriptor* fd = new fileDescriptor();

	//set count
	count = 0;

	for(int i = 0; i < num_blocks; i++)
	{
		freeBlock(i);
	}

	//set the name to 0
	memset(fd->name, 0, 8);

	//set length to 1
	fd->file_length = -1;

	//set first block to 0
	fd->block_number = 0;

	for(int i = 0; i < num_files; i++)
	{
		memcpy(storage_ptr + (16 * i), fd, 16);
		//
	}

	delete fd;

}

//find number of blocks
int diskDrive::findNBlocks(int n, int start)
{
	//declare variable
	int candidate, count;

	candidate = start;
	count = 0;

	while (count < n)
	{
		if(isBlockFree(candidate) == 1)
		{
			count++;
			candidate++;
		}
		else
		{
			count = 0;
			candidate++;
		}
		
	}

	if(candidate < num_blocks)
	{
		return candidate - n;
	}
	else 
	{
		return -1;
	}
		
}