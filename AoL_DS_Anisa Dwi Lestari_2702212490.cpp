#include <stdio.h> // included for standard input output function
#include <string.h> // included for string manipulation function
#include <stdlib.h> // included for memory allocation (malloc)

// define a structure to represent node in trie
struct TrieNode{
	char ch; // character stored in the node
	char* desc;	// description of the word
	int endWord; // mark the node to determine if it's the end of a word
	struct TrieNode* children[128]; // represent the children nodes of the current node
};

// function to create new trie node
struct TrieNode* createTrieNode(){
	// alocates memory for the node using malloc
	struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
  
  	// set all pointers in 'children' array to NULL
	for(int i = 0; i < 128; i++){
		node->children[i] = NULL;
	}
	  
	node->ch = '\0'; // set character to NULL
	node->desc = NULL; // set description to NULL
	node->endWord = 0; // set endWord flag to '0' or false
	  
	return node; // return to newly created node
}

// function to insert a word and its description into the trie
// takes 3 parameters: root node of the trie, word to be insert, description of the word
void insertTrie(struct TrieNode* root, char* word, char* desc){
	struct TrieNode* curr = root; // initialize 'curr' to the root node

	// iterates each character in the word 
	for (int i = 0; word[i] != '\0'; i++){
		int idx = word[i] - 'a'; // calculates index based on ASCII value
	    
	    // if the corresponding child doesn't exist
	    if(curr->children[idx] == NULL){
			curr->children[idx] = createTrieNode(); // create child using 'createTrieNode' function
	    	curr->children[idx]->ch = word[i]; // sets its character to the current character
	    }
	    
	    // have a prefix match, go down a level
	    curr = curr->children[idx];
	}
	
	curr->endWord = 1; // mark node as the end of a word
	curr->desc = (char*)malloc(sizeof(char) * (strlen(desc) + 1)); // allocates memory for 'desc' of the node
	strcpy(curr->desc, desc); // copies description string 
}

// function that allows user to add new slang word and its description
void releaseSlangWord(struct TrieNode* root){ // takes 1 parameter: root node of the trie
    char newWord[128]; // array 'newWord' to store new slang word
    int hasSpace; // tracks if the new word contains space
    
    // looping until user inputs at least 2 characters and no spaces
    do{
        hasSpace = 1; // set hasSpace flag to 1
        // prompt user to input new slang word 
        printf("Input a new slang word [Must be more than 1 characters and contain no spaces]: ");
        scanf("%[^\n]", newWord); // read user input until a newline character
        getchar(); // clears buffer
        
        // iterates each character in the word 
        for(int i = 0; newWord[i] != '\0'; i++){
            if(newWord[i] == ' '){ // if current character is a space
                hasSpace = 0; // set hasSpace to 0
                break; // breaks out the loop
            }
        }  
    }while((strlen(newWord) < 2) || (hasSpace == 0)); // looping until user inputs at least 2 characters and no spaces

    char wordDesc[128]; // array 'wordDesc' to store description
    int wordCount; // count the number of words in the description
    
    // looping until user inputs a description at least 3 words
    do{
        wordCount = 1; // set wordCount flag to 1
        // prompt user to input new slang word description at least 3 words
        printf("Input a new slang word description [Must be more than 2 words]: ");
        scanf("%[^\n]", wordDesc); // read user input until a newline character
        getchar(); // clears buffer
        
        // iterates each character in the word
        for(int i = 0; wordDesc[i] != '\0'; i++){
            if(wordDesc[i] == ' '){ // if current character is a space
                wordCount++; // count the number of words
            }
        }
    }while(wordCount < 3); // looping until user inputs a description at least 3 words

   // Check if the word already exists in the Trie
    struct TrieNode* curr = root; // initialize 'curr' to root node
    
    int wordExists = 1; // set wordExists to 1
    
    // iterates each character in the word
    for(int i = 0; newWord[i] != '\0'; i++){
        int idx = newWord[i] - 'a'; // calculates index based on ASCII value
        
        // if the corresponding child doesn't exist
        if(curr->children[idx] == NULL){
            wordExists = 0; // set wordExists to 0
            break; // breaks out the loop
        }
        
        // have a prefix match, go down a level
        curr = curr->children[idx];
    }

	// if word exists and current node marks to end of a word, update its description
    if(wordExists && curr->endWord){
        free(curr->desc);  // Free the previous description
        curr->desc = (char*)malloc(sizeof(char) * (strlen(wordDesc) + 1)); // allocates memory for 'wordDesc' of the node
        strcpy(curr->desc, wordDesc); // copies description string
        printf("\nSuccessfully updated a slang word.\n"); // print message word update is successful
    }else{ // if word doesn't exists, insert into trie
        insertTrie(root, newWord, wordDesc); // insert into the trie
        printf("\nSuccessfully released new slang word.\n"); // print message new word release is successful
    }
	
    printf("Press enter to continue...\n"); // print message to prompt user to press enter
    getchar(); // wait for user to press enter 
}

