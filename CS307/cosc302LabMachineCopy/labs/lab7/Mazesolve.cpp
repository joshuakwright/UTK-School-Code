
//Joshua Wright Lab 7 CS307 Gregor
//This program will take in a maze and the name for an output path file and find the way through the 
//maze using a DFS and then ouput the solution to the path file in the form of ordered pairs of 
//each successive cell form source to sink
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

struct cell {
	cell(int r_row = 0, int c_col = 0) {
		row = r_row;
		col = c_col;
	}
	int row;
	int col;
};

//Direction definintions
typedef enum direction {
	DIR_LEFT,
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	OVER
} DIR_T;

//Finds the direction of the wall relative to first specified cell
DIR_T get_dir(int row1, int col1, int row2, int col2) {
	if (row1 == row2) {
		if (col1 < col2) {
			return DIR_RIGHT;
		}
		if (col1 > col2) { 
			return DIR_LEFT;
		}
	}
	if (col1 == col2) {
		if (row1 < row2) {
			return DIR_DOWN;
		}
		if (row1 > row2) {
			return DIR_UP;
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s maze.txt path.txt\n", argv[0]);
		return 0;
	} 

	FILE *fin = fopen(argv[1], "r");
	FILE *fout = fopen(argv[2], "w");

	int Nrow;
	int Ncol;

	fscanf(fin, "MAZE %i %i", &Nrow, &Ncol);

	//populates maze with no walls
	bool maze[Nrow][Ncol][4];
	for (int i = 0; i < Nrow; i++) {
		for (int j = 0; j < Ncol; j++) {
			for (int k = 0; k < 4; k++) {
				maze[i][j][k] = false;
			}
		}
	}

	//creates boundry walls
	for (int i = 0; i < Nrow; i++) {
		for (int j = 0; j < Ncol; j++) {
			if (i == 0) {
				maze[i][j][1] = true;
			}
			if (i == Nrow-1) {
				maze[i][j][3] = true;
			}
			if (j == 0) {
				maze[i][j][0] = true;
			}
			if (j == Ncol-1) {
				maze[i][j][2] = true;
			}
		}
	}

	//fills the maze with walls from maze file
	int row1, row2, col1, col2;
	while (fscanf(fin, "%i %i %i %i", &row1, &col1, &row2, &col2) == 4) {
		maze[row1][col1][get_dir(row1, col1, row2, col2)] = true;
		maze[row2][col2][get_dir(row2, col2, row1, col1)] = true;
	}

	cell source(0,0);
	cell sink(Nrow-1,Ncol-1);

	int stack_size = 0;
	//stores cells in current path
	cell *stack = new cell[Nrow*Ncol];
	//stores next direction
	DIR_T *D = new DIR_T[Nrow*Ncol];
	DIR_T table[] = {DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, OVER};

	//keeps track of visited cells
	bool V[Nrow][Ncol];
	for (int i = 0; i < Nrow; i++) {
		for (int j = 0; j < Ncol; j++) {
			V[i][j] = false;
		}
	}

	cell point, to;
	stack[0].row = source.row;
	stack[0].col = source.col;
	D[0] = DIR_LEFT;
	V[source.row][source.col] = true;
	stack_size = 1;

	//DFS of maze that finds the path from source to sink
	while (stack_size > 0) {
		//when reaching a dead end, this will allow the function to backtrack until a new 
		//path is found
		while (D[stack_size-1] == 4) {
			V[stack[stack_size-1].row][stack[stack_size-1].col] = false;
			stack_size--;
			
			if (stack_size == 0) {
				fprintf(stderr, "Error: path cannot be found\n");
				return 0;
			}
		}
		
		point = stack[stack_size - 1];

		if (point.row == sink.row && point.col == sink.col) {
			break;
		}
		
		//gets the next cell based off the next direction
		DIR_T dir = D[stack_size - 1];
		switch (dir) {
			case 0: 
				to.row = point.row;
				to.col = point.col - 1;
				break;
			case 1: 
				to.row = point.row - 1;
				to.col = point.col;
				break;
			case 2: 
				to.row = point.row;
				to.col = point.col + 1;
				break;
			case 3: 
				to.row = point.row + 1;
				to.col = point.col;
				break;
		}

		D[stack_size - 1] = table[D[stack_size-1] + 1];

		//tests to see if wall exists in currecnt direciton
		if (maze[point.row][point.col][D[stack_size - 1] - 1]) continue;
				
		//checks to see if cell in current direction has been visited
		if (V[to.row][to.col]) continue;
		
		//if current direction is valid, push cell to stack and repeat process
		stack[stack_size].row = to.row;
		stack[stack_size].col = to.col;
		D[stack_size] = DIR_LEFT;
		V[to.row][to.col] = true;
		stack_size++;
	}

	fprintf(fout, "PATH %i %i\n", Nrow, Ncol);

	for (int i = 0; i < stack_size; i++) {
		fprintf(fout, "%i %i\n", stack[i].row, stack[i].col);
	}

	fclose(fin);
	fclose(fout);

	delete [] stack;
	delete [] D;
}
