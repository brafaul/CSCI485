#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int **removeSpace(char unFilPuzzle[9][19]){
	int **puzzle;
	puzzle = (int**) malloc(9*sizeof(int*));
	for(int i = 0; i < 9; i++){
		puzzle[i] = malloc(9*sizeof(int));
		int count = 0;
		char curr;
		curr = strtok(unFilPuzzle[i], " ");
		while(curr != NULL){
			puzzle[i][count] = (int)curr;
			count++;
			curr = strtok(NULL, " ");
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
		char puzzleTemp[9][19];
		char *line = NULL;
		size_t len = 0;
		for(int i=0; i < 9; i++){
			getline(&line, &len, fp);
			strcpy(puzzleTemp[i], line);
		}
		puzzle = removeSpace(puzzleTemp);
		return puzzle;
	}
}

int main(int argc, char *argv[]){
	if(argc == 2){
		read_puzzle(argv[1]);
	}else if(argc < 2){
		fprintf(stderr, "Please provide the name of the file containing the sudoku puzzle to check \n");
		exit(1);
	}else{
		fprintf(stderr, "Too many arguements provided \n");
		exit(1);
	}
}
