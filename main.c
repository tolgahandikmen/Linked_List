#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Word {
    char word[50];
    int wordCount;
    struct Word * next;
    struct RelatedWord * relatedWord;
} Word;


typedef struct RelatedWord {
    char word[50];
    int wordCount;
    struct RelatedWord * next;
} RelatedWord;

typedef struct OriginalText {
    char word[50];
    struct OriginalText * next;
} OriginalText;

typedef struct Header {
    char *fileName;
    Word * fileHeader;
    int control;
} Header;


/* Function to read a given file and  create to linked list */
Word * readInputFileToCreateLinkList (char *fileName, Word*, OriginalText *);

/* Function to add an element to linked list */
Word * addWord (Word *, char word[50], OriginalText *,int *readWordSize);

/* Function to add to related words of a word to the linked list */
RelatedWord * addRelatedWord (RelatedWord *, char word[50]);

/* Function to create a linked list which keeps all file sequentially */
OriginalText* addOriginalWord (OriginalText* , char word[50]);

/* Function to bubble sort the given linked list */
Word * bubbleSort (Word *start);
 
/* Function to swap data of two nodes a and b */
void swap (Word *a, Word *b);

/* Function to print most 3 word pairs */
void mostPairs (Word *);

/* Function to calculate cosine similarity of given linked lists */
double similarity (Word *, Word *);

/* strlwr() function is not allowed in ansi */
char* strlwr(char* s);

OriginalText* addOriginalWord (OriginalText* origin, char word[50]){
	OriginalText *temp_originalText = (OriginalText*)malloc(sizeof(OriginalText));
	strcpy(temp_originalText->word, word);
	temp_originalText->next = NULL;
	if(origin != NULL) temp_originalText->next = origin;		
	origin = temp_originalText;	
	return origin;	
}

/* Adding new word to linked list */
Word * addWord (Word *head, char word[50], OriginalText *origin, int* readWordSize) {
	Word *temp2 = (Word*)malloc(sizeof(Word));
	temp2 = head;
	int control_flag = 0;
	int control2 = 0;
	
	
	/* Check if the added element(word) is already in the linked list */
	if (temp2 != NULL) {
		while ( temp2 != NULL ) {
			/* If it exists, only increase the word counter */
			if ( !strcmp(temp2->word, word) ) {	
				temp2->wordCount++;	
				control_flag = 1;			
				break;
			}
			temp2 = temp2->next;
		}		
	}
	
	if(!control_flag){
		Word *temp = (Word*)malloc(sizeof(Word));
		strcpy(temp->word, word);
		temp->wordCount = 1;
		temp->next = NULL;
		temp->relatedWord = NULL;
		
		if(head != NULL) temp->next = head;		
		head = temp;		
		
		control2 = 1;
	}

	Word *temp1 = head;
	OriginalText *tempOri = origin;
	
	if (readWordSize){
			tempOri = tempOri->next;
			while (temp1 != NULL){		
				if ( !strcmp(temp1->word, tempOri->word) ) {
					temp1->relatedWord = addRelatedWord(temp1->relatedWord, word);
					break;
				}
		
				temp1 = temp1->next;
			}				
	} 
	return head;
}

/* Adding new related word to the word in the linked list */
RelatedWord * addRelatedWord (RelatedWord *baseWord, char word[50]) {	
	RelatedWord *temp2 = baseWord;
	
	/* Check if the added element(next word) is already in the linked list */
	while ( temp2 != NULL ) {
		if ( !strcmp(temp2->word, word) ) {				
			temp2->wordCount++;
			return baseWord;
		}
		temp2 = temp2->next;
	}	

	RelatedWord *temp = (RelatedWord*)malloc(sizeof(RelatedWord));	
	strcpy(temp->word, word);
	temp->wordCount = 1;
	temp->next = NULL;
	if(baseWord != NULL) temp->next = baseWord;		
	baseWord = temp;

	return baseWord;
}

