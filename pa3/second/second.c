#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void convertToBinary(long number, int bits) {
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

void convertToTwosComplementUtil(long number, int bits) {
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

  // while (counter < bits) {    // add the binary representation of the number to the array
  //   val[counter] = 0;
  //   counter++;
  // }
  //
  // for (int i = 0; i < bits; i++) {    // flip all the bits
  //   if (val[i] == 1) {
  //     val[i] = 0;
  //   } else if (val[i] == 0) {
  //     val[i] = 1;
  //   }
  // }

  int reachedOne = 0;
  for (int j = 0; j < bits; j++) {
    if (val[j] == 0 && reachedOne == 0) {
      continue;
    } else if (val[j] == 0 && reachedOne == 1) {
      val[j] = 1;
    } else if (val[j] == 1 && reachedOne == 0) {
      reachedOne++;
      continue;
    } else if (val[j] == 1 && reachedOne == 1) {
      val[j] = 0;
    }
  }

  for (int j = bits - 1; j >= 0; j--) {
     printf("%d", val[j]);
  }

  printf("\n");
}

void convertToTwosComplement(long number, int bits) {
  long lowerBound = -1 << (bits - 1);
  long upperBound = (1 << (bits - 1)) - 1;

  if (number < lowerBound) {
    for (int i = 0; i < bits; i++) {
      if (i == 0) {
        printf("1");
      } else {
        printf("0");
      }
    }
    printf("\n");
    return;
  }

  if (number > upperBound) {
    for (int i = 0; i < bits; i++) {
      if (i == 0) {
        printf("0");
      } else {
        printf("1");
      }
    }
    printf("\n");
    return;
  }

  if (number >= 0) {
    convertToBinary(number, bits);
    return;
  }

  convertToTwosComplementUtil(labs(number), bits);
}

int main(int argc, char* argv[argc+1]) {
  FILE* f = fopen(argv[1], "r");

  long number;
  int bits;
  while (fscanf(f, "%lu %d\n", &number, &bits) != EOF) {
    convertToTwosComplement(number, bits);
  }

  fclose(f);
  return EXIT_SUCCESS;
}
