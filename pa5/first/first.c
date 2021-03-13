#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

int memReads = 0;
int memWrites = 0;
int cacheHit = 0;
int cacheMiss = 0;

bool powerOfTwo(int n) {
   if (n == 0) {
     return false;
   }

   return (ceil(log2(n)) == floor(log2(n)));
}

struct Sets {
  int setNumber;
  int count;
  struct CacheLine* cacheLine;
};

struct CacheLine {
  int freq;
  long tag;
  struct CacheLine* nextCache;
};

struct Sets* allocateCache(struct Sets* array, int sets) {
  array = malloc(sizeof(struct Sets) * sets);

  for (int i = 0; i < sets; i++) {
    array[i].setNumber = i;
    array[i].cacheLine = 0;
    array[i].count = 0;
  }

  return array;
}

void freeList(struct Sets* array, int sets) {
  if (array == 0) {
    return;
  }

  struct CacheLine* temp, *temp2;

  for (int i = 0; i < sets; i++) {
    temp = array[i].cacheLine;

    if (temp == 0) {
      continue;
    } else {
      while (temp != 0) {
        temp2 = temp;
        temp = temp->nextCache;
        free(temp2);
      }
    }
  }

  free(array);
}

bool findTag(struct Sets* array, int sets, long tagValue, unsigned long setValue) {
  if (array == 0) {
    return false;
  }

  if (array[setValue].cacheLine == 0) {
    return false;
  }

  for (struct CacheLine* cache = array[setValue].cacheLine; cache != 0; cache = cache->nextCache) {
    if (cache->tag == tagValue) {
      return true;
    }
  }

  return false;
}

int returnCounter(struct Sets* array, int sets, long tagValue, unsigned long setValue) {
  if (array == 0) {
    return -1;
  }

  for (struct CacheLine* cache = array[setValue].cacheLine; cache != 0; cache = cache->nextCache) {
    if (cache->tag == tagValue) {
      return cache->freq;
    }
  }

  return -1;
}

bool isFull(struct Sets* array, int sets, int cacheLines, unsigned long setValue) {
  if (array == 0) {
    return false;
  }

  if (array[setValue].cacheLine == 0) {
    return false;
  }

  int counter = 0;
  struct CacheLine* cache = array[setValue].cacheLine;

  while (cache != 0 && counter < cacheLines) {
    cache = cache->nextCache;
    counter++;
  }

  if (counter < cacheLines) {
    return false;
  }

  return true;
}

int findLowestCounter(struct Sets* array, int sets, int cacheLines, unsigned long setValue) {
  int counter = 0;

  if (array[setValue].cacheLine == 0) {
    return -1;
  }

  struct CacheLine* temp = array[setValue].cacheLine;
  while (temp != 0) {
    if (temp->freq == 0) {
      return counter;
    }

    temp = temp->nextCache;
    counter++;
  }

  return -1;
}

int findHighestCounter(struct Sets* array, int sets, int cacheLines, unsigned long setValue) {
  int highest = 0;

  if (array[setValue].cacheLine == 0) {
    return -1;
  }

  struct CacheLine* temp = array[setValue].cacheLine;
  while (temp != 0) {
    if (temp->freq > highest) {
      highest = temp->freq;
    }

    temp = temp->nextCache;
  }

  return highest;
}

struct Sets* createCache(struct Sets* array, int setValue, long tagValue, int sets, int cacheLines, char command, char cachePolicy[20]) {

  if (findTag(array, sets, tagValue, setValue)) {
    if (strcmp(cachePolicy, "LRU")) {
      int frq = returnCounter(array, sets, tagValue, setValue);
      int highest = findHighestCounter(array, sets, cacheLines, setValue);
      for (struct CacheLine* temp = array[setValue].cacheLine; temp != 0; temp = temp->nextCache) {
        if (temp->freq == frq) {
          temp->freq = highest;
        } else if (temp->freq > frq) {
          temp->freq = temp->freq - 1;
        }
      }
    }

    cacheHit++;
    if (command == 'W') {
      memWrites++;
    }

    return array;
  } else {
    cacheMiss++;
  }

  if (command == 'R') {
    memReads++;
  } else {
    memReads++;
    memWrites++;
  }