/* Deleting the taken word from the linked list */
Word *  deleteWord (Word *head, char word[50]) {
	Word *temp = head;
	
	if (!strcmp(head->word, word)) {
		head = temp->next;
		free(temp);
		return head;
	}
	
	Word *temp2 = temp->next;	
	while (temp2 != NULL){
		if (!strcmp(temp2->word, word)) {
			break;
		}
		temp2=temp2->next;
		temp=temp->next;
	}
	temp->next = temp2->next;
	free(temp2);
	return head;
}

/* Printing the linked list for test steps */
void printLinkedList (Word *head) {
	Word *temp = (Word*)malloc(sizeof(Word));
	RelatedWord *temp2 = (RelatedWord*)malloc(sizeof(RelatedWord));
	temp2= NULL;
	temp = head;
	printf("List is: \n");
	while (temp != NULL) {
		temp2 = temp->relatedWord;
		printf (" %s", temp->word);
		printf ("%d", temp->wordCount);
		printf("{ ");
		while (temp2 != NULL) {
			printf (" %s", temp2->word);
			printf ("%d", temp2->wordCount);
			temp2 = temp2->next;
		}
		printf("} \n");
		
		temp = temp->next;
	}
	printf ("\n");
}

double similarity (Word * h1, Word * h2){
	Word *temp1 = h1;
	Word *temp2 = h2;
	char similarWords[20][50];
	int matrix[2][20];
	int i=0;
	int j=0;
	int control=0;
	int k,t;
	int similarCounter=0;
	double dotProduct = 0;
	double vectProduct = 0;
	double x,y;
	double result = 0;

	for(i=0; i<2; i++){
		for(j=0;j<20;j++){
			matrix[i][j] = 0;
		}
	}
	
	i=0;
	while ( temp1!=NULL && i<10 ){
		matrix[0][i] = temp1->wordCount;
		strcpy (similarWords[similarCounter],temp1->word);
		similarCounter++;
		temp2 = h2;		
		while (temp2 != NULL){
			matrix[1][i] = 0;
			if (!strcmp(temp1->word,temp2->word)){
				matrix[1][i] = temp2->wordCount;
				break;
			}
			temp2 = temp2->next;
		}
		i++;
		temp1 = temp1->next;
	}
	
	temp2 = h2;
	t=0;
	while ( temp2!=NULL && t<10 ){
		control = 0;
		for (k=0; k<similarCounter; k++){
			if (!strcmp(similarWords[k],temp2->word)){
				control = 1;
			}	
		}
		
		if(!control) {
			matrix[1][i] = temp2->wordCount;
			similarCounter++;
			temp1 = h1;		
			while (temp1 != NULL){
				matrix[0][i] = 0;
				if (!strcmp(temp2->word,temp1->word)){
					matrix[0][i] = temp1->wordCount;
					break;
				}
				temp1 = temp1->next;
			}
			i++;	
		}
		t++;
		temp2 = temp2->next;
	}
	
	
	/* dotProduct calculation */
	x = 0;
	for (i=0; i<20; i++){
		x = matrix[0][i] * matrix[1][i];
		dotProduct += x;
	}
	
	/* vectProduct calculation */
	for (i=0; i<20; i++){
		x = pow(matrix[0][i],2);
		vectProduct += x;
	}
	vectProduct = sqrt(vectProduct);
	
	y = 0;
	for (i=0; i<20; i++){
		x = pow(matrix[1][i],2);
		y += x;
	}
	y = sqrt(y);
	
	vectProduct *= y;
	
	result = dotProduct / vectProduct;

	printf("similarCounter: %d \n",similarCounter);
	return result;
}

