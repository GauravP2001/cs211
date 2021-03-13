#include <stdio.h>
#include <stdlib.h>

int isPrime(int n) {
  if (n < 2) {
    return 0;
  }

  for (int i = 2; i < n; i++) {
    if (n % i == 0) {
      return 0;
    }
  }

  return 1;
}

int main(int argc, char* argv[argc+1]) {

  int number;

  FILE* f = fopen(argv[1], "r");

  while (fscanf(f, "%d\n", &number) != EOF) {
    if (isPrime(number) == 1) {
      if(isPrime(number-2) == 1) {
        printf("yes\n");
      } else {
        if (isPrime(number+2) == 1) {
          printf("yes\n");
        } else {
          printf("no\n");
        }
      }
    } else {
      printf("no\n");
    }
  }

  fclose(f);
  return EXIT_SUCCESS;
}
