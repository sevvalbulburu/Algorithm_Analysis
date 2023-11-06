#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING 100

typedef struct data{
	char key[STRING];
	int link_count;
	char link[STRING][STRING];
	int insertnum;
}data;

typedef struct hashtable{
	data *element;
}hashTable;

void initializeHashTable(int hashSize, hashTable *h){
	int i;
	h->element = (data*)malloc(hashSize * sizeof(data));
	for(i = 0; i < hashSize; i++) {
		strcpy(h->element[i].key, "Empty");
		h->element[i].link_count = 0;
		h->element[i].insertnum = 0;
	}
}


int hornerKeyNum(char* key, int size) { // Horner Method
	int i, tmp;
	unsigned int hashkey = 0;
	for(i = 0; i < strlen(key); i++) {
		if (key[i < 'Z'])
			tmp = key[i] - 'A' + 27;
		else
			tmp = key[i] - 'a';
		hashkey = 31 * hashkey + tmp;
	}
	return hashkey % size;
}

char** searchElement(int size, hashTable *h, char *key, int* num) {
	int indx = hornerKeyNum(key, size);
	int last_check = (indx+size-1)%size, i;
// searching
	while( strcmp(h->element[indx].key, key) != 0 && indx != last_check){
		indx = (indx + 1) % size;
	}
	*num = h->element[indx].link_count;
// when key is one above
	if (strcmp(h->element[indx].key, key) != 0)
	{
		//printf("Not Found");
		return NULL;
	}
	char** r = (char**)malloc((*num)*sizeof(char*));
	for(i=0;i<*num;i++)
	{
		r[i] = (char*) malloc(STRING);
		strcpy(r[i], h->element[indx].link[i]);
	}
	return r;
}

void insertElement(int size, hashTable *h, char *key, char *link) {
	int indx = hornerKeyNum(key, size);
	int insertnum = 0;
// finding right place
	
	while( strcmp(h->element[indx].key, "Empty") != 0 && strcmp(h->element[indx].key, key) != 0){
		indx = (indx + 1) % size;
		insertnum++;
	}
// inserting
	h->element[indx].insertnum = insertnum;
	strcpy(h->element[indx].key, key);
	strcpy(h->element[indx].link[h->element[indx].link_count++], link);
}
// finds closest prime number to given size for hash table
int primeNum(int n) {
	int flag1 = 1, flag2;
	int i;
	if(n <= 1)
		return 1;
	while(flag1 == 1) {
		i = 2;
		flag2 = 1;
		while(flag2 == 1 && i < n) {
			if(n % i == 0)
				flag2 = 0;
			else
				i++;
		}
		if(flag2 == 0) 
			n++;
		else 
			flag1 = 0;
	}
	return n;	
}

int sizeOfHashTable(int N, float load_factor) { // calculates the size of hash table
	int hash_size;
	hash_size = (N *1.0) / load_factor;
	return primeNum(hash_size);
}

int numberOfUniqueKeys(char *keys[], int size) { // finds number of same keys and extract it from the key size;
	int i,j, samekey = 0;
    //printf(" %d ",(sizeof(arr) / sizeof(int)));
    for(i = 0; i < size; i++) {
        j = i + 1;
        while(strcmp(keys[i], keys[j] )!= 0 && j < size)
            j++;
        if(j < size)
            samekey++;
    }
    return (size - samekey);
}

