
//Joshua Wright Lab7 CS307 Greggor
//This program will take in a row and column and a .txt file and output a maze of the rowxcol size
//to the file in the form of four two rodered pairs, signfiing a wall inbetween them.
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "dset.h"

struct cell {
	cell(int r_row = 0, int c_col = 0) {
		row = r_row;
		col = c_col;
	}
	
	int row;
	int col;
};

void swap(cell &c1, cell &c2) { 
	cell temp;
	temp.row = c1.row;
	temp.col = c1.col;
	c1.row = c2.row;
	c1.col = c2.col;
	c2.row = temp.row;
	c2.col = temp.col;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "usage: %s nrow ncol maze.txt\n", argv[0]);
		return 0;
	}

	int Nrow = atoi(argv[1]);
	int Ncol = atoi(argv[2]);

	// Grid wall:     (i0,j0) | (i1,j1)
	// Horizontal wall: (i,j) | (i+1,j)
	// Vertical wall:   (i,j) | (i,j+1)

	int N = ((Nrow-1)*Ncol) + ((Ncol-1)*Nrow);
	cell wall[N][2];

	//creates grid with all walls filled in
	int n = 0;
	cell temp;
	for (int i = 0; i < Nrow-1; i++) {
		for (int j = 0; j < Ncol; j++) {
			wall[n][0] = cell(i,j);
			wall[n][1] = cell(i+1,j);
			n++;
		}
	}
	for (int i = 0; i < Nrow; i++) {
		for (int j = 0; j < Ncol-1; j++) {
			wall[n][0] = cell(i,j);
			wall[n][1] = cell(i,j+1);
			n++;
		}
	}

	//randomly swaps wall locations to ensure different maze ever time
	srand(time(NULL));
	for (int i = N-1; i > 0; --i) {
		int randnum = rand();
		swap(wall[i][0], wall[randnum % (i+1)][0]);
		swap(wall[i][1], wall[randnum % (i+1)][1]);
	}


	FILE *fout = fopen(argv[3], "w");
	fprintf(fout, "%s %i %i\n", "MAZE", Nrow, Ncol);

	//creates disjoint set for every cell and merges until every cell can reach every other cell
	dset maze(Nrow*Ncol);
	int k;
	for (k=0; k<N; k++) {
		int i = wall[k][0].col + ( wall[k][0].row * Ncol );
		int j = wall[k][1].col + ( wall[k][1].row * Ncol );
		if (maze.find(i) != maze.find(j)) {
			maze.merge(i,j);
		}

		else {
			fprintf(fout, "%*i %*i %*i %*i\n", 3, wall[k][0].row, 4, wall[k][0].col, 4, wall[k][1].row, 4, wall[k][1].col);
		}

		if (maze.size() == 1) break;
	}

	k++;
	for (; k < N; k++) {
		fprintf(fout, "%*i %*i %*i %*i\n", 3, wall[k][0].row, 4, wall[k][0].col, 4, wall[k][1].row, 4, wall[k][1].col);
	}

	fclose(fout);
}