int main( )
{
	int i = 0;
	int j = 0;
	char *inputFile, *secondInputFile;
	char command[1000];
	char *pch;
	char *word =  malloc (sizeof(char)* 50);
	double similarityFloat = 0;
	Word * head = NULL;
	OriginalText * origin = NULL;
	
	Header headers[100];
	for (i=0; i<100; i++){
		headers[i].fileName = malloc (sizeof(char)*500);
		headers[i].control=0;
	}
	
	/* the program continue until get '-q' value as command */
	while ( strcmp(gets(command), "-q") ) {
		/* gets() function takes command from user as a line */
		head = NULL;
		origin = NULL;

		/* strtok() parses taken command line according to whitespace  */
		pch = strtok (command, " ");
		while (pch != NULL) {			
		
			/* if command starts with '-r' command */
			if ( !strcmp (pch, "-r") ) {
				pch = strtok (NULL, " ");
				if ( !pch ){
					printf("input is not correct!\n"); 
					continue;
				}
				else {
					inputFile = pch;
					head = readInputFileToCreateLinkList (inputFile, head, origin);
					if (head!=NULL)	
						head = bubbleSort(head);
					
					i=0;
					while (headers[i].control){
						i++;
					}
					headers[i].control =1;
					headers[i].fileHeader = head;
					strcpy (headers[i].fileName, pch); 
				}				
			}
			
			/* if command starts with '-a' command */
			else if ( !strcmp (pch, "-a") ){
				int value;
				
				pch = strtok (NULL, " ");
				if ( !pch ){
					printf("input is not correct!\n"); 
					continue;
				}
				else {
					strcpy(word, pch);
/*					printf("eklenecek kelime> %s\n", pch); */
					pch = strtok (NULL, " ");
					if ( !pch ){
/*						printf("dosya degeri girilmedi tekrar komut giriniz!\n"); */
					}
					else {
						value = atoi(pch);
						pch = strtok (NULL, " ");
						word = strtok (word, " ,.;:?!");
						word = strlwr (word);
						
						i=0;
						while( strcmp (headers[i].fileName, pch)){
							i++;
						}
						for (j=0; j<value; j++) {
							headers[i].fileHeader = addWord(headers[i].fileHeader, word, origin, 0);	
						}
						headers[i].fileHeader = bubbleSort(headers[i].fileHeader);
					}																
				}								
			}

			/* if command starts with '-n2' command */
			else if ( !strcmp (pch, "-n2") ){
				pch = strtok (NULL, " ");
				if ( !pch ){
					printf("input is not correct!\n"); 
					continue;
				}
				else {
					inputFile = pch;
					i=0;
					while ( strcmp(headers[i].fileName, pch)){
						i++;
					}
					printf("The most recent 3 pair of words of %s:\n", pch);
					mostPairs(headers[i].fileHeader);					
				}								
			}
			
			/* if command starts with '-d' command */
			else if ( !strcmp (pch, "-d") ){
				pch = strtok (NULL, " ");
				if ( !pch ){
					printf("input is not correct!\n"); 
					continue;
				}
				else {
					strcpy(word, pch);
					pch = strtok (NULL, " ");
					if ( !pch ){
						printf("input is not correct!\n"); 
						continue;
					}
					else {
						inputFile = pch;
						word = strtok (word, " ,.;:?!");
						word = strlwr (word);

						i=0;
						while( strcmp(headers[i].fileName, pch)){
							i++;
						}
						headers[i].fileHeader = deleteWord(headers[i].fileHeader, word);
					}																
				}												
			}

			/* if command starts with '-s' command */
			else if ( !strcmp (pch, "-s") ){
				pch = strtok (NULL, " ");
				if ( !pch ){
					printf("input is not correct!\n"); 
					continue;
				}
				else {
					inputFile = pch;
					pch = strtok (NULL, " ");
					if ( !pch ){
						printf("input is not correct!\n"); 
						continue;
					}
					else {
						secondInputFile = pch;
						
						i=0;
						while( strcmp(headers[i].fileName, inputFile)){
							i++;
						}
						j=0;
						while( strcmp(headers[j].fileName, secondInputFile)){
							j++;
						}
						
						similarityFloat = similarity(headers[i].fileHeader, headers[j].fileHeader);
						printf("Cosine Similarity of %s and %s is %.3f \n",inputFile, secondInputFile, similarityFloat);
					}
				}
			}

			else {
				printf("Wrong command\n ");
			}
			pch = strtok (NULL, "");
		}
		printf("\n");
	}
/*	else -> system will be closed! */
	return 0;
}