  if (isFull(array, sets, cacheLines, setValue)) {  // eviction policy
    if (strcmp(cachePolicy, "fifo") == 0) { // FIFO

      struct CacheLine* temp = array[setValue].cacheLine;

      int counter = 0;
      while (counter < (array[setValue].count % cacheLines)) {
        temp = temp->nextCache;
        counter++;
      }

      temp->tag = tagValue;
      array[setValue].count = array[setValue].count + 1;
    } else {                              // LRU
      int counter = 0;
      int index = findLowestCounter(array, sets, cacheLines, setValue);
      struct CacheLine* temp = array[setValue].cacheLine;

      while (counter < index) {
        temp = temp->nextCache;
        counter++;
      }

      temp->tag = tagValue;

      for (struct CacheLine* temp = array[setValue].cacheLine; temp != 0; temp = temp->nextCache) {
        if (temp->tag != tagValue) {
          temp->freq = temp->freq - 1;
        } else {
          temp->freq = cacheLines - 1;
        }
      }
    }
  } else {
    struct CacheLine* temp = array[setValue].cacheLine;

    if (temp == 0) {
      temp = malloc(sizeof(struct CacheLine));
      temp->tag = tagValue;
      temp->nextCache = 0;
      temp->freq = 0;

      array[setValue].cacheLine = temp;
    } else {
      int setsCounter = 0;
      while (temp->nextCache != 0 && setsCounter < cacheLines) {
        temp = temp->nextCache;
        setsCounter++;
      }

      temp->nextCache = malloc(sizeof(struct CacheLine));
      temp->nextCache->tag = tagValue;
      temp->nextCache->nextCache = 0;
      temp->nextCache->freq = setsCounter + 1;
    }
  }

  return array;
}

int main(int argc, char* argv[argc+1]) {
  FILE* f = fopen(argv[5], "r");

  if (f == 0) {
   printf("error\n");
   return EXIT_SUCCESS;
  }

  if (argc != 6) {
   printf("error\n");
   fclose(f);
   return EXIT_SUCCESS;
  }

  int cacheSize = (atoi(argv[1]));
  char associativity[10];
  strcpy(associativity, argv[2]);
  char cachePolicy[20];
  strcpy(cachePolicy, argv[3]);
  int blockSize = (atoi(argv[4]));

  char command;
  unsigned long hex;

  struct Sets* array = 0;

 // FILE* f = fopen(argv[5], "r");

  if (argc != 6 || cacheSize == 0 || blockSize == 0 || powerOfTwo(cacheSize) == false || powerOfTwo(blockSize) == false) {
    printf("error\n");
    fclose(f);
    return EXIT_SUCCESS;
  }

  bool firstTime = true;
  while (fscanf(f, "%c %lx\n", &command, &hex) != EOF) {
      unsigned long blockOffset = log2(blockSize);
      unsigned long setBits;
      unsigned long setValue;
      long tagValue;
      if (associativity[0] == 'd') {
        setBits = log2((cacheSize / blockSize));
        setValue = ((hex >> blockOffset) & ((1 << setBits) - 1));
        tagValue = ((hex >> (blockOffset + setBits)));
        if (firstTime == true) {
          array = allocateCache(array, (cacheSize / blockSize));
          firstTime = false;
        }

        array = createCache(array, setValue, tagValue, (cacheSize / blockSize), 1, command, cachePolicy);
      } else if (associativity[5] == 0) {
        setBits = log2(1);
        setValue = ((hex >> blockOffset) & ((1 << setBits) - 1));
        tagValue = ((hex >> (blockOffset + setBits)));
        if (firstTime == true) {
          array = allocateCache(array, 1);
          firstTime = false;
        }

        array = createCache(array, setValue, tagValue, 1, (cacheSize / blockSize), command, cachePolicy);
      } else {
        int nWay = atoi(&associativity[strlen(associativity) - 1]);

        setBits = log2((cacheSize / (blockSize * nWay)));
        setValue = ((hex >> blockOffset) & ((1 << setBits) - 1));
        tagValue = ((hex >> (blockOffset + setBits)));
        if (firstTime == true) {
          array = allocateCache(array, (cacheSize / (blockSize * nWay)));
          firstTime = false;
        }

        array = createCache(array, setValue, tagValue, (cacheSize / (blockSize * nWay)), nWay, command, cachePolicy);
      }
  }

  printf("memread:%d\n", memReads);
  printf("memwrite:%d\n", memWrites);
  printf("cachehit:%d\n", cacheHit);
  printf("cachemiss:%d\n", cacheMiss);

  if (associativity[0] == 'd') {
    freeList(array, (cacheSize / blockSize));
  } else if (associativity[5] == 0) {
    freeList(array, 1);
  } else {
    freeList(array, (cacheSize / (blockSize * (atoi(&associativity[strlen(associativity) - 1])))));
  }
	
  fclose(f);
  return EXIT_SUCCESS;
}

