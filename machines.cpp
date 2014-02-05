
#include "machines.h"
#include "definitions.h"
#include <iomanip>
#include <iostream>
using namespace std;


//computer constructor
Computer::Computer(char* a, int size)
{
	//declare num_bytes
	int disk_size;

    //set name to 0
    memset(name, 0, 20);
    
    //copy input to name
    memcpy(name, a, 20);
    
    //set num_bytes to size
    disk_size = size;

	//check for valid size and create disk drive for computer with input value
    if( disk_size > 127)
    {
        ptr = new diskDrive(disk_size);
    }
    else
    {
        ptr = new diskDrive(128);
    }
};

//computer print function
void Computer::print()
{
    cout << "The name of this computer is: " << endl;
    for(int i = 0; i < strlen(name); i++)
    {
        cout << name[i];
    }
    
    cout << "\n\n";
    
    //print disk drive information
    (*ptr).print();
    
    cout << "\n";
}

//is this my name function
int Computer::isThisMyName(char* n)
{
    if(strcmp(name, n) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//PC constructor
PC::PC(char* name, int num_bytes, char* owner_name):Computer(name, num_bytes)
{
    //create new computer
    new Computer(name, num_bytes);

	maximum_number_of_files = 8;
    
    //set pc_name to 0
    memset(pc_owner, 0, 32);
    
    //copy owner name to pc_name
    memcpy(pc_owner, owner_name, 32);

	ptr->formatDrive(8);

};

//pc print function
void PC::print()
{
    Computer::print();
    cout << "The owner of this computer is: " << endl;
    for(int i = 0; i < strlen(pc_owner); i++)
    {
        cout << pc_owner[i];
    }
    
    cout << "\n\n\n\n\n";
}

//constructor for printer
printer::printer(char* name, int num_bytes, int num_inp):Computer(name, num_bytes)
{
    //create new computer
    new Computer(name, num_bytes);

	maximum_number_of_files = 16;
    

    //set num to num_inp
    num = num_inp;

	ptr->formatDrive(16);
};

//printer print function
void printer::print()
{
    Computer::print();
    cout << "The number of printers attach to this computer is: " << num << endl;
    cout << "\n\n\n\n\n";
}

//constructor for server
server::server(char* name, int num_bytes, char* location_inp):Computer(name, num_bytes)
{
    //create new computer
    new Computer(name, num_bytes);

	maximum_number_of_files = 16;

    //set location to 0
    memset(location, 0, 16);
    
    //set location to input
    memcpy(location, location_inp, 16);

	ptr->formatDrive(16);
};

//server print function
void server::print()
{
    Computer::print();
    cout << "The location of this printer is: " << endl;
    for(int i = 0; i < strlen(location); i++)
    {
        cout << location[i];
    }

	cout << "\n\n\n\n\n";
}

//find free file descriptor
int Computer::findFreeFileDescriptor()
{
	//declare variables
	int index, count;
	char test[64];
	fileDescriptor* fd = new fileDescriptor();

	//set index to 0
	index = 0;
	count = 0;

	//retrieve first block
	ptr->retrieveBlock(test, 0);

	//test first block
	while(count < 4)
	{
		memcpy(fd, test + 16 * count, 16);
		
		if(fd->file_length == -1)
		{
			index = count;
			delete fd;
			return index;
		}
	
		else if (fd->file_length != -1)
		{
			//index = count;
			//return index;
		}

		count++;
	}

	//retrieve second block
	ptr->retrieveBlock(test, 1);

	count = 0;

	//test second block
	while(count < 4)
	{
		memcpy(fd, test + 16 * count, 16);

		if (fd->file_length == -1)
		{
			delete fd;
			return count + 4;
		}

		if(fd->file_length != -1)
		{
			//index = count;
			//return index;
		}

		count++;
	}

	//retrieve third block
	if (8 < maximum_number_of_files)
	{
		//retrieve third block
		ptr->retrieveBlock(test, 2);

		count = 0;

		//test third block
		while(count < 4)
		{
			memcpy(fd, test + 16 * count, 16);

			if (fd->file_length == -1)
			{
				delete fd;
				return count + 8;
			}

			if(fd->file_length != -1)
			{
				//index = count;
				//return index;
			}

			count++;
		}

		//retrieve fourth block
		ptr->retrieveBlock(test, 3);

		count = 0;

		//test fourth block
		while(count < 4)
		{
			memcpy(fd, test + 16 * count, 16);

			if (fd->file_length == -1)
			{
				delete fd;
				return count + 12;
			}

			if(fd->file_length != -1)
			{
				//index = count;
				//return index;
			}

			count++;
		}
	}

	delete fd;

	return FILE_SYSTEM_FULL;
}

//get file descriptor
void Computer::getFileDescriptor(int n, fileDescriptor* fd)
{
	//declare variable
	char test[64];
	int block_num;
	int true_index;

	if(n < 4)
	{
		block_num = 0;
		true_index = n;
	}
	else if (n >= 4 && n < 8)
	{
		block_num = 1;
		true_index = n - 4;
	}
	else if (n >= 8 && n < 12)
	{
		block_num = 2;
		true_index = n - 8;
	}
	else if (n >= 12 &&  n < 16)
	{
		block_num = 3;
		true_index = n - 12;
	}

	ptr->retrieveBlock(test, block_num);

	memcpy(fd, test + 16 * true_index, 16);
	
}

//put file descriptor
void Computer::putFileDescriptor(int n, fileDescriptor* fd)
{
	//declare variable
	char test[64];
	int block_num;
	int true_index;
	char val[16]; 


	if(n < 4)
	{
		block_num = 0;
		true_index = n;
	}
	else if (n >= 4 && n < 8)
	{
		block_num = 1;
		true_index = n - 4;
	}
	else if (n >= 8 && n < 12)
	{
		block_num = 2;
		true_index = n - 8;
	}
	else if (n >= 12 &&  n < 16)
	{
		block_num = 3;
		true_index = n - 12;
	}

	ptr->retrieveBlock(test, block_num);

	memcpy(test + 16 * true_index, fd, 16);

	ptr->storeBlock(test, block_num);

	//memcpy(val, fd, 16);
	//memcpy(test + 16 * true_index, val, 16);
	//ptr->storeBlock(test, n);
}

//create file
int Computer::createFile(char* n, int len, char* ff)
{
	//declare variables
	int descript_index;
	char test[64];
	int block_num;
	int true_index;
	fileDescriptor* fd = new fileDescriptor();
	char transfer[16];
	int store_here;
	int data_segs;
	char to_store[64];

	//how many blocks of 64 in ff
	data_segs = (len + 63) / 64;

	//can store file?
	store_here = ptr->findNBlocks(data_segs, (maximum_number_of_files / 4) + 1);

	//if not enough room end
	if(store_here == -1)
	{
		return NOT_ENOUGH_SPACE;
	}

	//find if free slot?
	descript_index = findFreeFileDescriptor();

	if(descript_index == FILE_SYSTEM_FULL)
	{
		return FILE_SYSTEM_FULL;
	}
	
	if(descript_index < 4)
	{
		block_num = 0;
		true_index = descript_index;
	}
	else if (descript_index >= 4 && descript_index < 8)
	{
		block_num = 1;
		true_index = descript_index - 4;
	}
	else if (descript_index >= 8 && descript_index < 12)
	{
		block_num = 2;
		true_index = descript_index - 8;
	}
	else if (descript_index >= 12 &&  descript_index < 16)
	{
		block_num = 3;
		true_index = descript_index - 12;
	}
	

	//retrieve block
	ptr->retrieveBlock(test, block_num);
	
	//retrieve file descriptor
	memcpy(fd, test + 16 * true_index, 16);

	//set name
	memset((fd->name), 0, strlen(fd->name) + 1);

	//copy name
	memcpy((fd->name), n, 8);

	//set length
	fd->file_length = len;

	//set starting block
	fd->block_number = store_here;

	//update values
	//memcpy(test + 16 * true_index, fd, 16);
	memcpy(transfer, fd, 16);

	putFileDescriptor(descript_index, fd);

	//transfer data to memory
	for(int i = 0; i < data_segs; i++)
	{
		memset(to_store, 0, 64);
		memcpy(to_store, ff + (64 * i), 64);
		ptr->storeBlock(to_store, fd->block_number + (1 * i));
	}

	//allocate blocks
	for(int i = 0; i < data_segs; i++)
	{
		ptr->allocateBlock(store_here + i);
	}

	delete fd;

	return 0;

}

//print directory
void Computer::printDirectory()
{
	//declare variables
	int count;
	fileDescriptor* fd = new fileDescriptor();
	int current_file;
	char buffer[9];


	//set to 0
	count = 0;
	current_file = 0;

	while(count < maximum_number_of_files)
	{
		getFileDescriptor(count, fd);

		memset(buffer, 0, 9);

		memcpy(buffer, fd->name, 8);

		if((fd->file_length) == -1)
		{
			cout << "File entry " << count << " - not in use" << endl;
		}
		else
		{
			cout << "File entry " << count << " - file name = " << buffer << ", length = " << fd->file_length 
				<< ", first block on disk = " << fd->block_number << endl;
		}

		count++;
		
	}

	delete fd;
}

//findFile
int Computer::findFile(char* fname, fileDescriptor* fd)
{
	int test, file_num;
	fileDescriptor* fd_function = new fileDescriptor();

	test = 1;
	file_num = 0;

	while(file_num < maximum_number_of_files && test != 0)
	{
		//get next file descriptor
		getFileDescriptor(file_num, fd_function);
		
		//compare given name and file descriptor name
		test = strcmp(fname, fd_function->name);

		//if names aren't the same, go to next file
		if(test != 0)
		{
			file_num++;
		}
	}

	if(file_num < maximum_number_of_files)
	{
		memcpy(fd, fd_function, strlen(fd_function->name));
		fd->file_length = fd_function->file_length;
		fd->block_number = fd_function->block_number;
		delete fd_function;
		return file_num;
	}

	else
	{
		delete fd_function;
		return -1;
	}

	
}

//print one file
void Computer::printOneFile(fileDescriptor fd)
{
	//variables
	char file_content[64];
	int data_segs;
	int count;

	count = 0;

	//determine data segs
	data_segs = (fd.file_length + 63) / 64;

	

	cout << "Printing file " << fd.name << ":" << endl;
	for(int i = 0; i < data_segs; i++)
	{

		//set file content to 0
		memset(file_content, 0, 64);

		ptr->retrieveBlock(file_content, fd.block_number + (1 * i));

		for(int i = 0; count < fd.file_length && i < 64; i++)
		{
			if(file_content[i] == '\n')
			{
				cout << "\n"; 
				count++;
			}
			else
			{
				cout << file_content[i];
				count++;
			}
		}

		
	}
	cout << "\n" << endl;
}

//print files
void Computer::printFiles(int argc, char* argv[])
{
	//declare variables
	int test;
	fileDescriptor fd;

	for(int i = 2; i < argc; i++)
	{
		test = findFile(argv[i], &fd);

		if(test != -1)
		{
			printOneFile(fd);
		}
		else
		{
			cout << "File " << argv[i] << " Not Found." << endl;
		}

	}

}