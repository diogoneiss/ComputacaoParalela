
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

//prototipos
int main(int argc, char **argv);
double parametrosParalelizacao(int *array, int numThreads, int nested, int stop);
void preencherArray(int *a);
void swap(int *a, long i, long j);
long partition(int *a, long left, long right, long pivot);
void quickParallel(int *a, long left, long right, int stop);
void quickSequential(int *a, long left, long right);
int IsSort(int *array);

const size_t size = 10;

int main(int argc, char **argv)
{
    FILE *arquivoLog = fopen("logQuicksort.txt", "a");
    FILE *resultadosComparacao = fopen("resultadosQuicksort.txt", "a");
    fprintf(arquivoLog, "Iniciando mais uma iteração do programa, visando comparar resultados.\nTamanho do array de elementos aleatorios: [%ld]\n", size);
    long i;
    int *array;

    array = malloc(size * sizeof(long));

    int stop = 1000;
    int numThreads = 1000;
    int nestedNum = 4;
    double exec;

    double melhorTempoExec = 100;
    int arrayThreadNestedStop[3];

    for (numThreads = 1000; numThreads > 0; numThreads = numThreads / 2)
    {
        for(stop = 1000; stop > 0; stop = stop/2){
            for(nestedNum = 100; nestedNum > 0; nestedNum = nestedNum /2){
                exec = parametrosParalelizacao(array, numThreads, nestedNum, stop);
                fprintf(arquivoLog, "\tTempo: [%lf]\t Threads: [%d]\tNested: [%d] \t Stop: [%d]\n", exec, numThreads, nestedNum, stop);
                if (exec < melhorTempoExec)
                {
                    melhorTempoExec = exec;
                    arrayThreadNestedStop[0] = numThreads;
                    arrayThreadNestedStop[1] = nestedNum;
                    arrayThreadNestedStop[2] = stop;
                }
            }
        }
    }

    printf("Melhor tempo: [%lf]\nMelhores parâmetros: Threads: [%d]\tNested: [%d]\tStop: [%d]\n", melhorTempoExec, arrayThreadNestedStop[0], arrayThreadNestedStop[1], arrayThreadNestedStop[2]);
    fprintf(resultadosComparacao, "Dados para um array de tamanho %ld:\n", size);
    fprintf(resultadosComparacao, "Melhor tempo: [%lf]\nMelhores parâmetros: Threads: [%d]\tNested: [%d]\tStop: [%d]\n", melhorTempoExec, arrayThreadNestedStop[0], arrayThreadNestedStop[1], arrayThreadNestedStop[2]);
    return 0;
}
double parametrosParalelizacao(int *array, int numThreads, int nestedNum, int stop)
{
    preencherArray(array);
    double start, end;

    double startInicial = omp_get_wtime();

    omp_set_nested(nestedNum);

    omp_set_num_threads(numThreads);
    start = omp_get_wtime();
//estou utilizando tasks, ja que sections foram lentas demais
#pragma omp parallel
#pragma omp single
    quickParallel(array, 0, size - 1, stop);
#pragma omp barrier
    end = omp_get_wtime();

    double exec = end - start;

    printf("Tempo: %.7lf\tthreads: %d\tnested: %d\tstop: %d\n", exec, numThreads, nestedNum, stop);
    if (IsSort(array) == 1)
        printf("Result: Sorted\n");
    else
        printf("Result: Not Sorted\n");

    end = omp_get_wtime();
    //fprintf(resposta, "Melhor tempo de execução, em %d threads: [%lf]\n", melhorThread, melhorExecucao);
    // printf("Melhor tempo de execução, em %d threads: [%lf]\n", melhorThread, melhorExecucao);
    return exec;
}
void preencherArray(int *a)
{
    srand(time(NULL));
    for (int i = 0; i < size; i++)
        a[i] = rand() % size;
}

void swap(int *a, long i, long j)
{
    long t = a[i];
    a[i] = a[j];
    a[j] = t;
}

long partition(int *a, long left, long right, long pivot)
{
    long pos, i;
    swap(a, pivot, right);
    pos = left;

    // por algum motivo o omp nao me permite usar esse for paralelo...
    //#pragma omp parallel for
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
//quicksort paralelo, utilizando tasks
void quickParallel(int *a, long left, long right, int stop)
{
    if (left < right)
    {
        long pivot = (left + right) / 2;
        long pos = partition(a, left, right, pivot);

        if (stop > 1)
        { // chamadas paralelizadas

//tasks tiveram desempenho superior ao section
#pragma omp task
            quickParallel(a, left, pos - 1, stop / 2);
#pragma omp task
            quickParallel(a, pos + 1, right, stop / 2);
        }
        else
        {
            // #pragma omp taskwait
            quickSequential(a, left, pos - 1);
            quickSequential(a, pos + 1, right);
        }
    }
}
//versao não paralela do quicksort
void quickSequential(int *a, long left, long right)
{
    if (left < right)
    {
        long pivot = (left + right) / 2;
        long pos = partition(a, left, right, pivot);

        quickSequential(a, left, pos - 1);
        quickSequential(a, pos + 1, right);
    }
}

int IsSort(int *array)
{
    long i;
    for (i = 1; i < size; i++)
        if (array[i - 1] > array[i])
            return 0;
    return 1;
}
