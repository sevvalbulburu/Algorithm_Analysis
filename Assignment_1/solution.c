#include <stdio.h>
#include <stdlib.h>

int* BF(int* arr, int left, int right);
int* findMaxWithMiddle(int* arr, int left, int right);
int* findMaxWDiv(int* arr, int left, int right);

int main(){
    int i,N;
	//N = 12;
    //int arr[] = {-15, -39, -12, -6, -12, -8, -1, -11, -10, 0, -5, -3};
    N = 7;
    int arr[] = {10, -5, 13, -9, 12, -8, -1};
   // N = 5;
    //int arr[] = {-3, -5, -13, -9, -1};
    //N = 10;
    //int arr[] = {-30, 36, 2, -6, 52, 8, -1, -11, 10, 4};
    int *sonuc1, *sonuc2;   // toplam, start, stop_indx
    sonuc1 = findMaxWDiv(arr, 0, N-1);

    printf("DIZI: ");
    for ( i = 0; i < N; i++)
        printf("%d ", arr[i]);
        
    printf("\nRekursif Cozum:\n");
    printf("\n Baslangic indeksi:%d\n Bitis indeksi:%d\n Maximum Kazanc:%d\n", sonuc1[1], sonuc1[2], sonuc1[0]);
       if(sonuc1[0] <= 0) printf(" Kazanc yok!\n");
    
    sonuc2 = BF(arr, 0, N-1);
    printf("\nBrute Force Cozum:\n");
    printf("\n Baslangic indeksi:%d\n Bitis indeksi:%d\n Maximum Kazanc:%d\n", sonuc2[1], sonuc2[2], sonuc2[0]);
    if(sonuc2[0] <= 0) printf(" Kazanc yok!\n");


    return 0;
}

int* BF(int* arr, int left, int right){
    int* result = (int*) calloc(3, sizeof(int));   // gain, left, right
    int i, j, sum;
    result[0]=-10000;
    for ( i = left; i <= right; i++){
        sum = 0;
        for ( j = i; j <= right; j++){
            sum += arr[j];
            if (result[0] < sum){
                result[0] = sum;
                result[1] = i;
                result[2] = j;
            }
        }
    }
    return result;
}

int* findMaxWithMiddle(int* arr, int left, int right){
    int* result = (int*) calloc(3, sizeof(int));   //gain, left, right
    int mid = (right + left)/2;
    int i, sum, left_gain, right_gain;

    left_gain = -10000;
    sum = 0;
    for ( i = mid; i >= left; i--){
        sum += arr[i];
        if (sum > left_gain){
            left_gain = sum;
            result[1] = i;
        }
    }

    right_gain = -10000;
    sum = 0;
    for ( i = mid; i <= right; i++){
        sum += arr[i];
        if (sum > right_gain){
            right_gain = sum;
            result[2] = i;
        }
    }

    if (right_gain > left_gain && right_gain > left_gain + right_gain - arr[mid])
    {
        result[0] = right_gain;
        result[1] = mid;
    }
    else if (left_gain > right_gain && left_gain > left_gain + right_gain - arr[mid])
    {
        result[0] = left_gain;
        result[2] = mid;
    }
    else
    {
        result[0] = left_gain + right_gain - arr[mid];
    }
    return result;
}

int* findMaxWDiv(int* arr, int left, int right){
	int* result = (int*) calloc(3, sizeof(int)); 
    if (right-left < 1){    // durma kosulu
    	result[0] = arr[right];
    	result[1] = left;
    	result[2]= right;
        return result;
	}

    // arr yapisi -> sum, left, right
    int* left_gain = findMaxWDiv(arr, left, ((right-left)/2)+left);  // sol taraf icin max kazancli dizi bulma
    int* right_gain = findMaxWDiv(arr, (((right-left)/2)+left)+1, right);  // sol taraf icin max kazancli dizi bulma
    int* sum_gain = findMaxWithMiddle(arr, left_gain[1], right_gain[2]);   // orta noktayi iceren max aralik bulma

    if (left_gain[0] > right_gain[0] && left_gain[0] > sum_gain[0])
        return left_gain;
    else if (right_gain[0] > left_gain[0] && right_gain[0] > sum_gain[0])
        return right_gain;
    else
        return sum_gain;
}
