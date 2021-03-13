#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void convertToIEE(double number, int totalBits, int expBits, int fractionalBits) {
//  printf("\nThe number is %.10lf\n", number);
  int totalArray[totalBits];
  int binaryArray[52];
  int expArray[52];
  int counter;
  double value;

  if (number >= 1 || fabs(number) >= 1) {
    if (number > 1) {
      totalArray[0] = 0;
    } else {
      totalArray[0] = 1;
      number = number * -1;
    }


    value = 1.0;
    counter = 0;
    double num = number;

    while (number > value * 2) {      // getting the higest value before number
        value *= 2;
        counter++;
    }

    for (int i = 0; i < 52; i++) {          // adding the binary digits to the binaryArray
      if (value <= num) {
        binaryArray[i] = 1;
        num = num - value;
      } else {
        binaryArray[i] = 0;
      }

      value = value / 2;
    }

    // printf("Exponent is %d\n", counter);
    // printf("Binary Array:\n");
    // for (int i = 0; i < 52; i++) {
    //   printf("%d ", binaryArray[i]);
    // }
    // printf("\n");

    // Getting the exponent now
  } else {
    if ((number * -1) < 0) {
      totalArray[0] = 0;
    } else {
      totalArray[0] = 1;
      number = number * -1;
    }


    value = 0.5;
    counter = -1;
    double num = number;
    int boolean = 0;

    while (number > value * 2) {
        value /= 2;
    }

    for (int i = 0; i < 52; i++) {
      if (value <= num) {
        binaryArray[i] = 1;
        num = num - value;

        if (boolean == 0) {
          boolean = 1;
        }
      } else {
        binaryArray[i] = 0;

        if (boolean != 1) {
          counter--;
        }
      }

      value = value / 2;
    }


    // printf("Exponent is %d\n", counter);
    // printf("Binary Array:\n");
    // for (int i = 0; i < 52; i++) {
    //   printf("%d ", binaryArray[i]);
    // }
    // printf("\n");

    // Getting the exp now
  }
  // printf("Binary Array: \n");
  // for (int i = 0; i < 52; i++) {
  //   printf("%d", binaryArray[i]);
  // }
  // printf("\n");

  value = 1.0;
//  int normal = 1;
  int bias = ((1 << (expBits - 1)) - 1);
//  printf("The Bias is %d\n", bias);

//  printf("Counter is %d\n", counter);

  int start;
  int end;
  if (number >= 1 || (fabs(number) >= 1)) {
    start = 1;
    end = 52 - (counter + 1);
  } else {
    if ((counter) < (1 - bias)) {
      start = abs(counter + 1);
      end = 52 - abs(counter + 1);
    } else {
      start = abs(counter);
      end = 52 - abs(counter);
    }
  }
  // printf("Starting at index %d\n", start);
  // printf("Ending at index %d\n", end);
  int temp[52 - (counter + 1)];

  for (int i = 0; i < end; i++) {
    temp[i] = binaryArray[start];
    start++;
  }

  // printf("The rounding array: \n");
  // for (int i = 0; i < end; i++) {
  //   printf("%d", temp[i]);
  // }
  // printf("\n");

  // int round = rounding(temp, end, fractionalBits);

  int roundDownArray[fractionalBits];
  int roundUpArray[fractionalBits];
  double x1 = 0.0;
  double x2 = 0.0;
  double originalDistance = 0.0;
  double val = 0.5;
  int round = 0;

  for (int i = 0; i < fractionalBits; i++) {
    roundDownArray[i] = temp[i];
    roundUpArray[i] = temp[i];
  }

  for (int i = 0; i < end; i++) {
    if (temp[i] == 1) {
      originalDistance += val;
    }
    val /= 2.0;
  }

  // printf("Round Down Array: \n");
  // for (int i = 0; i < fractionalBits; i++) {
  //   printf("%d", roundDownArray[i]);
  // }
  // printf("\n");
  //
  // printf("The original distance is: %lf\n", originalDistance);

  // int carryExponent;
  int carryOver = 0;
  int num = 1;
  for (int i = fractionalBits - 1; i >= 0; i--) { // add one to the lower array
    if (roundDownArray[i] == 1 && carryOver == 0 && num == 1) {
      roundUpArray[i] = 0;
      carryOver++;
      num--;
    } else if (roundDownArray[i] == 0 && carryOver == 0 && num == 1) {
      roundUpArray[i] = 1;
      num--;
    } else if(roundDownArray[i] == 0 && carryOver >= 1 && num == 0) {
      roundUpArray[i] = 1;
      carryOver--;
    } else if (roundDownArray[i] == 1 && carryOver == 0 && num == 0) {
      roundUpArray[i] = 1;
    } else if (roundDownArray[i] == 0 && carryOver == 0 && num == 0) {
      roundUpArray[i] = 0;
    } else if (roundDownArray[i] == 1 && carryOver >= 1 && num == 0) {
      roundUpArray[i] = 0;
      carryOver++;
    }
  }

  // printf("Round Up Array: \n");
  // for (int i = 0; i < fractionalBits; i++) {
  //   printf("%d", roundUpArray[i]);
  // }
  // printf("\n");


  double value2 = .5;
  for (int i = 0; i < fractionalBits; i++) {
    if (roundDownArray[i] == 1) {
      x1 += value2;
    }
    value2 /= 2.0;
  }