// function that allows user to search a slang word and its description
void searchSlangWord(struct TrieNode* root){ // takes 1 parameter: root node of the trie
	char searchWord[128]; // array 'searchWord' to store the slang word
	int hasSpace; // tracks if the new word contains space
    
    // looping until user inputs at least 2 characters and no spaces
    do{
        hasSpace = 1; // set hasSpace flag to 1
        // propmt user to input slang word to be searched
		printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
		scanf("%[^\n]", searchWord); // read user input until a newline character
        getchar(); // clears buffer
        
        // iterates each character in the word 
        for(int i = 0; searchWord[i] != '\0'; i++){
            if(searchWord[i] == ' '){ // if current character is a space
                hasSpace = 0; // set hasSpace to 0
                break; // breaks out the loop
            }
        }
    }while((strlen(searchWord) < 2) || (hasSpace == 0)); // looping until user inputs at least 2 characters and no spaces
    
	
	// Check if the word already exists in the Trie
    struct TrieNode* curr = root; // initialize 'curr' to root node
    
    // iterates each character in the word
    for(int i = 0; searchWord[i] != '\0'; i++){
        int idx = searchWord[i] - 'a'; //calculates index based on ASCII value
        
        // if the corresponding child doesn't exist
        if(curr->children[idx] == NULL){
            printf("\nThere is no word \"%s\" in the dictionary.\n", searchWord); // print message that word is not found
            printf("Press enter to continue...\n"); // print message to prompt user to press enter
            getchar(); // wait for user to press enter
            return;  // exit function
        }
        
        // have a prefix match, go down a level
        curr = curr->children[idx];
    }

	// if current node marks to end of a word
    if(curr->endWord){
        printf("\nSlang word  : %s\n", searchWord); // print slang word
        printf("Description : %s\n", curr->desc); // print description
    }
    printf("Press enter to continue...\n"); // print message to prompt user to press enter
    getchar(); // wait for user to press enter
}

// function that lists all the words in the trie that have a given prefix
// takes 4 parameters: root node, prefix string, current index, count of words
void listPrefix(struct TrieNode *root, char *prefix, int x, int* count){ 
	struct TrieNode *curr = root; // initialize 'curr' to the root node
	
	if(curr->endWord){ // if current node marks to end of a word
		prefix[x] = '\0'; // set the current character to null
		printf("%d. %s\n", (*count)++, prefix); // prints the slang words
	}
	
	// iterates each node 
    for(int i = 0; i < 128; i++){
    	// if the corresponding child exists
    	if(curr->children[i] != NULL){
    		prefix[x] = i + 'a'; // updated the prefix array
    		listPrefix(curr->children[i], prefix, x + 1, count); // call 'listPrefix' for all child of the current node
		}
    }
}

// function that allows user to search all the slang word that have a given trie
void viewAllSlangWordsWithPrefix(struct TrieNode *root){ // takes 1 parameter: root node of trie
	int count = 1; // initialize count words to 1
    char prefix[100]; // array 'prefix' to store the prefix input
    
	struct TrieNode *curr = root; // initialize 'curr' to the root node

	// propmt user to input prefix to be searched
    printf("Input a prefix to be searched: ");
    scanf("%s", prefix); // read user input
    getchar(); // clears buffer

    int len = strlen(prefix); // calculate length prefix

    for(int i = 0; i < len; i++){ // iterates each character of prefix
        int idx = prefix[i] - 'a'; // calculates index based on ASCII value

        if(curr->children[idx] == NULL){ // if the corresponding child doesn't exist
            printf("\nThere is no prefix \"%s\" in the dictionary.\n", prefix); // print message that word is not found
            printf("Press enter to continue...\n"); // print message to prompt user to press enter
            getchar(); // wait for user to press enter
            return; // exit function
        }
		
		// have a prefix match, go down a level
		curr = curr->children[idx];
    }

    printf("\nWords starts with '%s':\n", prefix); // print message that word is found
    listPrefix(curr, prefix, len, &count); // call 'listPrefix' function and print slang word

    printf("Press enter to continue...\n"); // print message to prompt user to press enter
    getchar(); // wait for user to press enter
}

