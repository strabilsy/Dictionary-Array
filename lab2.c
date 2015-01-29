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
void printArray(char **array, int count);
void doubleArraySize(char ***wordArray, int *capacity);
void freeMemory(char ***array, int *count);
void menu(char ***array, int *wordCount, int *capacity);
int binarySearch(char **array, int count, char *word, int *found);

int searchWord(char ***array, int *count);
int insertWord(char ***dictionary, int *wordCount, char word[], int *capacity);
int removeWord(char **array, int *count);
int printCount(int count);
int saveDictionary (char *outFileName, char ***array, int *count);




int main(int argc, char* argv[])
{    
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
    menu(&wordArray, &wordCount , &capacity);
    
    if (saveDictionary(argv[2], &wordArray, &wordCount) != 0)
    {
      printf("Error writing to file\n");
    }
    else 
    {
      printf("Dictionary written to file '%s'\n", argv[2]);
    }
		
    freeMemory(&wordArray, &wordCount);

    /*printArray(wordArray, wordCount);*/

    return 0;
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
    /*printf("Capacity: %d\n", *capacity);*/
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
	if (*count+1 >= *capacity)
	{
	    /* call a function that will double the size of the array and copy 					its contents */
	    /*printf("Count: %d\n", *count);*/
	    doubleArraySize(array, capacity);
	}

	if (insertWord(array, count, word, capacity) != 0)
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
  int i; /*declaring index for for-loop*/
  char **newArray; /*declaring new array with double the size*/
    
  newArray = (char**)malloc((*capacity) * 2 * sizeof(char*));
    
  for (i=0; i < (*capacity); i++)/*copies each element of old array into  																		 new array*/
  {
    newArray[i] = (*wordArray)[i];
  }
    
  /*free old array to prevent memory leak*/
  free(*wordArray);
    
  /*update array pointer to new array*/
  *wordArray = newArray;
    
  *capacity *= 2;/*double the capacity of array*/
}

void freeMemory(char ***array, int *count)
{
    int i;
    for (i = 0; i < *count; i++)
    {
	if ((*array)[i] != NULL)
	{
	    free((*array)[i]);
	}
    }
    free(*array);
}

void menu(char ***array, int *wordCount, int *capacity)
{
    int quit;
    char operation;
    int ch;
    char word[WORD_LENGTH];

    quit = 0;
    do
    {
	printf("\nMenu\n\n");
	printf("Press 'S' for Search\nPress 'I' for Insert\nPress 'R' for Remove\nPress 'C' for Count\nPress 'Q' for Quit\n");
	fscanf(stdin, " ");
	fscanf(stdin, "%s", &operation);

	if (operation == 's' || operation == 'S')
	{
	    printf("\nSearch\n");
	    searchWord(array, wordCount);
	}

	else if (operation == 'i' || operation == 'I')
	{
	    printf("\nInsert\n");
	    printf("\nEnter a word: ");
	    scanf("%s", word);
	    insertWord(array, wordCount, word, capacity);
	}

	else if (operation == 'r' || operation == 'R')
	{
	    printf("\nRemove\n");
	    removeWord(*array, wordCount);
	}

	else if (operation == 'c' || operation == 'C')
	{
	    printf("\nCount\n");
	    printCount(*wordCount);
    	}

	else if (operation == 'q' || operation == 'Q')
	{
	    quit = 1;
	}

	else
	{
	    printf("\nInvalid Entry\n");
	}

	/* Flush buffer to prevent mishaps*/
	while ((ch = getchar()) != '\n' && ch != EOF);
	/*printArray(*array, *wordCount);*/
	
    } while (quit == 0);
}		
	
int searchWord(char ***array, int *count) {

  int i;
  int flag;
  char word[WORD_LENGTH];

  printf("\nEnter a word: ");
  scanf("%s", word);

  flag = 0;
  if (word == NULL) 
  {
    printf("\nInvalid Entry, returning to main menu\n");
  } 
  else 
  {
    for (i = 0; i < *count; i++) 
    {
      /* printf("\nChecking %s at \t%d", array[i], i); */
      if ((*array)[i] != NULL && strcmp((*array)[i], word) == 0) 
      {
        flag = 1;
        printf("\n'%s' found in array", word);
      }
    }
    if (flag == 0)
      printf("\n'%s' not found in array", word);
  }
  return 0;
}

int insertWord(char ***array, int *count, char *word, int *capacity)
{
    int i;
    char *wordPtr;
    int index;
    int found = 0;

    wordPtr = (char *)malloc((strlen(word) + 1) * sizeof(char));
 
    if (wordPtr == NULL)
    {
	fprintf(stderr,"    Malloc of array[%d] failed!\n", *count);
	return -1;
    }
    /* Memory for this word has been allocated, so copy characters
       and insert into array */

    strcpy(wordPtr, word);
	
    index = binarySearch(*array, *count, wordPtr, &found);

    if(found == 1)
    {
	printf("Word '%s' already exists, cannot enter it into the dictionary\n", word);
	free(wordPtr);
	return 1;
    }
    
    if (*count + 1 >= *capacity)
    { 
	doubleArraySize(array, capacity);

    }

    if(*count == 0)
    {
	*(*array) = wordPtr;
    }
 
    else
    {
	for( i = *count -1; i >= index; i--)
	{
	    (*array)[i+1] = (*array)[i];
	}

	(*array)[index] = wordPtr;
    }
   
    (*count)++;
    return 0;
}

int removeWord(char **array, int *count) 
{
    int index;
    char remove[WORD_LENGTH];
    int found = 0;


    printf("\nEnter a word: ");
    scanf("%s", remove);

    index = binarySearch(array, *count, remove, &found);

    if(found == 0)
    {
	printf("Can't find word");
    }
    else
    {
	int i;
	free(array[index]);
	for(i = index + 1; i < (*count); i++)
	{
	    array[i-1] = array[i];
	}
	(*count)--;
	printf("%s removed.", remove);
    }
    return 0;
}

int printCount(int count) 
{
    printf("Total words in dictionary: %d\n", count);
    return 0;
}

int saveDictionary (char *outFileName, char ***array, int *count) 
{

  int i;
  FILE *outFile;
  if ((outFile = fopen(outFileName, "w")) == NULL)
  {
    fprintf(stderr,"Error opening input file, %s\n", outFileName);
    return -1;
  }

  for (i = 0; i < *count; i++)
  {
    if ((*array)[i] != NULL && (fputs((*array)[i], outFile) == EOF || fputs("\n", outFile) == EOF))
    {
      return -1;
    }
  }

  if (fclose(outFile) == EOF) return -1;

  return 0;
}

void printArray(char **array, int count)
{
   int i=0;

   printf("\n");
   do
   {
	printf("array[%d]: %s\n", i, array[i]);
   } while (++i < count);
}

int binarySearch(char **array, int count, char *word, int *found)
{
    int mid;
    int low = 0;
    int high = count - 1;
    while(low <= high)
    {
	mid = (low + high) / 2;

	if(strcmp(word, array[mid]) > 0)
	{	
	    low = mid + 1;
	}
	else if(strcmp(word, array[mid]) < 0)
	{
	    high = mid - 1;
	}
	else
	{
	    /*printf("array[mid]: %s\n", array[mid]);*/
	    *found = 1;
	    return mid;
	}
    }
    return (low);
}
