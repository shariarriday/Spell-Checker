//Bismillahir Rahmanir Rahim

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Define a constant
#define CHAR_LIMIT 27
#define WORD_LIMIT 46
#define SUGGESION_LIMIT 100

//createing an array that suggests upto 100 words
char suggestions[SUGGESION_LIMIT][WORD_LIMIT];
int word_num = 0;
int word_len[SUGGESION_LIMIT];
	
//Datastructure definition
typedef struct trienode
{
	struct trienode* child_node[CHAR_LIMIT];
	bool word_end;
}trienode;

//Function declaration
int indexing(char letter); //function for indexing letters to numbers
void freenode(struct trienode* root);  //function to clear allocated memory
bool insert(struct trienode* root , char* word);  //a boolen function to check if everything is inserted correctly	
void suggest(struct trienode* root , char* word);  //function for suggestion of words
bool check(struct trienode* root , char* word);  //function to check if a word exists in dictionary
int anti_index(char index);  //function to change numbers to letters
bool new_word(char* new_word);
void complete(struct trienode* root);

//Defines a node 
struct trienode* trie;

/*
 *
 * main function
 * this contains main file i/o and other things
 */
int main()
{
	//Random Bullshit	
	printf("Welcome to the autocomplete engine, It's still in beta...\n");
	printf("Loading...\n");

	//CALLOCS THE Data
	trie = calloc(1, sizeof(trienode));

	//Creates a file pointer
	FILE* dict = fopen("words.txt" , "r+");

	//intializes an array for the words collected from the file
	char word[WORD_LIMIT];
	
	//Checks if pointer is NULL, if not then proceeds
	if(dict != NULL)
	{
		while(fscanf(dict , "%s" , word) != EOF)
		{
			if(!insert(trie , word)) // inserts each word in the data structure
				return 1; //error check
		}
		fclose(dict);
	}

	printf("Loading Done :D ...\n");
	printf("Please state what you want to do now:\n");	
	printf("1)Find out wrong spellings in a document.\n");
	printf("2)Find out wrong spellings in the paragraph written in console.\n");	
	printf("3)Use autocomplete to find words.\n");
	printf("4)Quit.\n");	

	do
	{
		char choice;
		scanf("%c" , &choice);

		if(choice == '1')
		{
			printf("Please provide the path of the document\n");
			char path[1000];
			scanf("%s" , path);
			
			//Creates a pointer with the input as a path
			FILE* doc = fopen(path , "r+");

			//Checks if pointer is NULL, if not then proceeds
			if(doc != NULL)
			{
				int wrong_count = 1;
				int word_count = 1;
				while(fscanf(doc , "%s" , word) != EOF)
				{
					
					//trims the word and makes it all small letters
					int len = strlen(word);
					for (int i = 0; i < len; ++i)
					{
						if(word[i] == '?' || word[i] == '.' || word[i] == '!' || word[i] == ';')
						{
							for (int j = i; j < len; ++j)
							{
								word[j] = word[j+1];
							}
						}
						else
						{
							word[i] = tolower(word[i]);
						}
					}
			
					//checks the word against all the words in the dictionary
					if(!check(trie , word))		
					{
						printf("Word No: %d \nWrong Count: %d: %s\n" , word_count, wrong_count++ , word);
					}
					word_count++;
				}
				fclose(doc);
			}
		break;
		}
		else if(choice == '2')
		{
			int word_count = 1;
			printf("Start typing here,(Maximum 100000 letters)\n");
			char input[100001];
			scanf(" %[^\n]%*c" , input);
			int len = strlen(input);
			int j = 0;
			int wrong_count = 1;
			for (int i = 0; i < len+1; ++i)
			{		
				word[j] = input[i];
				j++;
				if((input[i] == ' ' || input[i] == '?' || input[i] == '.' || input[i] == '!' || input[i] == ';' || input[i] == ',' || input[i] == '\0') && j!=1)
				{
					word[--j] = '\0';
					//trims the word and makes it all small letters
					int len_w = strlen(word);
					for (int k = 0; k < len_w; ++k)
						word[k] = tolower(word[k]);
			
					//checks the word against all the words in the dictionary
					if(!check(trie , word))		
					{
						printf("Word No: %d \nWrong Count: %d: %s\n" , word_count, wrong_count++ , word);
					}
					word_count++;
					j=0;
				}
				else if(j == 1 && input[i] == ' ')
					j=0;		
			}
		break;
		}
		else if(choice == '3')
		{
			scanf("%s" , word);
			suggest(trie , word);
			printf("\n");
			break;
		}
		else if(choice == '4')
			break;
		else
		{
			printf("Error, Please try giving correct input\n");
		}
	}while(1);
}

//Defining the functions
int indexing(char letter)
{ 
	if(letter != '\'')
		return (letter - 'a');

	return (CHAR_LIMIT-1); //returns 26 for \'
}

//Inserts the words in the data structure
bool insert ( struct trienode* root , char* word)
{
	//saves root nodes ponter
	trienode* save = root;
	
	int len = strlen(word);
	for (int i = 0; i < len; i++)
	{
		int index = indexing(word[i]);
		if(root->child_node[index] == NULL)
		{
			root->child_node[index] = calloc(1,sizeof(trienode));
			root->word_end = false;
			root = root->child_node[index];
		}
		else
			root = root->child_node[index];
	}
	//marks the end of a word
	root->word_end = true;
	//returns root to its initial stage
	root = save;

	return true;
}

bool check(struct trienode* root , char* word)
{
	//new ponter to traverse
	trienode* trav = root;

	int len = strlen(word);

	for (int i = 0; i < len; ++i)
	{
		int index = indexing(word[i]);

		if(trav->child_node[index] == NULL)
		{
			return false;
		}
		trav = trav->child_node[index];
	}

	if(trav->word_end)
		return true;

	return true;
}

//function to suggest
void suggest(struct trienode* root , char* word)
{
	//new ponter to traverse
	trienode* trav = root;

	int len = strlen(word);

	for (int i = 0; i < len; ++i)
	{
		int index = indexing(word[i]);
		printf("%c" , word[i]);
		if(trav->child_node[index] == NULL)
		{
			printf("...Sorry, Try again\n");
			return;
		}
		trav = trav->child_node[index];
	}
	if(trav != NULL)
		complete(trav);
}

void complete(struct trienode* root)
{
	//new ponter to traverse
	trienode* trav = root;

	if(trav->word_end == true)
		return;	

	for (int i = 0; i < 27; ++i)
	{
		if(trav->child_node[i] != NULL)
		{
			int anti = anti_index(i);
			printf("%c" , anti);
			complete(trav->child_node[i]);
			return;
		}
	}
}

//frees up memory using a recursive function
void freenode(struct trienode* root)
{
	for(int i = 0 ; i < CHAR_LIMIT ; i++)
	{
		if(root->child_node[i] != NULL)
		{
			freenode(root->child_node[i]);
		}
	}
	free(root);
}

//creates a char value from index
int anti_index(char index)
{
	if(index == 26)
		return '\'';
	else
		return (index+'a');
}