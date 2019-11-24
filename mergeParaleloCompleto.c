#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#define SIZE_ARRAY 10

int main(int argc, char **argv);
void preencherArray(int *arr);
void mergesort_serial(int arr[], int size);
void printarArray(int *arr);
void mergesort_parallel(int arr[], int size, int threads, int stop);
void Merge(int vec[], int vecSize);
int IsSorted(int *array, int size);

int main(int argc, char **argv)
{

	FILE *arquivoLog = fopen("logMergesort.txt", "a");
	FILE *resultadosComparacao = fopen("resultadosMergesort.txt", "a");
	fprintf(resultadosComparacao, "Iniciando mais uma iteração do programa, visando comparar resultados.\nTamanho do array de elementos aleatorios: [%d]\n", SIZE_ARRAY);
	int *arr = (int *)malloc(SIZE_ARRAY * sizeof(int));

	//int arr[] = {6, 7 , 1 , 5 , 6 ,11, 3, 14, 9, 5};
	double start, end, exec;
	int stop = 10;
	int threads;
	int nested;

	int melhoresExecucoes[3];
	double melhorTempo = 100;

	//for(stop = 1000; stop > 0; stop /= 2)
	for (nested = 10; nested > 0; nested /= 2){
		for (threads = 100; threads > 0; threads /= 2)
		{
			preencherArray(arr);
			omp_set_num_threads(threads);
			omp_set_nested(nested);

			start = omp_get_wtime();
			#pragma omp parallel
			#pragma omp single
			mergesort_parallel(arr, SIZE_ARRAY, threads, stop);
			//
			end = omp_get_wtime();

			exec = end - start;
			if (exec < melhorTempo)
			{
				melhorTempo = exec;
				melhoresExecucoes[0] = threads;
				melhoresExecucoes[1] = nested;
				melhoresExecucoes[2] = stop;
			}
			printf("Tempo: %.10f\tThreads: %d\tNested: %d\n", exec, threads, nested);
			fprintf(arquivoLog, "Tempo: %.10f\tThreads: %d\tNested: %d\n", exec, threads, nested);
		}
	}
	//printarArray(arr);

	if (IsSorted(arr, SIZE_ARRAY) == 1)
		printf("Resultado: ordenado!\n");
	else
		printf("Resultado: NÃO ordenado!\n");

	printf("Melhor tempo: %lf\tThreads: %d\tNested: %d\tStop: %d\n", melhorTempo, melhoresExecucoes[0], melhoresExecucoes[1], melhoresExecucoes[2]);
	fprintf(resultadosComparacao, "Melhor tempo: %lf\tThreads: %d\tNested: %d\tStop: %d\n", melhorTempo, melhoresExecucoes[0], melhoresExecucoes[1], melhoresExecucoes[2]);

	
	fclose(resultadosComparacao);
	fclose(arquivoLog);
	return 0;
}
void preencherArray(int *arr)
{
	//semente aleatoria
	srand(time(NULL));

	for (int i = 0; i < SIZE_ARRAY; i++)
		arr[i] = rand() % SIZE_ARRAY;
}

void mergesort_serial(int arr[], int size)
{
	int mid;
	if (size > 1)
	{
		mid = size / 2;
		mergesort_serial(arr, mid);
		mergesort_serial(arr + mid, size - mid);
	}

	Merge(arr, size);
}
void printarArray(int *arr)
{
	printf("[");
	for (int i = 0; i < SIZE_ARRAY; i++)
		printf(" %d", arr[i]);

	printf("]\n");
}

void mergesort_parallel(int arr[], int size, int threads, int stop)
{
	int mid;
	if (size > 1)
	{
		//uso esse contador de threads para decidir se ser� paralelizado ou nao
		if (stop > 1)
		{
			

				mid = size / 2;
#pragma omp task
				mergesort_parallel(arr, mid, threads, stop / 2);
#pragma omp task
				mergesort_parallel(arr + mid, size - mid, threads, stop - stop / 2);
#pragma omp taskwait
			Merge(arr, size);
			
		}
		else
			mergesort_serial(arr, size);
	}
}

void Merge(int vec[], int vecSize)
{
	int mid;
	int i, j, k;
	int *tmp;

	tmp = (int *)malloc(vecSize * sizeof(int));
	if (!tmp)
		exit(1);

	mid = vecSize / 2;

	i = 0;
	j = mid;
	k = 0;

	while (i < mid && j < vecSize)
	{
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

int IsSorted(int *array, int size)
{
	int i, value = 0;
	for (i = 1; i < size; i++)
		if (array[i - 1] > array[i])
			return 0;
	return 1;
}
