#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <nmmintrin.h> // for SSE4.2
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <x86intrin.h>

typedef struct {
	int rows;
	int columns;
	float **array;
} Matriz;

Matriz ler_matriz(char c){

	Matriz matriz;

	printf("Insira quantidade de linhas e colunas da matriz %c\n", c);
	scanf("%i %i", &matriz.rows, &matriz.columns);
	
	printf("Insira a matriz %c\n", c);

	matriz.array = (float**)malloc((matriz.rows + (4 - matriz.rows)) * sizeof(float*));
	for(int i = 0; i < matriz.rows; i++)
	{
		matriz.array[i] = (float*)calloc(matriz.columns + (4 - matriz.columns), sizeof(float));

		for(int j = 0; j < matriz.columns; j++)
		{
			scanf("%f", &matriz.array[i][j]);
		}
	}

	for(int i = matriz.rows; i < matriz.rows + (4 - matriz.rows); i++)
	{
		matriz.array[i] = (float*)calloc(matriz.columns + (4 - matriz.columns), sizeof(float));
	}

	printf("\n");

	return matriz;
}

Matriz matriz_vazia(int rows, int columns){

	Matriz matriz;
	matriz.rows = rows;
	matriz.columns = columns;

	matriz.array = (float**)malloc((matriz.rows + (4 - matriz.rows)) * sizeof(float*));
	for(int i = 0; i < matriz.rows; i++)
	{
		matriz.array[i] = (float*)calloc(matriz.columns + (4 - matriz.columns), sizeof(float));
	}

	for(int i = matriz.rows; i < matriz.rows + (4 - matriz.rows); i++)
	{
		matriz.array[i] = (float*)calloc(matriz.columns + (4 - matriz.columns), sizeof(float));
	}


	return matriz;
}

//i = linha da matriz A
//j = coluna da matriz B
//k = coluna da matriz A / Linha de B
/***              1   2    3   4
2 3  3 1  2*3+3*2  2*1+3*4
1 0  2 4
4 5  
*/

void multiplicar(Matriz matriz_A, Matriz matriz_B, float** result)
{

	int oldColumns = matriz_B.columns;
	int oldRows = matriz_B.rows;

	Matriz matriz = matriz_vazia(matriz.columns, matriz.rows);

	for(int i = 0; i < matriz_B.rows + (4 - matriz_B.rows); i++){
		for(int j = 0; j < matriz_B.columns + (4 - matriz_B.columns); j++){

			matriz.array[i][j] = matriz_B.array[j][i];
		}
	}


	for(int i = 0; i < matriz_A.rows; i ++)
	{
		for(int j = 0; j < matriz_B.columns; j++)
		{
			float **b = (float**)matriz_B.array;
			for(int k = 0; k <= matriz_A.columns / 4; k++)
			{
				__m128 *ptr_A = (__m128*)((float*)&(matriz_A.array[i][k*4]));
				__m128 *ptr_B = (__m128*)((float*)&(matriz.array[j][k*4]));
				__m128 ptr_C = _mm_mul_ps((*ptr_A), (*ptr_B));

				ptr_C = _mm_hadd_ps(ptr_C, ptr_C);
				ptr_C = _mm_hadd_ps(ptr_C, ptr_C);
				_mm_store_ss((float*)&result[i][j], ptr_C);
				
			}
		}
	}
}

int main(void){
	
	printf("AA de Arquitetura de Computadores 1 \n");
	printf("Multiplicação de matrizes de forma VETORIAL \n\n");


	
	
	Matriz matriz_A = ler_matriz('A');
	Matriz matriz_B = ler_matriz('B');


	clock_t c2, c1; /* variáveis que contam ciclos do processador */

	c1 = clock();


	Matriz matriz_C; 
		
	if(matriz_A.rows == matriz_B.columns){

		matriz_C = matriz_vazia(matriz_B.rows, matriz_A.columns);

		multiplicar(matriz_B, matriz_A, matriz_C.array);
	}
	else if(matriz_B.rows == matriz_A.columns){

		matriz_C = matriz_vazia(matriz_A.rows, matriz_B.columns);

		multiplicar(matriz_A, matriz_B, matriz_C.array);
	}
	else{

		printf("Não é possível executar multiplicação com as matrizes passadas!\n");
		return 0;
	}


	c2 = clock();
	double tempo = (c2 - c1) * 1000/CLOCKS_PER_SEC;
	printf("%lf c2\n", c2);
	printf("%lf c1\n", c1);


	printf("Matriz C resultante: \n");
	for(int i = 0; i < matriz_C.rows; i++)
	{
		for(int j = 0; j < matriz_C.columns; j++)
		{
			printf("%.3f ", matriz_C.array[i][j]);
		}
		printf("\n");
	} 

	printf("Tempo: %lf \n", tempo);

	return 0;
}

/*int main(void)
{
	float a[] = {1.0, 4.0, 9.0, 16.0};
	__m128 *ptr = (__m128*)a;
	_mm_store_ps((float*)a, _mm_sqrt_ps(*ptr));

	for(int i = 0; i < 4; i++){

		printf("%f ", a[i]);
	}
	printf("\n");

	/*
	union { __m128 a4; float a[4]; };
	__m128 a4 = _mm_set_ps( 4.0f, 4.1f, 4.2f, 4.3f );

	return 0;
} */