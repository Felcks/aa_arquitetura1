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

Matriz matriz_identidade(int rows, int columns){

	Matriz matriz;
	matriz.rows = rows;
	matriz.columns = columns;

	matriz.array = (float**)malloc(matriz.rows * sizeof(float*));
	for(int i = 0; i < matriz.rows; i++)
	{
		matriz.array[i] = (float*)calloc(matriz.columns, sizeof(float));
		
		for(int j = 0; j < matriz.columns; j++){
			matriz.array[i][j] = 0;

			if(i == j)
				matriz.array[i][j] = 1;
		}	
	}

	return matriz;
}

int conferir_matriz_identidade(Matriz matriz_A)
{
	for(int i = 0; i < matriz_A.rows; i ++)
	{
		for(int j = 0; j < matriz_A.columns; j++)
		{
			if(i == j){
				if(matriz_A.array[i][j] != 1)
					return 0;
			}
			else{
				if(matriz_A.array[i][j] != 0)
					return 0;
			}
		}
	}

	return 1;
}

int main(int argc, char *argv[]){
	
	printf("AA de Arquitetura de Computadores 1 \n");
	printf("Descobrir se uma matriz é identidade ou não de forma ESCALAR\n\n");

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