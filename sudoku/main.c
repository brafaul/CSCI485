#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int puzzleValidity = 1;

struct args{
	int **puzzle;
	int rowStart;
	int colStart;
};

void printPuzzle(int **puzzle){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			printf("%d ", puzzle[i][j]);
		}
		printf("\n");
	}
}

void groupCheck(int group[9]){
	int check[9] = {0,0,0,0,0,0,0,0,0};
	for(int i = 0; i < 9; i++){
		if(check[group[i]-1] == 0){
			check[group[i]-1] = 1;
		} else{
			puzzleValidity = 0;
		}
	}
}

void *checkRows(void *input){
	int **puzzle = ((struct args*)input)->puzzle;
	for(int i = 0; i < 9; i++){
		int line[9];
		for(int j = 0; j < 9; j++){
			line[j] = puzzle[i][j];
		}
		groupCheck(line);
	}
	pthread_exit(NULL);
	return NULL;
}
void *checkCols(void *input){
	int **puzzle = ((struct args*)input)->puzzle;
	for(int i = 0; i < 9; i++){
		int line[9];
		for(int j = 0; j < 9; j++){
			line[j] = puzzle[j][i];
		}
		groupCheck(line);
	}
	pthread_exit(NULL);
	return NULL;
}
void *checkBlock(void *input){
	int line[9];
	int count = 0;
	struct args *local = ((struct args*)input);
	int rowStart = local->rowStart;
	int colStart = local->colStart;
	int **puzzle = local->puzzle;
	//printPuzzle(puzzle);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			line[count] = puzzle[rowStart+i][colStart+j];
			count++;
		}
	}
	groupCheck(line);
	//pthread_exit(NULL);
	return NULL;
}

int **removeSpace(char unFilPuzzle[9][19]){
	int **puzzle;
	puzzle = (int**) malloc(9*sizeof(int*));
	for(int i = 0; i < 9; i++){
		puzzle[i] = malloc(9*sizeof(int));
		int count = 0;
		char curr;
		while(count < 9){
			curr = unFilPuzzle[i][count*2];
			puzzle[i][count] = curr - 48;
			count++;
		}
	}
	return puzzle;
}

int **read_puzzle(char *fileName){
	FILE *fp = fopen(fileName, "r");
	if(fp == NULL){
		fprintf(stderr, "File %s does not exist \n", fileName);
		exit(1);
	}else{
		int **puzzle;
		puzzle = (int**)malloc(9*sizeof(int*));
		char puzzleTemp[9][19];
		char *line = NULL;
		size_t len = 0;
		for(int i=0; i < 9; i++){
			puzzle = malloc(9*sizeof(int));
			getline(&line, &len, fp);
			strcpy(puzzleTemp[i], line);
			//printf("%s", puzzleTemp[i]);
		}
		puzzle = removeSpace(puzzleTemp);
		return puzzle;
	}
}

int main(int argc, char *argv[]){
	if(argc == 2){
		int **puzzle = read_puzzle(argv[1]);
		pthread_t *threads[11];
		int count = 0;
		//struct args blockArgs[9];
		for(int i = 0; i < 9; i += 3){
			for(int  j = 0; j < 9; j += 3){
				struct args blockArgs;
				blockArgs.rowStart = i;
			       	blockArgs.colStart = j;
				blockArgs.puzzle = puzzle;
				checkBlock(&blockArgs);
				count++;
			}
		}
		struct args puzzArg;
		puzzArg.puzzle = puzzle;
		int threadCheck =  pthread_create(&threads[9], NULL, checkRows, (void *)&puzzArg);
		threadCheck  = pthread_create(&threads[10], NULL, checkCols, (void *)&puzzArg);
		for(int i = 9; i < 11; i++){
			pthread_join(threads[i], NULL);
		}
		printf("%d \n", puzzleValidity);
	}else if(argc < 2){
		fprintf(stderr, "Please provide the name of the file containing the sudoku puzzle to check \n");
		exit(1);
	}else{
		fprintf(stderr, "Too many arguements provided \n");
		exit(1);
	}
}