int readFile(char *file_name, float load_factor, hashTable *h) {
	FILE *ptr;
	char **key, link[STRING][STRING], buff[255];
	char *tmp, *token, *tmpl, *line;
	int i = 0, j = 0,k;
	int N, hash_size; // number of unique keys and hash table's size
	key = (char**)malloc(STRING*sizeof(char*));
	for ( j = 0; j < STRING; j++)
        key[j] = (char*) malloc(STRING*sizeof(char));
 	tmpl = (char*)malloc(STRING*sizeof(char));
	tmp = (char*)malloc(5*sizeof(char));

	ptr = fopen(file_name, "r");
	if(ptr == NULL) {
		printf("File could not opened...\n");
		exit(0);
	}
	printf("File opened...\n");

	
  	while(fgets(buff, 255, ptr) != NULL) {
		if (buff[strlen(buff)-1] == '\n')
			buff[strlen(buff)-1] = '\0';
	  	strncpy(tmp, buff, 5);
		if(strcmp("https", tmp) == 0) {
			strcpy(tmpl, buff);
		}
		else {
			token = strtok(buff, " ");
			while(token != NULL) {		
				strcpy(key[i], token);
				strcpy(link[i], tmpl);
		    	//printf("%s %s\n", key[i], link[i]);
				i++;			
				token = strtok(NULL, " ");
			}
		}
    }
    //for(k = 0; k < i; k++) 
	//	printf("%s\n", key[k]);

    //defining hash table's size
    N = numberOfUniqueKeys(key, i);
    hash_size = sizeOfHashTable(N, load_factor);  
    //create hash table
    initializeHashTable(hash_size, h);
    
    for(k = 0; k < i; k++)
    {
		insertElement(hash_size, h, key[k], link[k]);
	}

    fclose(ptr);
    return hash_size;
}

char** unionLinks(char **links1, char **links2, hashTable *h, int size1, int size2) {
	int i, j;
	char *tmp;
	int lsize = size1 + size2;
	char **links;
	links = (char**)malloc(lsize * sizeof(char*));
	for ( j = 0; j < lsize; j++)
		links[j] = (char*) malloc(STRING*sizeof(char));
	
	//add all links to links array
	for(i = 0; i < size1; i++) {
		strcpy(links[i],links1[i]);
	}
	for(j = 0; j < size2; j++) {
		strcpy(links[i + j], links2[j]);
	}

	for(i = 0; i < lsize; i++) {
		j = i + 1;
		
		while(j < lsize && strcmp(links[i], links[j]) != 0) {
			j++;
		}
		if(j < lsize){
			strcpy(links[j], "Empty");
		}
	}
	
	return links;
}

char** intersectionLinks(char **links1, char **links2, hashTable *h, int size1, int size2) {
	int i, j;
	char *tmp;
	int lsize = size1 + size2;
	char **links;
	links = (char**)malloc(lsize * sizeof(char*));
	for ( j = 0; j < lsize; j++)
		links[j] = (char*) malloc(STRING*sizeof(char));
		
	//add all links to links array
	for(i = 0; i < size1; i++) {
		strcpy(links[i],links1[i]);
	}
	for(j = 0; j < size2; j++) {
		strcpy(links[i + j], links2[j]);
	}

	for(i = 0; i < lsize; i++) {
		j = i + 1;
		while(j < lsize && strcmp(links[j], links[i]) != 0) {
			j++;
		}
		if(j < lsize){
				strcpy(links[j], "Empty");
		}
		else{
			strcpy(links[i], "Empty");
		}
	}

	return links;
}

