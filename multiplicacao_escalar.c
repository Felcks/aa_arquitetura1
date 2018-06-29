#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
	matriz.array = (float**)malloc(matriz.rows * sizeof(float*));
	for(int i = 0; i < matriz.rows; i++)
	{
		matriz.array[i] = (float*)calloc(matriz.columns, sizeof(float));

		for(int j = 0; j < matriz.columns; j++)
		{
			scanf("%f", &matriz.array[i][j]);
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

	matriz.array = (float**)malloc(matriz.rows * sizeof(float*));
	for(int i = 0; i < matriz.rows; i++)
	{
		matriz.array[i] = (float*)calloc(matriz.columns, sizeof(float));
		
		for(int j = 0; j < matriz.columns; j++){
			matriz.array[i][j] = number;
		}	
	}

	return matriz;
}

void multiplicar(Matriz matriz_A, Matriz matriz_B, float** result)
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

int main(int argc, char *argv[]){
	
	printf("AA de Arquitetura de Computadores 1 \n");
	printf("Multiplicação de matrizes de forma ESCALAR \n\n");


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

		printf("Não é possível executar multiplicação com as matrizes inseridas!\n");
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
		printf("Matriz C resultante: \n");
		for(int i = 0; i < matriz_C.rows; i++)
		{
			for(int j = 0; j < matriz_C.columns; j++)
			{
				printf("%.3f ", matriz_C.array[i][j]);
			}
			printf("\n");
		} 
	}
	else if(argc == 4)
	{
		printf("Resultando no arquivo de saida!\n");
		fprintf(arq, "Matriz C resultante: %ix%i\n", rowsArg, columnsArg);
		for(int i = 0; i < matriz_C.rows; i++)
		{
			for(int j = 0; j < matriz_C.columns; j++)
			{
				fprintf(arq, "%.3f ", matriz_C.array[i][j]);
			}
			fprintf(arq, "\n");
		} 
	}


	return 0;
}