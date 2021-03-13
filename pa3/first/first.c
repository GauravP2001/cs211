#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void convertToBinary(unsigned long  number, int bits) {
  int val[bits];

  int counter = 0;

  while (number > 0 && counter < bits) {
    val[counter] = number % 2;
    number = number / 2;
    counter++;
  }

  while (counter < bits) {
    val[counter] = 0;
    counter++;
  }


  for (int j = bits - 1; j >= 0; j--) {
     printf("%d", val[j]);
  }

  printf("\n");
}


int main(int argc, char* argv[argc+1]) {
  FILE* f = fopen(argv[1], "r");

  unsigned long  number;
  int bits;
  while (fscanf(f, "%lu %d\n", &number, &bits) != EOF) {
    convertToBinary(number, bits);
  }

  fclose(f);
  return EXIT_SUCCESS;
}
