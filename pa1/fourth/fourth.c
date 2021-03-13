#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int** allocateMatrix(int** matrix, int n) {
  matrix = malloc(sizeof(int*) * n);

  for (int i = 0; i < n; i++) {
    matrix[i] = malloc(sizeof(int) * n);
  }

  return matrix;
}

int** createMatrix(int** matrix, int n) {
  int start = n / 2;
  int finalNum = n * n;

  int currentColumn = start;
  int currentRow = 0;
  int nextRow = 0;
  int nextColumn = 0;

  matrix[0][start] = 1;

  for (int i = 2; i < finalNum + 1; i++) {
    if ((currentColumn + 1) > (n - 1)) {
      nextColumn = 0;
    } else {
      nextColumn = currentColumn + 1;
    }

    if ((currentRow - 1) < 0) {
      nextRow = n - 1;
    } else {
      nextRow = currentRow - 1;
    }

    if (matrix[nextRow][nextColumn] > 0) {
      if ((currentRow + 1) > (n - 1)) {
        nextRow = 0;
      } else {
        nextRow = currentRow + 1;
        nextColumn = currentColumn;
      }
    }

    currentRow = nextRow;
    currentColumn = nextColumn;
    matrix[currentRow][currentColumn] = i;
  }

  return matrix;
}

void freeMatrix(int** matrix, int n) {
  for (int i = 0; i < n; i++) {
    free(matrix[i]);
  }

  free(matrix);
}

void printMatrix(int** matrix, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d\t", matrix[i][j]);
    }

    printf("\n");
  }
}


int main(int argc, char* argv[argc+1]) {
  int n = atoi(argv[1]);

  if (n % 2 == 0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  int** matrix = 0;
  matrix = allocateMatrix(matrix, n);
  matrix = createMatrix(matrix, n);
  printMatrix(matrix, n);


  freeMatrix(matrix, n);
  return EXIT_SUCCESS;
}
