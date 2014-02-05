#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#include "system_utilities.h"
#include "machines.h"
#include "definitions.h"

int main(int argc, char argv)
{
	
    //declare variables
	ifstream infile;
	int cmd_num, num_tokens, err_check, node_pos;
    char* tokens[MAX_TOKENS_ON_A_LINE];
    char input[MAX_CMD_LINE_LENGTH];
	char* file_text;
	char buffer[MAX_CMD_LINE_LENGTH];
	int char_count, prev_char_count;
	char* file_token[MAX_TOKENS_ON_A_LINE];
	int num_file_char;
	int num_data_tokens;

	//initialize
	char_count = 0;
	prev_char_count = 0;

	//read input file
	infile.open("p8input.txt", ios::in);
	if(infile.fail())
	{
		cout << "Failure reading input file" << endl;
		return 1;
	}

	//fill command list
	fillCommandList();

	//main loop
	do
	{
		//set input to 0
        memset(input, 0, MAX_CMD_LINE_LENGTH);
        
        //get line
        infile.getline(input, MAX_CMD_LINE_LENGTH);
        
        //parse command line
        num_tokens = parseCommandLine(input, tokens);
        
        //get command number
        cmd_num = getCommandNumber(tokens[0]);

		//switch statement
		switch(cmd_num)
		{
			case ADD_NETWORK_NODE:
				cout << "Recognized add_network_node command" << endl;
				err_check = addNetworkNode(tokens, num_tokens);
				printError(err_check);
				break;

			case DELETE_NETWORK_NODE:
				cout << "Recognized delete_network_node command" << endl;
				err_check = deleteNetworkNode(tokens, num_tokens);
				printError(err_check);
				break;

			case SYSTEM_STATUS:
				cout << "Recognized system_status command\n\n\n" << endl;
				systemStatus();
				wait();
				break;
				
			case HALT:
				cout << "Recognized the halt command. Program will now end" << endl;
				break;
			
			case LS:
				cout << "Recognized the ls command" << endl;
				node_pos = findNode(tokens[1]);
				err_check = node_pos;
				if(err_check == COMPUTER_NOT_FOUND)
				{
					printError(err_check);
					break;
				}
				else
				{
					selectCompAndListDescriptors(node_pos);
				}
				break;
				
			case UNDEFINED_COMMAND:
				cout << "Undefined Command" << endl;
				break;

			case PRINT_FILES:
				cout << "Print Files commands recognized" << endl;
				for(int i = 0; i < num_tokens; i++)
				node_pos = findNode(tokens[1]);
				selectCompAndPrintFiles(node_pos, num_tokens, tokens);
				break;

			case CREATE_FILE:
				cout << "Recognized the create_file command" << endl;
				node_pos = findNode(tokens[1]);
				err_check = node_pos;
				num_file_char = str_convert(tokens[3]);
				file_text = (char*) malloc(num_file_char);
				memset(file_text, 0, num_file_char);
				if(err_check == COMPUTER_NOT_FOUND)
				{
					printError(err_check);
					free(file_text);
					break;
				}
				else
				{
					while(prev_char_count < num_file_char)
					{
						//make buffer 0
						memset(buffer, 0, MAX_CMD_LINE_LENGTH);

						//read in next line
						infile.getline(buffer, MAX_CMD_LINE_LENGTH);

						//parse file line
						num_data_tokens = parseCommandLine(buffer, file_token);

						if(prev_char_count > 0)
						{
							file_text[prev_char_count - 1] = '\n';
						}
						
						for(int i = 0; i < num_data_tokens; i++)
						{

							char_count = strlen(file_token[i]);

							memcpy(file_text + (prev_char_count), file_token[i], char_count);

							prev_char_count = prev_char_count + char_count;
						
							if(prev_char_count != num_file_char - 1)
							{
								memcpy(file_text + (prev_char_count), " ", 1);
							}

							prev_char_count++;
							char_count++;
						}
					}
					err_check = selectCompAndCreateFile(node_pos, tokens[2], str_convert(tokens[3]), file_text);
				}
				prev_char_count = 0;
				char_count = 0;
				if (err_check == FILE_SYSTEM_FULL)
				{
					cout << "File system was full. The file was not created." << endl;
					free(file_text);
					break;
				}
				else
				{
					cout << "File created successfully" << endl;
				}
				free(file_text);
				break;
		}

		//free allocated memory for tokens
		for(int i = 0; i < num_tokens; i++)
        {
            delete(tokens[i]);
        }
        
		cout << "\n\n" << endl;

        
    }while (cmd_num != HALT);
	



	wait();
    return 0;
}