// function that prints all the words in the trie
// takes 4 parameters: root node, array temp, current depth, count words
void printAllSlangWords(TrieNode* root, char temp[], int depth, int* count) {
    
    if(root->endWord){ // if current root marks to end of a word
        temp[depth] = '\0'; // set the next character of temp to null
        printf("%d. %s\n", (*count)++, temp); // prints the slang words
    }

    for(int i = 0; i < 128; i++){ 	// iterates each node
        if(root->children[i] != NULL){ // if the corresponding child exists
            temp[depth] = root->children[i]->ch; // updates the temp array with the character of that child node
            printAllSlangWords(root->children[i], temp, depth + 1, count);  // recursive calls 'printAllSlangWords' for that child node
        }
    }
}

// function that allows user to view all the slang words in the trie
void viewAllSlangWords(TrieNode* root){ // takes 1 parameters: root node
	int count = 1; // initialize count of words to 1
    char temp[128];  // array 'temp' to store the current word
    
    int hasChildren = 0; // set 'hasChildren' flag to 0
    
    for(int i = 0; i < 128; i++){ // iterates each node
        if(root->children[i]!= NULL){ // if the corresponding child exists
            hasChildren = 1;  // set hasChildren to 1
            break; // breaks out the loop
        }
    }
    
    if(hasChildren == 0){ // if root node does not have children
    	printf("\nThere is no slang word yet in the dictionary.\n"); // print message that word is not found
	}else{ // if root node has children
		printf("\nList of all slang words in the dictionary: \n"); // print message that word is found
		printAllSlangWords(root, temp, 0, &count); // calls 'printAllSlangWords' to print the words
	}
	
	printf("Press enter to continue...\n"); // print message to prompt user to press enter
	getchar(); // wait for user to press enter
}
    
// main function	
int main(){
	struct TrieNode* root = NULL; // initialize root to NULL
	root = createTrieNode(); // call 'createTrieNode' and assign to root
	int choice; // user's choice
		
	// main program loop
	while(choice != 5){ // looping until user enters '5'
		system("cls"); // clear the console screen
		printf("\n");
	    printf("     ____    ___    ___    ____  _      _____ \n");
	    printf("    | __ )  / _ \\  / _ \\  / ___|| |    | ____|\n");
	    printf("    |  _ \\ | | | || | | || |  _ | |    |  _|  \n");
	    printf("    | |_) || |_| || |_| || |_| || |___ | |___ \n");
	    printf("    |____/  \\___/  \\___/  \\____||_____||_____|\n");

        puts("");      
		// asks user to choose from 5 options                          
		printf("What do you want to do?\n");
		printf("1. Release a new slang word\n");
		printf("2. Search a slang word\n");
		printf("3. View all slang words starting with a certain prefix word\n");
		printf("4. View all slang words\n");
		printf("5. Exit\n");
		printf(">> ");
		
		scanf("%d", &choice); getchar(); // read user's choice
		
		switch(choice){
			case 1: // 1. Release a new slang word
				releaseSlangWord(root); // calls 'releaseSlangWord' function
				break; // breaks out the loop
			case 2: // 2. Search a slang word
				searchSlangWord(root); // calls 'searchSlangWord' function
				break; // breaks out the loop
			case 3: // 3. View all slang words starting with a certain prefix word
				viewAllSlangWordsWithPrefix(root); // calls 'viewAllSlangWordsWithPrefix' function	
				break; // breaks out the loop
			case 4: // 4. View all slang words
				viewAllSlangWords(root); // calls 'viewAllSlangWords' function
				break; // breaks out the loop
			case 5: // 5. Exit
				printf("\nThank you... Have a nice day :)\n"); // print a message
				break; // breaks out the loop
						
		}
	}
	
	printf("\nMade by Anisa Dwi Lestari - 2702212490\n");	
	printf("Thank you for using the program. Exiting...\n");
	
	return 0; // exit program
}
