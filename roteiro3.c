#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define MAX_CHUNK 1500
#define MAX_THREAD 8

int ehPrimo(long num) {
   long divisor;

   if( num <= 1 )
    return 0;
   else
    if(num > 3){
      if(num % 2 == 0)
        return 0;
      long max_divisor = sqrt(num);
      for(divisor = 3; divisor <= max_divisor; divisor += 2)
	    if (num % divisor == 0)
	       return 0;
    }
   return 1;
}

int main() {
	double inicio;
  long LIMITE_MAX = 2000000;
  long contaPrimo;
  long soma;
  int n;
  /*
	Bloco de código padrão, que estou copiando e colando com diferentes diretivas pragma
  */
 
  double tempoParaleloRedux, tempoParaleloCrit;

  	omp_set_num_threads(MAX_THREAD);
	inicio = omp_get_wtime();
  
  if (LIMITE_MAX <= 1)
    soma = 0;
  else {
    soma = 1;
 
	#pragma omp parallel for private (contaPrimo) reduction(+:soma)
    for (n = 3; n < LIMITE_MAX; n += 2){
      contaPrimo = ehPrimo(n);
      soma = soma + contaPrimo;
    }
  }
  tempoParaleloRedux = omp_get_wtime() - inicio;
  printf("Número total de primos: %ld\n", soma);
  printf("Tempo de execução paralelo, com REDUCTION: %lf\n",omp_get_wtime()-inicio);
   
  //FIM BLOCO PADRÃO
  
  inicio = omp_get_wtime();
  
  if (LIMITE_MAX <= 1)
    soma = 0;
  else {
    soma = 1;
 
	#pragma omp parallel for private (contaPrimo) reduction(+:soma) schedule (dynamic,MAX_CHUNK)
    for (n = 3; n < LIMITE_MAX; n += 2){
      contaPrimo = ehPrimo(n);
      #pragma omp critical
      soma = soma + contaPrimo;
    }
  }
  printf("Número total de primos: %ld\n", soma);
  tempoParaleloRedux = omp_get_wtime() - inicio;
  printf("Tempo de execução paralelo, com dynamic: %lf\n", tempoParalelo);
   
  inicio = omp_get_wtime();
  
  if (LIMITE_MAX <= 1)
    soma = 0;
  else {
    soma = 1;
 
	#pragma omp parallel for private (contaPrimo) reduction(+:soma) schedule (static, MAX_CHUNK)
    for (n = 3; n < LIMITE_MAX; n += 2){
      contaPrimo = ehPrimo(n);
      #pragma omp critical
      soma = soma + contaPrimo;
    }
  }
  printf("Número total de primos: %ld\n", soma);
  printf("Tempo de execução paralelo, com static: %lf\n",omp_get_wtime()-inicio);
   
  inicio = omp_get_wtime();
  
  if (LIMITE_MAX <= 1)
    soma = 0;
  else {
    soma = 1;
 
	#pragma omp parallel for private (contaPrimo) reduction(+:soma) schedule (guided,MAX_CHUNK)
    for (n = 3; n < LIMITE_MAX; n += 2){
      contaPrimo = ehPrimo(n);
      #pragma omp critical
      soma = soma + contaPrimo;
    }
  }
  printf("Número total de primos: %ld\n", soma);
  printf("Tempo de execução paralelo, com static: %lf\n",omp_get_wtime()-inicio);
   
	inicio = omp_get_wtime();
  
  if (LIMITE_MAX <= 1)
    soma = 0;
  else {
    soma = 1;
 
	#pragma omp parallel for private (contaPrimo) reduction(+:soma) schedule (auto)
    for (n = 3; n < LIMITE_MAX; n += 2){
      contaPrimo = ehPrimo(n);
      #pragma omp critical
      soma = soma + contaPrimo;
    }
  }
  printf("Número total de primos: %ld\n", soma);
  printf("Tempo de execução paralelo, com auto: %lf\n",omp_get_wtime()-inicio);
   

  inicio = omp_get_wtime();
  
  if (LIMITE_MAX <= 1)
    soma = 0;
  else {
    soma = 1;
 
    for (n = 3; n < LIMITE_MAX; n += 2){
      contaPrimo = ehPrimo(n);
      soma = soma + contaPrimo;
    }
  }
  printf("Número total de primos: %ld\n", soma);
  
  printf("Tempo de execução sequencial: %lf\n",omp_get_wtime() - inicio);

  printf("Máximo de threads nativo: %d\n", omp_get_max_threads());
  printf("Máximo de threads ativo: %d\n", MAX_THREAD);
  printf("Número de threads setadas: %d\n", omp_get_num_threads());
  printf("Speedup médio: %lf", tempoParaleloCrit/tempoParaleloRedux);
  return 0;
}
