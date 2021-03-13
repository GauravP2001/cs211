#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeMatrix(int** matrix, int n) {
  for (int i = 0; i < n; i++) {
    free(matrix[i]);
  }

  free(matrix);
}


int** allocateMatrix(int** matrix, int n) {
  matrix = malloc(sizeof(int*) * n);

  for (int i = 0; i < n; i++) {
    matrix[i] = malloc(sizeof(int) * n);
  }

  return matrix;
}

// void printMatrix(int** matrix, int n) {
//   for (int i = 0; i < n; i++) {
//     for (int j = 0; j < n; j++) {
//       printf("%d\t", matrix[i][j]);
//     }
//
//     printf("\n");
//   }
// }

int determinant(int** matrix, int n) {
  int det;
  int c;
  int s = 1;
  int** arr = 0;
  arr = allocateMatrix(arr, n);
  int row;
  int column;

  if (n == 1) {
    freeMatrix(arr, n);
    return matrix[0][0];
  } else {
    det = 0;

    for (c = 0; c <= n - 1; c++) {
      row = 0;
      column = 0;

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          arr[i][j] = 0;

          if (i != 0 && j != c) {
            arr[row][column] = matrix[i][j];

            if (column < (n-2)) {
              column++;
            } else {
              column = 0;
              row++;
            }
          }
        }
      }

      det = det + s * (matrix[0][c] * determinant(arr, n - 1));
      s = s * -1;
    }
  }
  freeMatrix(arr, n);
  return det;
}

int main(int argc, char* argv[argc+1]) {
  FILE* f = fopen(argv[1], "r");

  if (f == 0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  int n;
  fscanf(f, "%d\n", &n);

  int** matrix = 0;
  matrix = allocateMatrix(matrix, n);

  int temp;
  int row = 0;
  int column = 0;

  while (fscanf(f, "%d", &temp) != EOF) {

    matrix[row][column] = temp;

    if (column != n - 1) {
      column++;
    } else {
      column = 0;
      row++;
    }
  }

  // printMatrix(matrix, n);
  int det = determinant(matrix, n);
  printf("%d\n", det);

  freeMatrix(matrix, n);
  fclose(f);
  return EXIT_SUCCESS;
}
