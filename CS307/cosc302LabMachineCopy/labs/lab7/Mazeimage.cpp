
//Joshua Wright Lab 7 CS307 Gregor
//This program will take in a maze file and (if specified) a path file and will creat a ppm image
//based off of the maze and (if specified) with the path overlayed onto the maze
#include <cstdlib>
#include <cstdio>
#include <cstring>

struct RGB {
	RGB() {R = '0'; G = '0'; B = '0';}

	unsigned char R, G, B;
};

int main(int argc, char *argv[]) {

	if (argc != 2 && argc != 3) {
		fprintf(stderr, "usage: %s maze.txt [path.txt]\n", argv[0]);
		return 0;
	}
	
	FILE *finmaze = fopen(argv[1], "r");
	FILE *finpath;
	if (argc == 3) {
		finpath = fopen(argv[2], "r");
	}

	int Nrow, Ncol;
	fscanf(finmaze, "MAZE %i %i", &Nrow, &Ncol);
	if (argc == 3) {
		fscanf(finpath, "PATH %i %i", &Nrow, &Ncol);
	}
	
	int cellw = 10;
	RGB** img;

	img = new RGB*[(Nrow*cellw)+1];
	img[0] = new RGB[((Nrow*cellw)+1)*((Ncol*cellw)+1)];

	for (int i = 1; i <= Nrow*cellw; i++) {
		img[i] = img[i-1] + ((Ncol*cellw+1));
	}

	//creates all white image
	for (int i = 0; i < Nrow*cellw; i++) {
		for (int j = 0; j < Ncol*cellw; j++) {
			img[i][j].R = 255;
			img[i][j].G = 255;
			img[i][j].B = 255;
		}
	}

	//adds in boundry walls
	for (int i = 0; i < Nrow*cellw+1; i++) {
		for(int j = 0; j < Ncol*cellw+1; j++) {
			if (i == 0) {
				img[i][j].R = 0;
				img[i][j].G = 0;
				img[i][j].B = 0;
			}
			if (i == Nrow*cellw) {
				img[i][j].R = 0;
				img[i][j].G = 0;
				img[i][j].B = 0;
			}
			if (j == 0) {
				img[i][j].R = 0;
				img[i][j].G = 0;
				img[i][j].B = 0;
			}
			if (j == Nrow*cellw) {
				img[i][j].R = 0;
				img[i][j].G = 0;
				img[i][j].B = 0;
			}
		}
	}

	//adds path if specified
	if (argc == 3) {
		int row, col;
		while (fscanf(finpath, "%i %i", &row, &col) == 2) {
			for (int i = 1; i < cellw; i++) {
				for(int j = 1; j < cellw; j++) {
					img[row*cellw+i][col*cellw+j].R = 143;
					img[row*cellw+i][col*cellw+j].G = 188;
					img[row*cellw+i][col*cellw+j].B = 143;
				}
			}
		}	
	}

	//adds in all walls specified in maze file
	int row1, col1, row2, col2, rowpoint, colpoint;
	while (fscanf(finmaze, "%i %i %i %i", &row1, &col1, &row2, &col2) == 4) {
		for (int i = 0; i <= cellw; i++) {
			if (row1 == row2) {
				if (col1 < col2) {
					img[row1*cellw+i][col2*cellw].R = 0;
					img[row1*cellw+i][col2*cellw].G = 0;
					img[row1*cellw+i][col2*cellw].B = 0;
				}
				else {
					img[row1*cellw+i][col1*cellw].R = 0;
					img[row1*cellw+i][col1*cellw].G = 0;
					img[row1*cellw+i][col1*cellw].B = 0;
				}
			}
			else {
				if (row1 < row2) {
					img[row2*cellw][col1*cellw+i].R = 0;
					img[row2*cellw][col1*cellw+i].G = 0;
					img[row2*cellw][col1*cellw+i].B = 0;
				}
				else {
					img[row1*cellw][col1*cellw+i].R = 0;
					img[row1*cellw][col1*cellw+i].G = 0;
					img[row1*cellw][col1*cellw+i].B = 0;
				}
			}
		}
	}

	//opens up border at source and sink
	for (int i = 0; i <= cellw; i++) {
		img[0][i].R = 255;
		img[0][i].G = 255;
		img[0][i].B = 255;
	}
	for (int i = 0; i <= cellw; i++) {
		img[Nrow*cellw][Ncol*cellw-cellw+i].R = 255;
		img[Nrow*cellw][Ncol*cellw-cellw+i].G = 255;
		img[Nrow*cellw][Ncol*cellw-cellw+i].B = 255;
	}

	FILE *fout = fopen("maze.ppm", "wb");
	fprintf(fout, "P6\n");
	fprintf(fout, "%i %i\n", Nrow*cellw+1, Ncol*cellw+1);
	fprintf(fout, "%i\n", 255);

	fwrite(img[0], 3, ((Nrow*cellw)+1)*((Ncol*cellw)+1), fout);
	
	fclose(finmaze);
	if (argc == 3) fclose(finpath);
	fclose(fout);

	delete img[0];
	delete img;

	return 0;
}
