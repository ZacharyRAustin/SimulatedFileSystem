#include "system_utilities.h"
#include "definitions.h"
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

#include "system_utilities.h"
#include "machines.h"


//declare computer pointer
Computer* network[MAX_NETWORK_NODES];

//declare int to keep track of the number of computers in the network
int number_comp = 0;

//declare int arr to keep track of computer positions in the network
int comp_positions[MAX_NETWORK_NODES] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

//declare into to keep track of position in comp_positions
int position_number = 0;

//string to int converter
int str_convert(char* a)
{
    //declare variables
    int char_value, number, total_so_far;
    
    //intialize variables
    char_value = 0;
    number = 0;
    total_so_far = 0;
    
    //for loop
    for(int i = 0; i < strlen(a); i++)
    {
        //get char value
        char_value = a[i];
        
        //get decimal value
        number = char_value - 48;
        
        //get total so far
        total_so_far = (10 * total_so_far) + number;
    
    }
    
    return total_so_far;
    
}

//system status function
void systemStatus()
{
	for(int i = 0; i < MAX_NETWORK_NODES; i++)
    {
		if(comp_positions[i] != -1)
		{
        network[comp_positions[i]]->print();
		}    
	}
}

//add network node function
int addNetworkNode(char* toks[], int numberToks)
{
    if(number_comp == MAX_NETWORK_NODES)
    {
        return NETWORK_FULL;
    }
    else
    {
        if(strcmp(toks[1], "PC") == 0)
        {
            network[number_comp] = new PC(toks[2], str_convert(toks[3]), toks[4]);
			comp_positions[position_number] = number_comp;
            position_number++;
			number_comp++;
			
            return 0;
        }
        
        else if (strcmp(toks[1], "printer") == 0)
        {
            network[number_comp] = new printer(toks[2], str_convert(toks[3]), str_convert(toks[4]));
			comp_positions[position_number] = number_comp;
            position_number++;
            number_comp++;
            return 0;
        }
        else if (strcmp(toks[1], "server") == 0)
        {
            network[number_comp] = new server(toks[2], str_convert(toks[3]), toks[4]);
			comp_positions[position_number] = number_comp;
            position_number++;
            number_comp++;
            return 0;
        }
        else
        {
            return INVALID_COMPUTER_TYPE;
        }
    }
}

//delete network node function
int deleteNetworkNode (char* toks[], int numberToks)
{
    //declare variable
	int node_in_network;
    
    //set node in network to return value from find node
    node_in_network = findNode(toks[1]);
    
    //test node in network
    if(node_in_network == -1)
    {
        return COMPUTER_NOT_FOUND;
    }
    
    else if (node_in_network != -1)
    {
        delete network[node_in_network];
		comp_positions[node_in_network] = -1;
		number_comp--;
        return 0;
    }
}

//find node function
int findNode(char* n)
{
    //declare variables
    int place;
    
    //set place to -1;
    place = -1;
    
	for(int i = 0; i < number_comp; i++)
    {
        if(network[i]->isThisMyName(n) == 1)
        {
            place = i;
			return place;
        }
    }
    
    //return value
	return COMPUTER_NOT_FOUND;
}

//parseCommandLine function
int parseCommandLine(char line[], char* tokens[])
{
    //declare variables
    int count, char_position, num_tokens, token_length, token_position;
    char* holder;
    
    //initialize variables to 0
    count = 0;
    token_length = 0;
    num_tokens = 0;
    char_position = 0;
    token_position = 0;
    
    //while we are still on the same line
    while (count < MAX_CMD_LINE_LENGTH)
    {
        //check for first non blank input
        while(line[count] == ' ')
        {
            //add to count until find a non blank input
            count++;
        }
        
        //check if non blank input is a "
        if(line[count] == '"')
        {
            //add one to count to get position of first char of new token
            count++;
            
            //set char position to position of first char of new token
            char_position = count;
            
            //while token isn't terminated by end "
            while(line[count] != '"')
            {
                
                //add 1 to token length
                token_length++;
                
                //add one to count
                count++;
            }
            
            //allocate memory for next position in token array
            tokens[token_position] = (char*) malloc(token_length + 1);
            
			//intialize token position to 0
			memset(tokens[token_position], 0, token_length + 1);
            
            //allocate memory for holder array to hold token values
            holder = (char*) malloc(token_length + 1);
            
            //initialize holder to 0
            memset(holder, 0, token_length + 1);
            
            //set token char values to holder
            for(int i = 0; i < token_length; i++)
            {
                holder[i] = line[char_position];
                char_position++;
            }
            
            //place holder values in token[]
            memcpy(tokens[token_position], holder, token_length);
            
            //free holder
            free(holder);
            
            //increase the number of tokens, count and token position by 1
            token_position++;
            num_tokens++;
            count++;
            
			//reset token_length to 0
			token_length = 0;
            
        }
        
        //if next non blank input is not a "
        if(line[count] != '"' && line[count] != ' ')
        {
            //set char position to position of first char of new token
            char_position = count;
            
            //while token isn't terminated by end "
            while(line[count] != ' ')
            {
                
                //add 1 to token length
                token_length++;
                
                //add one to count
                count++;
            }
            
            //allocate memory for next position in token array
            tokens[token_position] = (char*) malloc(token_length + 1);
            
			//intialize token position to 0
			memset(tokens[token_position], 0, token_length + 1);
            
            //allocate memory for holder array to hold token values
            holder = (char*) malloc(token_length + 1);
            
            //initialize holder to 0
            memset(holder, 0, token_length + 1);
            
            //set token char values to holder
            for(int i = 0; i < token_length; i++)
            {
                holder[i] = line[char_position];
                char_position++;
            }
            
            //place holder values in token[]
            memcpy(tokens[token_position], holder, token_length);
            
            //free holder
            free(holder);
            
            //increase the number of tokens and token position by 1
            token_position++;
            num_tokens++;
            count++;
            
			//reset token length to 0
			token_length = 0;
            
        }
        
    }
    
    return num_tokens;
}

