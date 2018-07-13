#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <nmmintrin.h> // for SSE4.2
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <x86intrin.h>
#include <tmmintrin.h>


typedef struct {
	int rows;
	int columns;
	int transformedRows;
	int transformedColumns;
	float **array;
} Matriz;

Matriz matriz_identidade(int rows, int columns);

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

	Matriz matriz = matriz_identidade(rows, columns);
	//matriz.array[rows-1][columns-1] = number;
	matriz.array[rows-1][0] = number;

	return matriz;
	
	/*Matriz matriz;
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

	return matriz;*/
}

Matriz matriz_identidade(int rows, int columns){

	Matriz matriz;
	matriz.rows = rows;
	matriz.columns = columns;

	matriz.transformedRows = transform_size(matriz.rows);
	matriz.transformedColumns = transform_size(matriz.columns);

	matriz.array = (float**)malloc(matriz.transformedRows * sizeof(float*));
	for(int i = 0; i < matriz.transformedRows; i++)
	{
		matriz.array[i] = (float*)calloc(matriz.transformedColumns, sizeof(float));
		matriz.array[i][i] = 1;
		
	}

	return matriz;
}

int conferir_matriz_identidade(Matriz matriz_A)
{
	int somaDiagonal = 0;
	for(int i = 0; i < matriz_A.rows; i ++)
	{
		somaDiagonal += matriz_A.array[i][i];

		int k_max = transform_size(matriz_A.columns);
		for(int j = 0; j < k_max / 4; j++)
		{
			float r;
			int k = j * 4;

			__m128 *ptr_A = (__m128*)((float*)&(matriz_A.array[i][k]));
			__m128 ptr_C = _mm_hadd_ps(*ptr_A, *ptr_A);
			ptr_C = _mm_hadd_ps(ptr_C, ptr_C);

			_mm_store_ss((float*)&r, ptr_C);
			//printf("r= %lf\n", r);


			int diff = i - k; 
			if(diff <= 3 && diff >= 0){
				
				if(r != 1)
					return 0;
			}
			else{
				if(r != 0)
					return 0;
			}
		}
	}

	if(somaDiagonal != matriz_A.rows)
		return 0;

	return 1;
}

int main(int argc, char *argv[]){
	
	printf("AA de Arquitetura de Computadores 1 \n");
	printf("Descobrir se uma matriz é identidade ou não de forma VETORIAL\n\n");

	Matriz matriz_A;

	int rowsArg;
	int columnsArg;
	int ehIdentidade;

	char url[]="saida.txt";
	FILE *arq;
	arq = fopen(url, "w");
	if(arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	
	if(argc == 1){
		matriz_A = ler_matriz('A');
	}
	else if(argc == 4){
		rowsArg = atoi(argv[1]);
		columnsArg = atoi(argv[2]);
		ehIdentidade = atoi(argv[3]);
		
		if(ehIdentidade == 1)
			matriz_A = matriz_identidade(rowsArg, columnsArg);
		else
			matriz_A = matriz_de_numero(rowsArg, columnsArg, 3);
	}
	else{
		printf("Argumentos inválidos.\n");
		printf("Use no formato [rows, columns, identidade(1 = sim 0 = nao)].\n");
		return 0;
	}

	clock_t c2, c1; /* variáveis que contam ciclos do processador */
	c1 = clock();

	int resultado; 	
	if(matriz_A.rows == matriz_A.columns){

		resultado = conferir_matriz_identidade(matriz_A);
		
	}
	else{

		printf("A matriz precisa ser quadrada!\n");
		return 0;
	}


	/*for(int i = 0; i < 50000; i++){
		for(int j = 0; j < 50000; j++){

		}
	}*/
	
	c2 = clock();
	double tempo = (c2 - c1) * 1000.0/CLOCKS_PER_SEC;
	printf("\nTempo: %lf ms.\n", tempo);

	if(argc == 1)
	{
		if(resultado == 1)
			printf("A matriz inserida é identidade.\n");
		else
			printf("A matriz inserida NÃO é identidade.\n");
	}
	else if(argc == 4)
	{
		if(resultado == 1)
			printf("A matriz inserida é identidade.\n");
		else
			printf("A matriz inserida NÃO é identidade.\n");

		/*printf("OBS: A matriz inserida está no arquivo de saida!\n");
		fprintf(arq, "Matriz A: %ix%i\n", rowsArg, columnsArg);
		for(int i = 0; i < matriz_A.rows; i++)
		{
			for(int j = 0; j < matriz_A.columns; j++)
			{
				fprintf(arq, "%.f ", matriz_A.array[i][j]);
			}
			fprintf(arq, "\n");
		} */
	}


	return 0;
}