Word* readInputFileToCreateLinkList (char *fileName, Word * head, OriginalText * origin) {
	FILE *inputFile;
	char word[200];
	char *ptr;
	char *temp, *temp2, *token;
	int i = 0;
	int j = 0;
	int *readWordSize = 0;
	
    inputFile = fopen(fileName, "r");
	
    if (inputFile == 0)
    {
        /* fopen returns 0, the NULL pointer, on failure */
		perror("Can not open input file");
        return head;
    }
    else 
    {
    	/* reading input file according to taken parameter */        
        while ( fscanf(inputFile,"%s", &word) != EOF) {
			
			/* strlwr() converts word to lowercase */
			ptr = strlwr(word);
        	ptr = strtok (ptr, " ,.;:?!");
        	
        	/* If word has paranthesis after parsing word according to the tokens, continue */
        	
			/* deleting from word to left paranthesis or right paranthesis or "-'" */
        	if (strstr (ptr, "(") || strstr (ptr, ")") || strstr (ptr, "-") || strstr (ptr, "'")) {
        		token = strtok(ptr, "()'- ");
				while( token != NULL ) {
					origin = addOriginalWord(origin, token);
					head = addWord(head,token, origin, readWordSize);
					readWordSize++;
					
					token = strtok (NULL, "()'- ");					
					j++;			
				}		
				continue;
			}
			
			origin = addOriginalWord(origin, ptr);
			head = addWord(head, ptr, origin, readWordSize);
			readWordSize++;
        	j++;
		}
    }
	fclose(inputFile);	
	return head;
}

/* Bubble sort the given linked list */
Word * bubbleSort(Word *start) {
    int swapped, i;
    Word *ptr1;
    Word *ptr2;
    Word *lptr = NULL;
 
    /* Checking for empty list */
    if (ptr1 == NULL)
        return start;
        
    do
    {
        swapped = 0;
        ptr1 = start;
 
        while (ptr1->next != lptr) {
        	ptr2 = ptr1->next;
            if (ptr1->wordCount < ptr2->wordCount) { 
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
    
    return start;
}
 
/* function to swap data of two nodes a and b */
void swap(Word *a, Word *b) {
	Word *temp = (Word*)malloc(sizeof(Word));
	
	strcpy (temp->word, a->word);
	temp->wordCount = a->wordCount;
	temp->relatedWord = a->relatedWord;
	
	strcpy (a->word, b->word);
	a->wordCount = b->wordCount;
	a->relatedWord = b->relatedWord;
	
	strcpy (b->word, temp->word);
	b->wordCount = temp->wordCount;
	b->relatedWord = temp->relatedWord;
	
	free (temp);
	return;
}

void mostPairs (Word * head){
	Word *temp = head;
	RelatedWord *rw;
	int most1, most2, most3;
	char mchar1[100];	
	char mchar2[100];
	char mchar3[100];
	most1=0;
	most2=0;
	most3=0;
	
    /* Checking for empty list */
    if (temp == NULL){
		printf("Error! Can not write most word pairs \n");   
        return ;
	}
 	
	while (temp->next != NULL) {
		rw = temp->relatedWord;
		while (rw != NULL) {
			if (rw->wordCount > most1) {
				strcpy (mchar3, mchar2);
				strcpy (mchar2, mchar1);
				strcpy (mchar1, temp->word);
				strcat (mchar1, " ");
				strcat (mchar1, rw->word);				
				
				most3 = most2;
				most2 = most1;
				most1 = rw->wordCount;	
			}
			else if (rw->wordCount > most2) {
				strcpy (mchar3, mchar2);		
				strcpy (mchar2, temp->word);
				strcat (mchar2, " ");
				strcat (mchar2, rw->word);
				
				most3 = most2;
				most2 = rw->wordCount;
			}
			else if (rw->wordCount > most3) {
				strcpy (mchar3, temp->word);
				strcat (mchar3, " ");
				strcat (mchar3, rw->word);				
				
				most3 = rw->wordCount;
			}
			
			rw = rw->next;	
		}
		temp = temp->next; 	
	}
	
	printf("%s (%d)\n%s (%d)\n%s (%d)\n",mchar1,most1, mchar2,most2, mchar3,most3);
	return ;
}

char* strlwr(char* s) {
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = tolower((unsigned char) *tmp);
    }

    return s;
}
