/*

   Thanks to Prof. Henrique Cota!

*/

#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

void swap(int *a, int i, int j);
int partition(int *a, int left, int right, int pivot);
void quickSequential(int *a, int left, int right);
int IsSort(int *array, int size);

int main(int argc, char **argv)
{
    int size = 1000000, i, *array;

    array = malloc(size * sizeof(int));
    srand(time(NULL));
    for (i = 0; i < size; i++)
        array[i] = rand() % size;
    double start, end;
    int MAX_THREADS = 4096; // Caution!
    start = omp_get_wtime();
     
  quickSequential(array, 0, size - 1); 

     end = omp_get_wtime();
    printf("Tempo de execução: %lf\n", end-start);
    if (IsSort(array, size) == 1)
                    printf("Result: Sorted\n");
                else
                    printf("Result: Not Sorted\n");
    return 0;
}

void swap(int *a, int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

int partition(int *a, int left, int right, int pivot)
{
    int pos, i;
    swap(a, pivot, right);
    pos = left;
    for (i = left; i < right; i++)
    {
        if (a[i] < a[right])
        {
            swap(a, i, pos);
            pos++;
        }
    }
    swap(a, right, pos);
    return pos;
}

void quickSequential(int *a, int left, int right)
{
    if (left < right)
    {
        int pivot = (left + right) / 2;
        int pos = partition(a, left, right, pivot);

        quickSequential(a, left, pos - 1);
        quickSequential(a, pos + 1, right);
    }
}

int IsSort(int *array, int size)
{
    int i;
    for (i = 1; i < size; i++)
        if (array[i - 1] > array[i])
            return 0;
    return 1;
}
