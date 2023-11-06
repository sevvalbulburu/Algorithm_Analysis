#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printIntMatrix(int** mtrx, int N)
{
	int i, j;
	printf("\nInteger Oyun tahtasi:\n");
	for(i = 0; i < N; i++) 
	{
		for(j = 0; j < N; j++) 
			printf("%d\t", mtrx[i][j]);
		printf("\n"); 
	}
}

void printStrMatrix(char** mtrx, int N)
{
	int i, j, k = 0;
	printf("\nOyun tahtasi:\n");
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) 
		{
			printf("%s\t", mtrx[k]);
			k++;
		}
		printf("\n");
	}
}

// Girilen stringteki renkler convert islemi icin colorMtrx dizisinde tutulur.
char** colorsMatrix(char** mtrx, int N)
{
	char** colorMtrx;
	int i;
	// char matrisindeki renkler icin tmp dizi
	colorMtrx = (char**)malloc(N * sizeof(char*));
	for(i = 0; i < N; i++) 
		colorMtrx[i] = (char*)malloc(N * sizeof(char));
		
	for(i = 0; i < N; i++) 
		strcpy(colorMtrx[i], mtrx[i]);

	return colorMtrx;
}

// String matrisi Integer matrisine cevirir
int** convertStrToInt(char** mtrx, char** colorMtrx, int N)
{
	int i, j, k, l, match = 0;
	int** intMtrx;

	// int matris	
	intMtrx = (int**)malloc(N * sizeof(int*));
	for(i = 0; i < N; i++)
		intMtrx[i] = (int*)malloc(N * sizeof(int));

	// string dizisinini integer dizisine cevrilmesi
	k = 0;
	for(i = 0; i < N ; i++) 
	{
		for(j = 0; j < N; j++) 
		{
			l = 0;
			match = 0;
			while(match == 0 && l < N) 
			{
				if(strcmp(colorMtrx[l], mtrx[k]) == 0) 
					match = 1;
				l++;
			}
			intMtrx[i][j] = l;
			k++;
		}
	}
	
	return intMtrx;
}

// Integer matrisi String matrisine cevirir
void convertIntToStr(int** mtrx, char** colorMtrx, char** result, int N)
{
	int i, j, k = 0;
	
	for(i = 0; i < N ; i++) 
	{
		for(j = 0; j < N; j++) 
		{
			strcpy(result[k], colorMtrx[mtrx[i][j] - 1]);
			k++;
		}
	}
	
//	return result;
}

char** getMtrx(int N)
{
	int i, j;
	char** mtrx;
	int dimension = N * N;
	mtrx = (char**)malloc(dimension * sizeof(char*));
	for(i = 0; i < dimension; i++) 
		mtrx[i] = (char*)malloc(N * sizeof(char));
		
	printf("Oyun tahtasini giriniz:\n");
	for(i = 0; i < dimension; i++) 
		scanf("%s", mtrx[i]);

	return mtrx;
}

int** shiftMatrix(int** mtrx, int N, int row, int mode)
{
	int i, j, tmp;
	tmp = mtrx[row][N - 1];
	for(i = N - 1; i > 0; i--) {
		mtrx[row][i] = mtrx[row][i- 1];
	}
	mtrx[row][0] = tmp;
	if(mode == 2)
		printIntMatrix(mtrx, N);
	
	return mtrx;
}

int check(int** mtrx, int N, int row, int *shifts, int mode)
{
	int i,j, match = 0;
	//Durma Kosulu -> matris N. kez shift edildiginde ilk durumuna gelir. 
	//Ilgili satir bir adim kaydirilir. Onceki satirlarla eslesen eleman varsa islem tekrarlanir.
	if(row == 0) 
	{
		shifts[row] ++;
		shiftMatrix(mtrx, N, row, mode);
		if(mode == 2)
			printf("row: %d shift: %d \n", row, shifts[row]);
		return 1;
	}
	if(shifts[row] < N - 1) 
	{
		shifts[row] ++;
		shiftMatrix(mtrx, N, row, mode);
		
		i = 0;
		while(i < row && match == 0) {
			j = 0;
			while(j < N && match == 0) {
				if(mtrx[row][j] == mtrx[i][j])
					match = 1;
				else
					j++;
			}
			i++;
		}
		if(mode == 2)
			printf("row: %d shift: %d match: %d\n", row, shifts[row], match);
		if(match == 1){
			check(mtrx, N, row, shifts, mode);
		}
		else
			return 1;
	}
	else 
		return 0;
}

