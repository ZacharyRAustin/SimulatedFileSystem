#ifndef DISK_DRIVE_H
#define DISK_DRIVE_H	

//define diskDrive class
class diskDrive
{
public:
	diskDrive(int a);
	~diskDrive();
	void print();
	int allocateBlock(int a);
	int freeBlock(int a);
	int isBlockFree(int a);
	int storeBlock(char* d, int blk);
	int retrieveBlock(char* d, int blk);
	void formatDrive(int num_files);
	int findNBlocks(int n, int start);
    
private:
	int num_blocks;
	char* bitmap_ptr;
	char* storage_ptr;
};

#endif