//  printf("X1 is %lf\n", x1);

  double value3 = .5;
  for (int i = 0; i < fractionalBits; i++) {
    if (roundUpArray[i] == 1) {
      x2 += value3;
    }
    value3 /= 2.0;
  }
//  printf("X2 is %lf\n", x2);

  if ( ((x1 + x2) / 2.0) == originalDistance) {
    if (roundDownArray[fractionalBits - 1] == 0 && roundUpArray[fractionalBits - 1] != 0) {
  //    printf("Returning 0\n");
      round = 0;
    } else if (roundDownArray[fractionalBits - 1] != 0 && roundUpArray[fractionalBits - 1] == 0) {
//      printf("Returning 1\n");
      round = 1;
    }
  } else if (x1 == originalDistance) {
//    printf("Returning 0\n");
    round = 0;
  } else {
    if ((originalDistance - x1) < (originalDistance - x2) && carryOver < 3) {    // return 0 = round down, return 1 = round up, return 2 = round up the exp
//      printf("Returning 0\n");
      round = 0;
    } else if (( originalDistance - x1) > (x2 - originalDistance) && carryOver < 3) {
//      printf("Returning 1\n");
      round = 1;
    } else if (carryOver >= 3) {
      if ((originalDistance - x1) < (originalDistance - x2)) {
  //      printf("Add exponent and less than\n");
        round = 3;
      } else if (( originalDistance - x1) > (x2 - originalDistance)) {
//        printf("Add exponent and more than\n");
        round = 4;
      }
    }
  }

  if (round == 3 || round == 4) {
    counter++;
  }

  int exp = (counter + (1 << (expBits - 1)) - 1);


//  printf("The exp is %d\n", exp);


//  printf("Comparing %d and %d\n", counter, (1-bias));
  if ((counter) < (1 - bias)) {
    for (int i = 1; i <= expBits; i++) {  // adding 0 to the exp part of the main array
      totalArray[i] = 0;
    }
  //  normal = 0;
  } else {
    int c = 0;
  //  normal = -1;
    while (exp > 0 && c < 52) {
      expArray[c] = exp % 2;
      exp = exp / 2;
      c++;
    }

    while (c < 52) {
      expArray[c] = 0;
      c++;
    }

    // printf("The exponent array is: \n");
    // for (int i = 0; i < 52; i++) {
    //   printf("%d ", expArray[i]);
    // }
    // printf("\n");

    int j = 1;
    for (int i = expBits - 1; i >= 0; i--) {
      totalArray[j] = expArray[i];
      j++;
    }
  }

  // printf("The counter is %d\n", counter);
//  int j;
//  if (normal == 1) {
//    if (counter < 0) {
//      j = (counter * -1);
//    } else {
//      j = 1;
//    }

//  } else {
//    j = ((counter* -1) - 1);
//  }

//  printf("Round is %d\n", round);
  int x = 0;
  if (round == 0) {
    for (int i = expBits+1; i < totalBits; i++) {
      totalArray[i] = roundDownArray[x];
      x++;
    }
  } else if (round == 1 || round == 4 || round == 3) {
    for (int i = expBits+1; i < totalBits; i++) {
      totalArray[i] = roundUpArray[x];
      x++;
    }
  }

  for (int i = 0; i < totalBits; i++) {
    printf("%d", totalArray[i]);
  }
  printf("\n");
 }

int main(int argc, char* argv[argc+1]) {
  FILE* f = fopen(argv[1], "r");

  double number;
  int totalBits;
  int expBits;
  int fractionalBits;

  while (fscanf(f, "%lf %d %d %d\n", &number, &totalBits, &expBits, &fractionalBits) != EOF) {
    convertToIEE(number, totalBits, expBits, fractionalBits);
  }


  fclose(f);
  return EXIT_SUCCESS;
}
