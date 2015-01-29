/* lab2 Starter
   Functionality: 
     declares a char **array (a dynamically allocated array of pointers)
     mallocs the char **array in a function (passing it in via ***)
     passes the char **array to function for load and print
*/

/*
assume input file:

horse
cat
rat
oryx
bat
alligator
crocodile
pig
monkey
aardvark
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define  WORD_LENGTH  50
#define  INITIAL_ARRAY_MAX 25

int loadArray(char *inFileName, char ***array, int *count, int *capacity);
int insertWord(char **dictionary, int *wordCount, char word[]);
void printArray(char **array, int count);
void doubleArraySize(char ***wordArray, int *capacity);
void menu(char ***array, int *wordCount, int *capacity);

int main(int argc, char* argv[])
{
    printf("Fuck\n");
    char **wordArray;  /* declare the dynamic array of strings */
                       /* the array is NOT allocated yet */
    int capacity = INITIAL_ARRAY_MAX;
    int wordCount = 0;

    if (argc != 3)
    {
        fprintf(stderr,"Usage: %s inputfile outputfile\n", argv[0]);
        return 1;
    }

    if (loadArray(argv[1], &wordArray, &wordCount, &capacity) != 0)
    {
        fprintf(stderr,"    loadArray failed! Program terminating...\n");
        return 1;  /* don't need to call exit - we're in main! */
    }

    printf("\n  Finished loading %d words.\n", wordCount);

/* HERE IS WHAT OUR ARRAY LOOKS LIKE NOW
    wordArray is pointing to a 50-element block of memory

    wordArray[0] is a char * -> "horse\0"
             [1] is a char * -> "cat\0"
             [2] is a char * -> "rat\0"
             [3] is a char * -> "oryx\0"
             ...      char * -> "bat\0"
             ...      char * -> "alligator\0"
             ...      char * -> "crocodile\0"
             ...      char * -> "pig\0"
             ...      char * -> "monkey\0"
             [9] is a char * -> "aardvark\0"
            [10] is a char * -> ????
             ...
             ...
             ...
            [49] is a char * -> ????

   REMEMBER the 50 char * pointers are stored consecutively in 1 block
   of memory (which is why wordArray[5], for example, is well-defined,
   but the strings are distributed "randomly" throughout the heap.
*/

    printArray(wordArray, wordCount);/* comment this out if using a big file!*/

    return 0;
}

void menu(char ***array, int *wordCount, int *capacity)
{
  int exit;
  char option;
  char word[WORD_LENGTH];
  
  exit = 0;
  do 
  {
    printf("\nMenu\n");
    printf("'S'earch, 'I'nsert, 'R'emove, 'C'ount, 'P'rint, 'Q'uit\n");
    scanf("%c", &option);
    /* takes in char + newline character
       see "newline handler" below
    */
    if (option == 's' || option == 'S')
    {
      /* call search() */
      printf("\nSearch\n");
      searchWord(array, wordCount);
    } 
    else if (option == 'i' || option == 'I') 
    {
      /* call insert() */
      printf("\nInsert\n");
      printf("\nEnter a word: ");
      scanf("%s", word);
      insertWord(array, wordCount, word, capacity);
      printf("\nWord inserted.\n");
    } 
    else if (option == 'r' || option == 'R') 
    {
      /* call remove() */
      printf("\nRemove\n");
      removeWord(array, wordCount);
    } 
    else if (option == 'c' || option == 'C') 
    {
      /* call count() */
      printf("\nCount\n");
      printCount(*wordCount);
    } 
    else if (option == 'p' || option == 'P') 
    {
      printf("\nPrint\n");
      printArray(*array, *wordCount);
    } 
    else if (option == 'q' || option == 'Q') 
    {
      exit = 1;
    } 
    else 
    {
      /* invalid entry */
      printf("\nInvalid Entry, returning to main menu\n");
    }
    /* newline handler */
    while (option != '\n') 
    {
      scanf("%c", &option);
    }
  } 
  while (exit == 0);
}


int loadArray(char *inFileName, char ***array, int *count, int *capacity)
{
    FILE *inFile;
    char word[WORD_LENGTH];  /* this is the ONLY auto array we'll need */

    if ((inFile = fopen(inFileName, "r")) == NULL)
    {
	fprintf(stderr,"Error opening input file, %s\n", inFileName);
	return -1;
    }

    *array = (char **)malloc(*capacity * sizeof(char*));
    if (*array == NULL)
    {
	fprintf(stderr, "Malloc of array in loadArray failed!\n");
	return -1;
    }

    printf("Reading file %s (each . is 5000 words read)\n", inFileName);

    *count = 0;
    while (fscanf(inFile, "%s", word) == 1)
    {
	if (*count >= *capacity)
	{
/* call a function that will double the size of the array and copy its contents */
	//doubleArraySize(array, capacity);
	}

	if (insertWord(*array, count, word) != 0)
	{
	    fprintf(stderr,"    Insert returned an error!\n");
	    fclose(inFile);
	    return 1;
	}
	
	if (*count % 5000 == 0)
	{
	    printf(".");
	    fflush(stdout);  /* stdout is buffered, so have to force flush */
	}
    }

    fclose(inFile);

    return 0;
}

void doubleArraySize(char ***wordArray, int *capacity)
{
    int i; //declaring index for for-loop
    char **newArray; //declaring new array with double the size
    
    newArray = (char**)malloc((*capacity) * 2 * sizeof(char*));
    
    for (i=0; i < (*capacity); i++)//copies each element of old array into new array
    {
	newArray[i] = (*wordArray)[i];
    }
    
    /*free old array to prevent memory leak*/
    free(*wordArray);
    
    /*update array pointer to new array*/
    *wordArray = newArray;
    
    *capacity *= 2;//double the capacity of array
}

void printArray(char **array, int count)
{
   int i=0;

   printf("\n");
   do  {
       printf("array[%d]: %s\n", i, array[i]);
   } while (++i<count);
}

int insertWord(char **array, int *count, char word[])
{
    char *wordPtr;
    int i;
    int wordPosition;
    int compare;

    wordPtr = (char *)malloc((strlen(word) + 1) * sizeof(char));
    if (wordPtr == NULL)
    {
	fprintf(stderr,"    Malloc of array[%d] failed!\n", *count);
	return -1;
    }
    /* Memory for this word has been allocated, so copy characters
       and insert into array */

    strcpy(wordPtr, word);
    
    /* insert in order*/
    
    i=0;
    wordPosition = 0;
    compare = -1;
    
    if (*count > 0)
    {
	printf("%d\n", *count);
	while (i < (*count) && compare < 0)
	{
	    if ((*array)[i] == NULL)
	    {
		printf("Error: accessing memory outside array or an empty element");
		return 1;
	    }
	
	    compare = strcmp((*array)[i], wordPtr);
	    if (compare == 0)
	    {
	        printf("Error: word is already in dictionary");
	        free(wordPtr);
	        return 1;
	    }
	    
	    i++;
	    
	}
	
	wordPosition = i-1;
	
	for (i = (*count); i >= wordPosition; i--)
	{
	    (*array)[i+1] = (*array)[i];
	}
    }

    *array[wordPosition] = wordPtr;
    (*count)++;
    printf("%d\n", (*array)[1]);

    return 0;
}