int main() {
	char *file_name = "Sample.txt", **link1, **link2, **keys, *key;
	int i,num, num1, num2,j, flag = 1, wordnum, mod;
	float load_fac;
	hashTable h;


	while(flag == 1) {
		printf("Mod seciniz 0: Normal Mod, 1: Detay Mod\n");
		scanf("%d", &mod);
		printf("\nCikmak icin 0'a basiniz.\n'Kac kelime gireceginizi yaziniz(en fazla iki kelime girilebilir.)\n 'VE'-'VEYA' kullaniniz.\n");
		scanf("%d", &wordnum);

		// normal mod:
		if(mod == 0) {
		 	
			if(wordnum == 0) {
				printf("Program sonlandirildi.\n");
				flag = 0;
			}
			else if(wordnum == 1 ) {
				
				printf("Load factor:\n");
				scanf("%f", &load_fac);
				
				int size = readFile(file_name, load_fac, &h);
				
				printf("Kelime giriniz.\n");
				key = (char*) malloc(STRING);
				scanf("%s", key);
				
				link1 = searchElement(size, &h, key, &num);
				if (link1 == NULL)
					printf("Aradiginiz kelime bulunamadi.\n");
				else{
					printf("Kelimeyi iceren linkler:\n");
					for(i=0; i < num; i++)
						printf("%s ", link1[i]);
					printf("\n");
				}
			}
			else if(wordnum == 2) {
				printf("Load factor:\n");
				scanf("%f", &load_fac);
				
				int size = readFile(file_name, load_fac, &h);
				
				keys = (char**)malloc((wordnum + 1)*sizeof(char*));
				for ( j = 0; j < (wordnum + 1); j++)
		        	keys[j] = (char*) malloc(STRING*sizeof(char));
		        if(keys == NULL)
		        	printf("Not allocated\n");
		        	
		        printf("Kelimeleri giriniz.\n");
		        for(i = 0; i < wordnum + 1; i++) {
		        	scanf("%s", keys[i]);
				}
	
				// veya durumu
		        if(strcmp(keys[1], "ve") != 0) {
		        	link1 = searchElement(size, &h, keys[0], &num1);
		        	link2 = searchElement(size, &h, keys[2], &num2);
		        	if(link1 == NULL && link2 != NULL){	        		
		        		printf("Ilk kelime bulunamadi.\n");
		        		printf("Ikinci Kelimeyi iceren linkler:\n");
						for(i=0; i < num2; i++)
							printf("%s ", link2[i]);
						printf("\n");
					}
		        	else if(link1 != NULL && link2 == NULL){
		        		printf("Ikinci kelime bulunamadi.\n");
		        		printf("Ilk Kelimeyi iceren linkler:\n");
						for(i=0; i < num1; i++)
							printf("%s ", link1[i]);
						printf("\n");
					}
					else if(link1 == NULL && link2 == NULL)
						printf("Kelimeler bulunamadi.\n");
					else{					
						link1 = unionLinks(link1, link2, &h, num1, num2);
						printf("Kelimeleri iceren linkler:\n");
						for(i = 0; i < num1 + num2; i++) {
							if(strcmp(link1[i], "Empty") != 0)
								printf("%s\n", link1[i]);
						}
					}
				}
		        
		        else if(strcmp(keys[1], "ve") == 0) {
		        	link1 = searchElement(size, &h, keys[0], &num1);
		        	link2 = searchElement(size, &h, keys[2], &num2);
		        	if(link1 == NULL && link2 != NULL){	        		
		        		printf("Ilk kelime bulunamadi.\n");
		        		printf("Ikinci Kelimeyi iceren linkler:\n");
						for(i=0; i < num2; i++)
							printf("%s ", link2[i]);
						printf("\n");
					}
		        	else if(link1 != NULL && link2 == NULL){
		        		printf("Ikinci kelime bulunamadi.\n");
		        		printf("Ilk Kelimeyi iceren linkler:\n");
						for(i=0; i < num1; i++)
							printf("%s ", link1[i]);
						printf("\n");
					}
					else if(link1 == NULL && link2 == NULL)
						printf("Kelimeler bulunamadi.\n");
						
					else{
						link1 = intersectionLinks(link1, link2, &h, num1, num2);
						printf("Kelimelerin ikisini de iceren linkler:\n");
						for(i = 0; i < num1 + num2; i++) {
							if(strcmp(link1[i], "Empty") != 0)
								printf("%s\n", link1[i]);
						}	
					}
				}
	
			}
			else {
				printf("Fazla kelime sayisi girdiniz, tekrar deneyin.\n");
			}
		} 
		// Detay mod
		else if( mod == 1) {
		 	
			if(wordnum == 0) {
				printf("Program sonlandirildi.\n");
				flag = 0;
			}
			else if(wordnum == 1 ) {
				
				printf("Load factor:\n");
				scanf("%f", &load_fac);
				
				int size = readFile(file_name, load_fac, &h);
				
				
				printf("Kelime giriniz.\n");
				key = (char*) malloc(STRING);
				scanf("%s", key);
				
				link1 = searchElement(size, &h, key, &num);
				if (link1 == NULL)
				printf("Aradiginiz kelime bulunamadi.\n");
				else{
					printf("Kelimeyi iceren linkler:\n");
					for(i=0; i < num; i++)
						printf("%s ", link1[i]);
					printf("\n");
				}
				//table
				printf("\n\nTable Size: %d\n", size);
				printf("Hash Table:\n");
				for(i = 0; i < size; i++) {
					if(strcmp(h.element[i].key, "Empty") != 0){
						printf("Key: %s\nInsert Number: %d\nLinks \n", h.element[i].key, h.element[i].insertnum );
						for(j = 0; j < h.element[i].link_count; j++) {
							
								printf(" %s\n", h.element[i].link[j]);
						}
						
					}

				}				
			}
			else if(wordnum == 2) {
				printf("Load factor:\n");
				scanf("%f", &load_fac);
				
				int size = readFile(file_name, load_fac, &h);
				
				keys = (char**)malloc((wordnum + 1)*sizeof(char*));
				for ( j = 0; j < (wordnum + 1); j++)
		        	keys[j] = (char*) malloc(STRING*sizeof(char));
		        if(keys == NULL)
		        	printf("Not allocated\n");
		        	
		        printf("Kelimeleri giriniz.\n");
		        for(i = 0; i < wordnum + 1; i++) {
		        	scanf("%s", keys[i]);
				}
	
				// veya durumu
		        if(strcmp(keys[1], "ve") != 0) {
		        	link1 = searchElement(size, &h, keys[0], &num1);
		        	link2 = searchElement(size, &h, keys[2], &num2);
		        	if(link1 == NULL && link2 != NULL){	        		
		        		printf("Ilk kelime bulunamadi.\n");
		        		printf("Ikinci Kelimeyi iceren linkler:\n");
						for(i=0; i < num2; i++)
							printf("%s ", link2[i]);
						printf("\n");
					}
		        	else if(link1 != NULL && link2 == NULL){
		        		printf("Ikinci kelime bulunamadi.\n");
		        		printf("Ilk Kelimeyi iceren linkler:\n");
						for(i=0; i < num1; i++)
							printf("%s ", link1[i]);
						printf("\n");
					}
					else if(link1 == NULL && link2 == NULL)
						printf("Kelimeler bulunamadi.\n");
					else{
						
						link1 = unionLinks(link1, link2, &h, num1, num2);
						printf("Kelimeleri iceren linkler:\n");
						for(i = 0; i < num1 + num2; i++) {
							if(strcmp(link1[i], "Empty") != 0)
								printf("%s\n", link1[i]);
						}
					}
				}
		        
		        else if(strcmp(keys[1], "ve") == 0) {
		        	link1 = searchElement(size, &h, keys[0], &num1);
		        	link2 = searchElement(size, &h, keys[2], &num2);
		        	if(link1 == NULL && link2 != NULL){	        		
		        		printf("Ilk kelime bulunamadi.\n");
		        		printf("Ikinci Kelimeyi iceren linkler:\n");
						for(i=0; i < num2; i++)
							printf("%s ", link2[i]);
						printf("\n");
					}
		        	else if(link1 != NULL && link2 == NULL){
		        		printf("Ikinci kelime bulunamadi.\n");
		        		printf("Ilk Kelimeyi iceren linkler:\n");
						for(i=0; i < num1; i++)
							printf("%s ", link1[i]);
						printf("\n");
					}
					else if(link1 == NULL && link2 == NULL)
						printf("Kelimeler bulunamadi.\n");
						
					else{
						link1 = intersectionLinks(link1, link2, &h, num1, num2);
						printf("Kelimelerin ikisini de iceren linkler:\n");
						for(i = 0; i < num1 + num2; i++) {
							if(strcmp(link1[i], "Empty") != 0)
								printf("%s\n", link1[i]);
						}	
					}
				}
				//table
				printf("\n\nTable Size: %d\n", size);
				printf("Hash Table:\n");
				for(i = 0; i < size; i++) {
					if(strcmp(h.element[i].key, "Empty") != 0){
						printf("Key: %s\nInsert Number: %d\nLinks \n", h.element[i].key, h.element[i].insertnum );
						for(j = 0; j < h.element[i].link_count; j++) {
							
								printf(" %s\n", h.element[i].link[j]);
						}
						
					}

				}
	
			}
			else {
				printf("Fazla kelime sayisi girdiniz, tekrar deneyin.\n");
			}	
					
		}
	}
	

	
	return 0;
}