int organizeMatrix(int** mtrx, int N, int mode, int row, int* shifts, int flag)
{
	// Durma Kosullari -> satirlarin sifirdan üste cikmasi veya N - 1 den asagi inmesi
	if(row == -1)
		return 0;
	
	if(row  == N)
		return 1;
	// flag -> 1 bir satir asagi inmeli flag -> 0 backtraking	
	if(flag == 1) 
	{
		row++;
		if(row != N)
			flag = check(mtrx, N, row, shifts, mode);
	}

	else 
	{
		shifts[row] = 0;
		row--;
		while (row >= 0 && shifts[row] == N-1)
		{
			shifts[row] = 0;
			row--;
		}
		flag = check(mtrx, N, row, shifts, mode);
	}
	
	return organizeMatrix(mtrx, N, mode, row, shifts, flag);

}

void normalMode(int** intMtrx, char** strMtrx, int N) 
{
	int result;
	int *shifts = (int *) calloc(N, sizeof(int));
		
	strMtrx = getMtrx(N);
	printStrMatrix(strMtrx, N);
	intMtrx = convertStrToInt(strMtrx, colorsMatrix(strMtrx, N), N);
	printIntMatrix(intMtrx, N);
	result = organizeMatrix(intMtrx, N, 1, 0, shifts, 1);
	if(result == 1)
	{
		printf("\nCozumlenmis Oyun Tahtasi:\n");
		printIntMatrix(intMtrx, N);
		convertIntToStr(intMtrx, colorsMatrix(strMtrx, N), strMtrx, N);
		printStrMatrix(strMtrx, N);
	}
	else
		printf("\nGirilen Oyun Tahtasi Icin Cozum Yok.\n");	
	free(shifts);
}

void detailedMode(int** intMtrx, char** strMtrx, int N) 
{
	int result;
	int *shifts = (int *) calloc(N, sizeof(int));
	strMtrx = getMtrx(N);
	printStrMatrix(strMtrx, N);
	intMtrx = convertStrToInt(strMtrx, colorsMatrix(strMtrx, N), N);
	printIntMatrix(intMtrx, N);
	result = organizeMatrix(intMtrx, N, 2, 0, shifts, 1);
	if(result == 1)
	{
		printf("\nCozumlenmis Oyun Tahtasi:\n");
		printIntMatrix(intMtrx, N);
		convertIntToStr(intMtrx, colorsMatrix(strMtrx, N), strMtrx, N);
		printStrMatrix(strMtrx, N);
	}
	else
		printf("\nGirilen Oyun Tahtasi Icin Cozum Yok.\n");	
	free(shifts);
}

int main() 
{
	int N, mode, flag = 1;;
	char** strMtrx, colorMtrx;
	int** intMtrx;

	
	while(flag != 0) 
	{
		printf("\nMode (Normal Mod -> 1 Detay Mod -> 2):\n");
		scanf("%d", &mode);
		
		if(mode == 1)
		{
			printf("Oyun tahtasinin boyutunu giriniz:\n");
			scanf("%d", &N);
			normalMode(intMtrx, strMtrx, N);
		}
		else if(mode == 2) 
		{
			printf("Oyun tahtasinin boyutunu giriniz:\n");
			scanf("%d", &N);
			detailedMode(intMtrx, strMtrx, N);

		}
		else
			flag = 0;
	}

	return 0;
}
/*
3*3
mavi
sari
mor
sari
mor
mavi
mavi
mor
sari

4*4
mavi
sari
mor
kirmizi
sari
kirmizi
mor
mavi
mavi
mor
sari
kirmizi
kirmizi
mavi
mor
sari

4*4
mavi
sari
mor
kirmizi
mavi
sari
mor
kirmizi
mavi
sari
mor
kirmizi
mavi
sari
mor
kirmizi


5*5
mavi
sari
mor
kirmizi
pembe
mavi
sari
pembe
mor
kirmizi
pembe
mavi
sari
mor
kirmizi
mavi
sari
mor
pembe
kirmizi
mavi
sari
mor
kirmizi
pembe
*/

