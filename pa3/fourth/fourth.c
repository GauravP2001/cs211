#include <stdio.h>
#include <stdlib.h>

void decimalFractionToCanonical(double number, int bits) {
  int mainArray[bits + 1];
  double valueArray[bits + 1];

  if (number >= 1.0) {
    double value = 1.0;
    int counter3 = 0;

    while (number > value * 2) {
        value *= 2;
        counter3++;
    }

    for (int i = 0; i < bits + 1; i++) {
      valueArray[i] = value;
      value /= 2.0;
    }

    for (int i = 0; i < bits + 1; i++) {
      if (valueArray[i] <= number && number != 0) {
        mainArray[i] = 1;
        number = number - valueArray[i];
      } else {
        mainArray[i] = 0;
      }
    }

    for (int i = 0; i < bits + 1; i++) {
      printf("%d", mainArray[i]);

      if (i == 0) {
        printf(".");
      }
    }

    printf(" %d\n", counter3);

  } else if (number < 1) {
    double value = .5;
    int counter3 = -1;

    while (value > number) {
      value /= 2;
      counter3--;
    }

    for (int i = 0; i < bits + 1; i++) {
      valueArray[i] = value;
      value /= 2;

      if (valueArray[i] <= number) {
        mainArray[i] = 1;
        number = number - valueArray[i];
      } else {
        mainArray[i] = 0;
      }

      printf("%d", mainArray[i]);

      if (i == 0) {
        printf(".");
      }
    }

    printf(" %d\n", counter3);
  }
}

int main(int argc, char* argv[argc+1]) {
  FILE* f = fopen(argv[1], "r");

  double number;
  int bits;

  while (fscanf(f, "%lf %d\n", &number, &bits) != EOF) {
    decimalFractionToCanonical(number, bits);
  }

  fclose(f);
  return EXIT_SUCCESS;
}
