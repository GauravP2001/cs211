#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

char* binaryOutput(char hex) {

  char* temp = malloc(sizeof(char) * 5);

  switch (hex) {
    case '0':
        strcpy(temp, "0000");
        break;
    case '1':
        strcpy(temp, "1000");
        break;
    case '2':
        strcpy(temp, "0100");
        break;
    case '3':
        strcpy(temp, "1100");
        break;
    case '4':
        strcpy(temp, "0010");
        break;
    case '5':
        strcpy(temp, "1010");
        break;
    case '6':
        strcpy(temp, "0110");
        break;
    case '7':
        strcpy(temp, "1110");
        break;
    case '8':
        strcpy(temp, "0001");
        break;
    case '9':
        strcpy(temp, "1001");
        break;
    case 'A':
    case 'a':
        strcpy(temp, "0101");
        break;
    case 'B':
    case 'b':
        strcpy(temp, "1101");
        break;
    case 'C':
    case 'c':
        strcpy(temp, "0011");
        break;
    case 'D':
    case 'd':
        strcpy(temp, "1011");
        break;
    case 'E':
    case 'e':
        strcpy(temp, "0111");
        break;
    case 'F':
    case 'f':
        strcpy(temp, "1111");
        break;
  }

  return temp;
}

void hexToDecimal(int totalBits, int expBits, int fractionBits, char hex[20], int precision) {
//  printf("%s\n", hex);
  int len = strlen(hex);
  int array[totalBits];
  int expArray[expBits];
  int fractionArray[fractionBits];
  // int normalArray[precision+1];
  // int denormalArray[precision+1];

  char binaryString[(len-2) * 5];

  for (int i = len - 1; i >= 2; i--) {
     char* temp = binaryOutput(hex[i]);

     if (i == (len - 1)) {
       strcpy(binaryString, temp);
     } else {
       strcat(binaryString, temp);
     }

     free(temp);
  }

  // printf("Full array\n");
  // for (int i = 0; i < (len-2) * 4; i++) {
  //   printf("%c", binaryString[i]);
  // }
  // printf("\n");

  int j = 0;
  for (int i = totalBits - 1; i >= 0; i--) {    // adding all the bits to the totalArray in reverse
    int x =  binaryString[j] - '0';
    array[i] = x;
    j++;
  }

  // printf("Total Array (in reverse order): \n");
  // for (int i = totalBits - 1; i >=0; i--) {
  //   printf("%d", array[i]);
  // }
  // printf("\n\n");

  j = totalBits - 1;
  for (int i = fractionBits - 1; i >= 0; i--) {   // popullating the fractionArray
    fractionArray[i] = array[j];
    j--;
  }

  // printf("Fraction Array: \n");
  // for (int i  = 0; i < fractionBits; i++) {
  //   printf("%d", fractionArray[i]);
  // }
  // printf("\n");

  j = expBits;
  for (int i = expBits - 1; i >= 0; i--) {    // popullating the expArray
    expArray[i] = array[j];
    j--;
  }

  // printf("Exp Array: \n");
  // for (int i  = 0; i < expBits; i++) {
  //   printf("%d", expArray[i]);
  // }
  // printf("\n");

  int exp = 0;
//  int fraction = 0;

  int k = expBits - 1;
  for (int i = 0; i < expBits; i++) {
    if (expArray[i] == 1) {
      exp += ((1 << k));
    }

    k--;
  }
//  printf("The exp is: %d\n", exp);
  int normal = 1;
  if (exp == 0) { // if it is denormal
    normal = 0;
  }

  int bias = ((1 << (expBits - 1)) - 1);
//  printf("The bias is: %d\n", bias);
  double e = (exp - bias);
  // printf("The E is: %d\n", e);

  double power;
  if (normal == 1) {  // if it is a normal value
    power = pow(2.0, e);
//    printf("Number = %lf\n", power);
    e = e - 1.0;
  } else {
    power = 0.0;
  }

  for (int i = 0; i < fractionBits; i++) {
    if (fractionArray[i] == 1) {
      power += pow(2.0, e);
    }
    e = e - 1.0;
  }

  if (array[0] == 1) {
    printf("-");
  }

  char p[5];
  sprintf(p, "%d", precision);

  char z[10] = "%.";
  strcat(z, p);

  char lf[10] = "lf\n";

  strcat(z, lf);
  // printf("%s\n", z);

  printf(z, power);
}




int main (int argc, char* argv[argc+1]) {
  FILE* f = fopen(argv[1], "r");

  int totalBits;
  int expBits;
  int fractionBits;
  char hex[20];
  int precision;

  while (fscanf(f, "%d %d %d %s %d\n", &totalBits, &expBits, &fractionBits, hex, &precision) != EOF) {
    hexToDecimal(totalBits, expBits, fractionBits, hex, precision);
  }


  fclose(f);
  return EXIT_SUCCESS;
}
