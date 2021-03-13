#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long* sToC(long number, int bits, long* val) {
  long num = number * - 1;

  for (int i = 0; i < bits; i++) {
    if (num % 2 == 0) {
      val[i] = 1;
    } else {
      val[i] = 0;
    }

    num /= 2;
  }

  int position = 0;
  int boolean = 0;

  while (boolean == 0) {
    if (val[position] == 0) {
      val[position] = 1;
      boolean = 1;
    } else {
      val[position] = 0;
      position++;
    }
  }

  return val;
}

long bToD(int bits, long* val) {
  long* val2 = malloc(sizeof(long) * bits);
  int counter = 1;

  for (int i = 0; i < bits; i++) {
    val2[i] = counter;
    counter *= 2;
  }

  long num = 0;

  for (int j = 0; j < bits; j++) {
    num += (val[j] * val2[j]);
  }

  free(val2);
  return num * -1;
}

long* convertToBinary(long number, int bits, long* val) {
  // long* val = malloc(sizeof(long) * bits);
  // int counter = 0;
  //
  // while (number > 0 && counter < bits) {
  //   val[counter] = number % 2;
  //   number = number / 2;
  //   counter++;
  // }
  //
  // while (counter < bits) {
  //   val[counter] = 0;
  //   counter++;
  // }

  for (int i = 0; i < bits; i++) {
    val[i] = number % 2;
    number /= 2;
  }

  return val;
}

long uToS(long number, int bits) {
  long upperBound = (1 << (bits - 1)) - 1;

  if (number <= upperBound) {
    return number;
  }

  long* val = malloc(sizeof(long) * bits);
  val = convertToBinary(number, bits, val);

  int position = 0;
  int boolean = 0;

  while (boolean == 0) {
    if (val[position] == 1) {
      val[position] = 0;
      boolean = 1;
    } else {
      val[position] = 1;
      position++;
    }
  }

  for (int i = bits - 1; i >= 0 ; i--) {
    if (val[i] == 1) {
      val[i] = 0;
    } else {
      val[i] = 1;
    }
  }

  long finalNumber = bToD(bits, val);
  free(val);

  return finalNumber;
}

long sToU(long number, int bits) {
  if (number >= 0) {
    return number;
  }

  long* val = malloc(sizeof(long) * bits);
  val = sToC(number, bits, val);

  long finalNumber = bToD(bits, val);

  free(val);

  return finalNumber * -1;
}

int main(int argc, char* argv[argc+1]) {
  FILE* f = fopen(argv[1], "r");

  long number;
  int bits;

  char from;
  char to;

  while (fscanf(f, "%ld %d %c %c\n", &number, &bits, &from, &to) != EOF) {
    long num;

    if (from == 'u') {
      num = uToS(number, bits);
    } else {
      num = sToU(number, bits);
    }

    printf("%ld\n", num);
  }

  fclose(f);
  return EXIT_SUCCESS;
}
