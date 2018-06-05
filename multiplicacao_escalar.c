#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
	int rows;
	int columns;
	int **array;
} Matriz;

Matriz ler_matriz(char c){

	Matriz matriz;

	printf("Insira quantidade de linhas e colunas da matriz %c\n", c);
	scanf("%i %i", &matriz.rows, &matriz.columns);
	
	printf("Insira a matriz %c\n", c);
	matriz.array = (int**)malloc(matriz.rows * sizeof(int*));
	for(int i = 0; i < matriz.rows; i++)
	{
		matriz.array[i] = (int*)calloc(matriz.columns, sizeof(int));

		for(int j = 0; j < matriz.columns; j++)
		{
			scanf("%i", &matriz.array[i][j]);
		}
	}
	printf("\n");

	return matriz;
}

Matriz matriz_vazia(int rows, int columns){

	Matriz matriz;
	matriz.rows = rows;
	matriz.columns = columns;

	matriz.array = (int**)malloc(matriz.rows * sizeof(int*));
	for(int i = 0; i < matriz.rows; i++)
	{
		matriz.array[i] = (int*)calloc(matriz.columns, sizeof(int));
	}


	return matriz;
}

void multiplicar(Matriz matriz_A, Matriz matriz_B, int** result)
{
	for(int i = 0; i < matriz_A.rows; i ++)
	{
		for(int j = 0; j < matriz_B.columns; j++)
		{
			for(int k = 0; k < matriz_A.columns; k++)
			{
				result[i][j] += matriz_A.array[i][k] * matriz_B.array[k][j];
			}
		}
	}
}

int main(void){
	
	printf("AA de Arquitetura de Computadores 1 \n");
	printf("Multiplicação de matrizes de forma ESCALAR \n\n");


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


	/*for(int i = 0; i < 50000; i++){
		for(int j = 0; j < 50000; j++){

		}
	}*/
	
	c2 = clock();
	double tempo = (c2 - c1) * 1000/CLOCKS_PER_SEC;
	

	printf("Matriz C resultante: \n");
	for(int i = 0; i < matriz_C.rows; i++)
	{
		for(int j = 0; j < matriz_C.columns; j++)
		{
			printf("%i ", matriz_C.array[i][j]);
		}
		printf("\n");
	} 

	printf("Tempo: %lf \n", tempo);

	return 0;
}