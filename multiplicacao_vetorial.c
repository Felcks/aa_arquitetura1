#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <nmmintrin.h> // for SSE4.2
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <x86intrin.h>

// PARA COMPILAR! -msse3

typedef struct {
	int rows;
	int columns;
	int transformedRows;
	int transformedColumns;
	float **array;
} Matriz;

int transform_size(int size){

	if(size % 4 == 0)
		return size;

	int mul = (size / 4) + 1;
	return mul * 4;
}

Matriz ler_matriz(char c){

	Matriz matriz;

	printf("Insira quantidade de linhas e colunas da matriz %c\n", c);
	scanf("%i %i", &matriz.rows, &matriz.columns);

	matriz.transformedRows = transform_size(matriz.rows);
	matriz.transformedColumns = transform_size(matriz.columns);
	
	printf("Insira a matriz %c\n", c);

	matriz.array = (float**)malloc(matriz.transformedRows * sizeof(float*));
	for(int i = 0; i < matriz.transformedRows; i++)
	{
		matriz.array[i] = (float*)calloc(matriz.transformedColumns, sizeof(float));

		/*O Input só ocorre nas linhas e colunas em que o usuário requisitou */
		if(i < matriz.rows){
			for(int j = 0; j < matriz.columns; j++)
			{
				scanf("%f", &matriz.array[i][j]);
			}
		}
	}

	printf("\n");

	return matriz;
}

Matriz matriz_vazia(int rows, int columns){

	Matriz matriz;
	matriz.rows = rows;
	matriz.columns = columns;

	matriz.array = (float**)malloc(matriz.rows * sizeof(float*));
	for(int i = 0; i < matriz.rows; i++)
	{
		matriz.array[i] = (float*)calloc(matriz.columns, sizeof(float));
	}

	return matriz;
}

Matriz matriz_de_numero(int rows, int columns, float number){

	Matriz matriz;
	matriz.rows = rows;
	matriz.columns = columns;

	matriz.transformedRows = transform_size(matriz.rows);
	matriz.transformedColumns = transform_size(matriz.columns);

	matriz.array = (float**)malloc(matriz.transformedRows * sizeof(float*));
	for(int i = 0; i < matriz.transformedRows; i++)
	{
		matriz.array[i] = (float*)calloc(matriz.transformedColumns, sizeof(float));
		
		if(i < matriz.rows){
			for(int j = 0; j < matriz.columns; j++){
				matriz.array[i][j] = number;
			}
		}
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

	/* Matriz transposta da matriz B para usar no vetor de inteiros */
	Matriz matriz = matriz_vazia(matriz_B.transformedColumns, matriz_B.transformedRows);
	for(int i = 0; i < matriz_B.transformedRows; i++){
		for(int j = 0; j < matriz_B.transformedColumns; j++){

			matriz.array[i][j] = matriz_B.array[j][i];
		}
	}

	float r;
	for(int i = 0; i < matriz_A.rows; i ++)
	{
		for(int j = 0; j < matriz_B.columns; j++)
		{
			float **b = (float**)matriz_B.array;
			int k_max = transform_size(matriz_A.columns);
			for(int k = 0; k < k_max / 4; k++)
			{
				__m128 *ptr_A = (__m128*)((float*)&(matriz_A.array[i][k*4]));
				__m128 *ptr_B = (__m128*)((float*)&(matriz.array[j][k*4]));
				__m128 ptr_C = _mm_mul_ps((*ptr_A), (*ptr_B));

				ptr_C = _mm_hadd_ps(ptr_C, ptr_C);
				ptr_C = _mm_hadd_ps(ptr_C, ptr_C);

				_mm_store_ss((float*)&r, ptr_C);
				result[i][j] += r;
			}
		}
	}
}

int main(int argc, char *argv[]){
	
	printf("AA de Arquitetura de Computadores 1 \n");
	printf("Multiplicação de matrizes de forma VETORIAL \n\n");

	Matriz matriz_A; 
	Matriz matriz_B;

	int rowsArg;
	int columnsArg;
	int numberArg;

	char url[]="saida.txt";
	FILE *arq;
	arq = fopen(url, "w");
	if(arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	
	if(argc == 1){
		matriz_A = ler_matriz('A');
		matriz_B = ler_matriz('B');
	}
	else if(argc == 4){
		rowsArg = atoi(argv[1]);
		columnsArg = atoi(argv[2]);
		numberArg = atoi(argv[3]);
		
		matriz_A = matriz_de_numero(rowsArg, columnsArg, numberArg);
		matriz_B = matriz_de_numero(rowsArg, columnsArg, numberArg);
	}
	else{
		printf("Argumentos inválidos.\n");
		printf("Use no formato [rows, columns, number].\n");
		return 0;
	}

	/* variáveis que contam ciclos do processador */
	clock_t c2, c1; 
	c1 = clock();


	Matriz matriz_C; 
		
	if(matriz_A.rows == matriz_B.columns){

		matriz_C = matriz_vazia(matriz_B.transformedRows, matriz_A.transformedColumns);

		multiplicar(matriz_B, matriz_A, matriz_C.array);
	}
	else if(matriz_B.rows == matriz_A.columns){

		matriz_C = matriz_vazia(matriz_A.transformedRows, matriz_B.transformedColumns);

		multiplicar(matriz_A, matriz_B, matriz_C.array);
	}
	else{

		printf("Não é possível executar multiplicação com as matrizes inseridas!\n");
		return 0;
	}


	c2 = clock();
	double tempo = (c2 - c1) * 1000.0/CLOCKS_PER_SEC;
	printf("\nTempo: %lf ms.\n", tempo);

	if(argc == 1)
	{
		printf("Matriz C resultante: \n");
		for(int i = 0; i < matriz_C.rows; i++)
		{
			for(int j = 0; j < matriz_C.columns; j++)
			{
				printf("%.f ", matriz_C.array[i][j]);
			}
			printf("\n");
		} 
	}
	else if(argc == 4)
	{
		/*printf("Resultando no arquivo de saida!\n");
		fprintf(arq, "Matriz C resultante: %ix%i\n", rowsArg, columnsArg);
		for(int i = 0; i < matriz_C.rows; i++)
		{
			for(int j = 0; j < matriz_C.columns; j++)
			{
				fprintf(arq, "%.f ", matriz_C.array[i][j]);
			}
			fprintf(arq, "\n");
		} */
	}


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