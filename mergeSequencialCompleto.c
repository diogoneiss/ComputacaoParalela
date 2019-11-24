#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#define SIZE_ARRAY 3000000
void mergesort(int arr[], int size);
void Merge(int vec[], int vecSize);
int IsSorted(int *array, int size);
//tamanho do array

int main(int argc, char** argv) {


	int* arr = (int*) malloc(SIZE_ARRAY * sizeof(int));

  //semente aleatoria
	srand(time(NULL));

	for (int i = 0; i < SIZE_ARRAY; i++)
		arr[i] = rand()%SIZE_ARRAY;

  //int arr[] = {6, 7 , 1 , 5 , 6 ,11, 3, 14, 9, 5};
	double start, end;

	start = omp_get_wtime();
	mergesort(arr, SIZE_ARRAY);
	end = omp_get_wtime();

	printf("Tempo: %.10f\n", end - start);

	if(IsSorted(arr, SIZE_ARRAY) == 1)
		printf("Resultado: ordenado!\n");
	else
		printf("Resultado: NÃO ordenado!\n");

	return 0;
}

void mergesort(int arr[], int size) {
	int mid;
	if(size > 1) {
		mid = size / 2;
		mergesort(arr, mid);
		mergesort(arr + mid, size - mid);
		Merge(arr, size);
	}
}

void Merge(int vec[], int vecSize) {
	int mid;
	int i, j, k;
	int* tmp;

	tmp = (int*) malloc(vecSize * sizeof(int));
	if (!tmp)
		exit(1);

	mid = vecSize / 2;

	i = 0;
	j = mid;
	k = 0;

	while (i < mid && j < vecSize) {
		if (vec[i] < vec[j])
			tmp[k] = vec[i++];
		else
			tmp[k] = vec[j++];
		++k;
	}

	if (i == mid)
		while (j < vecSize)
			tmp[k++] = vec[j++];
	else
		while (i < mid)
			tmp[k++] = vec[i++];

	for (i = 0; i < vecSize; ++i)
		vec[i] = tmp[i];

	free(tmp);
}

int IsSorted(int *array, int size) {
	int i, value = 0;
	for(i = 1; i < size; i++)
		if(array[i-1] > array[i])
			return 0;
	return 1;
}