//command element class
class commandElement
{
public:
	commandElement(char* a, int b);
	int amIThisCommand(char* input);
    
private:
	char* pointer;
	int cmd;
};

//command element constructor
commandElement::commandElement(char* a, int b)
{
    //declare variables
    int string_length;
    
    //determine what string length should be
    string_length = strlen(a);
    
    //allocate space for class member pointer
    pointer = (char*) malloc(string_length);

	//set pointer memory to 0
	memset(pointer, 0, 20);
    
    //copy string into class member pointer
    memcpy(pointer, a, string_length);
    
    //set cmd to the given command
    cmd = b;
}

// amIThisCommand function
int commandElement::amIThisCommand(char* input)
{
    if(strcmp(input, pointer) == 0)
    {
        return cmd;
    }
    else
        return 0;
}

//declare variables
commandElement* cmd_ptr[NUMBER_OF_COMMANDS];

//fill command list function
void fillCommandList()
{

	//allocate storage for all commands in cmd_ptr
	for(int i = 0; i < NUMBER_OF_COMMANDS; i++)
	{
		cmd_ptr[i] = (commandElement*) malloc(sizeof(commandElement));
	}
    
	//set all ptrs to 0 in cmd_ptr
	for(int i = 0; i < NUMBER_OF_COMMANDS; i++)
	{
		memset(cmd_ptr[i], 0, sizeof(commandElement));
	}
    
	//set first element to system status
	cmd_ptr[0] = new commandElement("system_status", SYSTEM_STATUS);

    //set second element to halt
    cmd_ptr[1] = new commandElement("halt", HALT);
    
    //set third element to add_network_node
    cmd_ptr[2] = new commandElement("add_network_node", ADD_NETWORK_NODE);
    
    //set fourth element to delete_network_node
    cmd_ptr[3] = new commandElement("delete_network_node", DELETE_NETWORK_NODE);
    
    //set fifth element to create_file
    cmd_ptr[4] = new commandElement("create_file", CREATE_FILE);
    
    //set sixth element to ls
    cmd_ptr[5] = new commandElement("ls", LS);
    
    //set seventh element to delete_files
    cmd_ptr[6] = new commandElement("delete_files", DELETE_FILES);
    
    //set eight element to print_files
    cmd_ptr[7] = new commandElement("print_files", PRINT_FILES);
    
    //set ninth element to transfer_file
    cmd_ptr[8] = new commandElement("transfer_file", TRANSFER_FILE);
    
	
}

//get command number function
int getCommandNumber(char* s)
{
    //declare variables
    int i, cmd_num, count;
    
    //intialize variables to zero
    i = 0;
    cmd_num = 0;
    count = 9;
    
    //determine which command it is and set cmd_num to command value
    do {
        cmd_num = cmd_ptr[i]->amIThisCommand(s);
        i++;
        count--;
        
    } while (cmd_num == 0 && count > 0);
    
    if(cmd_num == 0)
    {
        return UNDEFINED_COMMAND;
    }
    else
    {
        return cmd_num;
    }
}

//wait function
void wait()
{
	system("pause");
}

//print error
void printError(int err)
{
	if(err == BAD_BLOCK_NUMBER)
	{
		cout << "Error. This was a bad block number.\n\n\n" << endl;
	}

	else if (err == NETWORK_FULL)
	{
		cout << "The Network is Full. This computer was not added.\n\n\n" << endl;
	}
	else if (err == COMPUTER_NOT_FOUND)
	{
		cout << "This computer was not found. No operation occurred.\n\n\n" << endl;
	}
	else if (err == INVALID_COMPUTER_TYPE)
	{
		cout << "This is an invalid computer type. It was not added to the network.\n\n\n" << endl;
	}
	else if (err == FILE_SYSTEM_FULL)
	{
		cout << "The file system is full." << endl;
	}
	else if (err != 0)
	{
		cout << "Unspecified Error\n\n\n" << endl;
	}
}

//file descriptor constructor
fileDescriptor::fileDescriptor()
{
	memset(name, 0, 8);
	file_length = -1;
	block_number = 0;
}

//function to select computer from network array and create a file on that computer
int selectCompAndCreateFile(int pos, char* name, int length, char* ff)
{
	//declare variable
	int val;

	val = network[pos]->createFile(name, length, ff);

	return val;
}

//function to select computer from network array and prints its file descriptors
void selectCompAndListDescriptors(int pos)
{
	network[pos]->printDirectory();
}

//select computer and print files
void selectCompAndPrintFiles(int pos, int argc, char* argv[])
{
	network[pos]->printFiles(argc, argv);
}