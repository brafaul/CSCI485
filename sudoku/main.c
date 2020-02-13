//Name: main.c
//Authors: Brayden Faulkner and Hayden Nanny
//Project: Sudoku


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//Stores the arguements that are passed to each thread
//Rowstart and colstart are left unused in checking the rows and columns
struct args{
	int **puzzle;
	int rowStart;
	int colStart;
};

//Takes in an array of nine int and returns 1 if they are all different
int groupCheck(int group[9]){
	//Stores every value found in the array
	int check[9] = {0,0,0,0,0,0,0,0,0};
	for(int i = 0; i < 9; i++){
		if(check[group[i]-1] == 0){
			check[group[i]-1] = 1;
		} else{
			return 0;
		}
	}
	return 1;
}

//Serves as the function base for the tread that checks all the rows in
//a puzzle. Takes in the puzzle as a parameter
void *checkRows(void *input){
	int **puzzle = ((struct args*)input)->puzzle;
	int check = 1;
	for(int i = 0; i < 9; i++){
		int line[9];
		for(int j = 0; j < 9; j++){
			line[j] = puzzle[i][j];
		}
		if(groupCheck(line) == 0){
			check = 0;
		}
	}
	if(check == 0){
		pthread_exit((void *) 0);
	} else{
		pthread_exit((void *) 1);
	}
}

//Serves as the function base for the thread that checks all the columns
//for uniqueness. Takes in the puzzle as a paremeter
void *checkCols(void *input){
	int **puzzle = ((struct args*)input)->puzzle;
	int check = 1;
	for(int i = 0; i < 9; i++){
		int line[9];
		for(int j = 0; j < 9; j++){
			line[j] = puzzle[j][i];
		}
		if(groupCheck(line) == 0) {
			check = 0;		
		}
	}
	if(check == 0){
		pthread_exit((void *) 0);
	}else{
		pthread_exit((void *) 1);
	}
}

//Serves a function base for the threads that check every 3*3 block for 
//uniqueness. Takes the first column and row it needs to check, and the
//puzzle as parameters 
void *checkBlock(void *input){
	//Stores whether the block is valid or not
	int check;
	int line[9];
	int count = 0;
	struct args *local = ((struct args*)input);
	int rowStart = local->rowStart;
	int colStart = local->colStart;
	int **puzzle = local->puzzle;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			line[count] = puzzle[rowStart+i][colStart+j];
			count++;
		}
	}
	check = groupCheck(line);
	if(check == 0){
		pthread_exit((void *) 0);
	}else{
		pthread_exit((void *) 1);
	}
}

//Turns the raw read in puzzle into a 2d array of ints. Takes in a 2d 
//array of chars as a parameter
int **removeSpace(char unFilPuzzle[9][19]){
	int **puzzle;
	puzzle = (int**) malloc(9*sizeof(int*));
	for(int i = 0; i < 9; i++){
		puzzle[i] = malloc(9*sizeof(int));
		int count = 0;
		char curr;
		while(count < 9){
			//Takes every other character from the string
			curr = unFilPuzzle[i][count*2];
			puzzle[i][count] = curr - 48;
			count++;
		}
	}
	return puzzle;
}

//Reads the puzzle in from a file and returns a 2d array of chars. Takes
//the filename as a parameter 
int **readPuzzle(char *fileName){
	FILE *fp = fopen(fileName, "r");
	//Prints an error if the file does not exist
	if(fp == NULL){
		fprintf(stderr, "File %s does not exist \n", fileName);
		exit(1);
	}else{
		int **puzzle;
		puzzle = (int**)malloc(9*sizeof(int*));
		//Stores the raw read in characters
		char puzzleTemp[9][19];
		char *line = NULL;
		size_t len = 0;
		for(int i=0; i < 9; i++){
			puzzle = malloc(9*sizeof(int));
			getline(&line, &len, fp);
			strcpy(puzzleTemp[i], line);
		}
		puzzle = removeSpace(puzzleTemp);
		return puzzle;
	}
}

int main(int argc, char *argv[]){
	if(argc == 2){
		//Stores the returned value from each thread
		void *checks[11];
		//Stores the total puzzle read in from the file
		int **puzzle = readPuzzle(argv[1]);
		//Stores all the threads
		pthread_t threads[11];
		int count = 0;
		//Series of loops that creates and 
		for(int i = 0; i < 9; i += 3){
			for(int  j = 0; j < 9; j += 3){
				struct args blockArgs;
				blockArgs.rowStart = i;
			       	blockArgs.colStart = j;
				blockArgs.puzzle = puzzle;
				pthread_create(&threads[count], NULL, checkBlock, (void *)&blockArgs);
				count++;
			}
		}
		//Stores just the puzzle to be passed to the row checking
		//and column checking threads
		struct args puzzArg;
		puzzArg.puzzle = puzzle;
		//Thread that checks all the rows
		pthread_create(&threads[9], NULL, checkRows, (void *)&puzzArg);
		//Thread that checks all of the columns
		pthread_create(&threads[10], NULL, checkCols, (void *)&puzzArg);
		//Joins all the threads and stores their results in the
		//checks array
		for(int i = 0; i < 11; i++){
			pthread_join(threads[i], &checks[i]);
		}
		int finCheck = 1;
		//Loops through checks and sets finCheck to zero if 
		//any zeroes are found
		for(int i = 0; i < 11; i++){
			if(checks[i] == 0){
				finCheck = 0;
			}
		}
		//If puzzle is not valid
		if(finCheck == 0){
			printf("Puzzle is not valid \n");
		}//If puzzle is valid
		else{
			printf("Puzzle is valid \n");	
		}
		return 0;
	}else if(argc < 2){
		fprintf(stderr, "Please provide the name of the file containing the sudoku puzzle to check \n");
		exit(1);
	}else{
		fprintf(stderr, "Too many arguements provided \n");
		exit(1);
	}
}
