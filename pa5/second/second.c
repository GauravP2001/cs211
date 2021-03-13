#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

struct Sets* array1 = 0;
struct Sets* array2 = 0;

int memReads = 0;
int memWrites = 0;
int l1cacheHit = 0;
int l1cacheMiss = 0;
int l2cacheHit = 0;
int l2cacheMiss = 0;

int fifoCounter = 0;

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
  unsigned long hex;
  int setValue;
  int freq;
  int fifoCount;
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

long getTag(unsigned long hex, int blockSize, int cacheSize, char associativity[20]) {

  unsigned long setBits;
  unsigned long blockOffset = log2(blockSize);

  if (associativity[0] == 'd') {
    setBits = log2((cacheSize / blockSize));
  } else if (associativity[5] == 0) {
    setBits = log2(1);
  } else {
    setBits = log2((cacheSize / (blockSize * atoi(&associativity[strlen(associativity) - 1]))));
  }

  unsigned long tagValue = ((hex >> (blockOffset + setBits)));

  return tagValue;
}

long getSet(unsigned long hex, int blockSize, int cacheSize, char associativity[20]) {

  unsigned long setBits;
  unsigned long blockOffset = log2(blockSize);

  if (associativity[0] == 'd') {
    setBits = log2((cacheSize / blockSize));
  } else if (associativity[5] == 0) {
    setBits = log2(1);
  } else {
    setBits = log2((cacheSize / (blockSize * atoi(&associativity[strlen(associativity) - 1]))));
  }
  return ((hex >> blockOffset) & ((1 << setBits) - 1));
}

bool findTag(struct Sets* array, int cacheSize, char associativity[10], int sets, int blockSize, long tagValue, int setValue) {
  for (struct CacheLine* cache = array[setValue].cacheLine; cache != 0; cache = cache->nextCache) {
    long tag = getTag(cache->hex, blockSize, cacheSize, associativity);
    // printf("Comparing %ld and %ld\n", tag, tagValue);
    if (tag == tagValue && cache->setValue == setValue) {
      return true;
    }
  }

  return false;
}

int returnPreviousCounter(struct Sets* array, int blockSize, int cacheSize, char associativity[10], int sets, long tagValue, int setValue) {
  for (struct CacheLine* cache = array[setValue].cacheLine; cache != 0; cache = cache->nextCache) {
    long tag = getTag(cache->hex, blockSize, cacheSize, associativity);
    if (tag == tagValue) {
      return cache->freq;
    }
  }

  return -1;
}

int findLowestCounter(struct Sets* array, int sets, int cacheLines, int setValue) {
  int counter = 0;
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

int findLowestFifoCounter(struct Sets* array, int sets, int cacheLines, int setValue) {
  int counter = 0;
  int index = 0;
  int low = 2147483647;
  struct CacheLine* temp = array[setValue].cacheLine;
  while (temp != 0) {
    if (temp->fifoCount < low) {
      low  = temp->fifoCount;
      index = counter;
    }

    temp = temp->nextCache;
    counter++;
  }

  return index;
}

bool isFull(struct Sets* array, int sets, int cacheLines, unsigned long setValue) {
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

int findHighestCounter(struct Sets* array, int sets, int cacheLines, unsigned long setValue) {
  int highest = 0;
  struct CacheLine* temp = array[setValue].cacheLine;
  while (temp != 0) {
    if (temp->freq > highest) {
      highest = temp->freq;
    }

    temp = temp->nextCache;
  }

  return highest;
}

void deleteNode(struct Sets* array, int blockSize, int cacheSize, char associativity[10], long tagValue, int setValue, int cacheLines2) {
  struct CacheLine* front = array[setValue].cacheLine;
  struct CacheLine* back = array[setValue].cacheLine;

  // 0 --> x
  // 0 --> a --> b --> c
  // 0 --> b --> c

  int counter = 0;
  while (front != 0) {    // deleting the node
    long tag = getTag(front->hex, blockSize, cacheSize, associativity);
    // printf("Current Tag is: %ld and tagValue is %ld\n", tag, tagValue);
    if (tag == tagValue && array[setValue].cacheLine->nextCache == 0) { // if there is only one node in the set
      array[setValue].cacheLine = 0;
      array[setValue].count = 0;
      free(front);
      break;
    } else if (tag == tagValue && counter == 0) { // if its the fist node
      array[setValue].cacheLine = front->nextCache;
      free(front);

      if (array[setValue].count % cacheLines2 > counter) {
        array[setValue].count--;
      }
      break;
    } else if (tag == tagValue && front->nextCache == 0) {  // last node we are trying to delete
      back->nextCache = 0;
      free(front);

      if (counter == array[setValue].count % cacheLines2) {
        array[setValue].count = 0;
      }
      break;
    } else if (tag == tagValue) { // somewhere in the middle
      back->nextCache = front->nextCache;
      free(front);

      if (array[setValue].count % cacheLines2 > counter) {
        array[setValue].count--;
      }
      break;
    } else {
      back = front;
      front = front->nextCache;
      counter++;
    }
  }
}

unsigned long insertNode(struct Sets* array, unsigned long hexa, int blockSize, int cacheSize, int sets, int cacheLines, char associativity[10], char cachePolicy[20], long tagValue, int setValue) {
  unsigned long tempHex = 0;
  if(isFull(array, sets, cacheLines, setValue)) { // if L1 is full, activate the eviction polocies
    if (strcmp(cachePolicy, "fifo") == 0) {
      struct CacheLine* temp = array[setValue].cacheLine;

      int counter = 0;
      int lowestIndex = findLowestFifoCounter(array, sets, cacheLines, setValue);
      while (counter < lowestIndex) {
        temp = temp->nextCache;
        counter++;
      }

      tempHex = temp->hex;

      temp->hex = hexa;
      temp->setValue = setValue;

      temp->fifoCount = ++fifoCounter;

      array[setValue].count = array[setValue].count + 1;

      if (array[setValue].count >= cacheLines) {
        array[setValue].count = 0;
      }
    } else {  // LRU for adding the deleted node from L2 to L1
      int counter = 0;
      int index = findLowestCounter(array, sets, cacheLines, setValue);
      struct CacheLine* temp = array[setValue].cacheLine;

      while (counter < index) {
        temp = temp->nextCache;
        counter++;
      }

      tempHex = temp->hex;
      temp->hex = hexa;
      temp->setValue = setValue;
      // printf("Hex after is %ld\n", temp->hex);

      for (struct CacheLine* temp = array[setValue].cacheLine; temp != 0; temp = temp->nextCache) {
        // long tag = getTag(temp->hex, blockSize, cacheSize, associativity);
        // printf("Temp->Hex: %lx | tag: %ld tagValue: %ld\n", temp->hex, tag, tagValue);
        if (temp->hex != hexa) {
          temp->freq = temp->freq - 1;
        } else {
          temp->freq = cacheLines - 1;
        }
      }
    }
  } else {  // if it is not full
    struct CacheLine* temp = array[setValue].cacheLine;

    if (temp == 0) {
      temp = malloc(sizeof(struct CacheLine));
      temp->hex = hexa;
      temp->setValue = setValue;
      temp->nextCache = 0;
      temp->freq = 0;
      temp->fifoCount = fifoCounter;

      array[setValue].cacheLine = temp;
    } else {
      int cacheCounter = 0;
      while (temp->nextCache != 0 && cacheCounter < cacheLines) {
        temp = temp->nextCache;
        cacheCounter++;
      }

      temp->nextCache = malloc(sizeof(struct CacheLine));
      temp->nextCache->hex = hexa;
      temp->nextCache->setValue = setValue;
      temp->nextCache->nextCache = 0;
      temp->nextCache->freq = cacheCounter + 1;
      temp->nextCache->fifoCount = fifoCounter;
    }

    fifoCounter++;
  }

  return tempHex;
}


void createCache(unsigned long hex, int blockSize, int cacheSize1, int cacheSize2, int setValue1, int sets1, int cacheLines1, int setValue2, int sets2, int cacheLines2, char command, char cachePolicy1[20], char cachePolicy2[20], char associativity1[10], char associativity2[10]) {
  long tagValue1 = getTag(hex, blockSize, cacheSize1, associativity1);
  long tagValue2 = getTag(hex, blockSize, cacheSize2, associativity2);

  if (findTag(array1, cacheSize1, associativity1, sets1, blockSize, tagValue1, setValue1)) { // if the given address is in L1
    if (strcmp(cachePolicy1, "LRU") || strcmp(cachePolicy1, "lru")) { // update LRU counters
      int frq = returnPreviousCounter(array1, blockSize, cacheSize1, associativity1, sets1, tagValue1, setValue1);
      // printf("Old counter is %d\n", frq);
      int highest = findHighestCounter(array1, sets1, cacheLines1, setValue1);

      for (struct CacheLine* temp = array1[setValue1].cacheLine; temp != 0; temp = temp->nextCache) {
        if (temp->freq == frq) {
          temp->freq = highest;
          // printf("Highest counter is %d\n", temp->freq);
        } else if (temp->freq > frq) {
          temp->freq = temp->freq - 1;
        }
      }
    }

    l1cacheHit++;
    if (command == 'W') {
      memWrites++;
    }

    return;
  } else if (findTag(array2, cacheSize2, associativity2, sets2, blockSize, tagValue2, setValue2)) {  //if it is found in L2

    int frq = returnPreviousCounter(array2, blockSize, cacheSize2, associativity2, sets2, tagValue2, setValue2);
    deleteNode(array2, blockSize, cacheSize2, associativity2, tagValue2, setValue2, cacheLines2);  // delete the node from L2

    for (struct CacheLine* temp = array2[setValue2].cacheLine; temp != 0; temp = temp->nextCache) { //updating LRU counters
      if (temp->freq > frq) {
        temp->freq = temp->freq - 1;
      }
    }

    // Now, have to insert the node in L1

    if (isFull(array1, sets1, cacheLines1, setValue1)) { // if L1 is full, activate the eviction polocies
      unsigned long tempHex;
      // Inserting the deleted block in L1
      tempHex = insertNode(array1, hex, blockSize, cacheSize1, sets1, cacheLines1, associativity1, cachePolicy1, tagValue1, setValue1);

      int setVal = getSet(tempHex, blockSize, cacheSize2, associativity2);
      insertNode(array2, tempHex, blockSize, cacheSize2, sets2, cacheLines2, associativity2, cachePolicy2, tagValue2, setVal);

    } else {
      insertNode(array1, hex, blockSize, cacheSize1, sets1, cacheLines1, associativity1, cachePolicy1, tagValue1, setValue1);
    }

    l1cacheMiss++;
    l2cacheHit++;

    if (command == 'W') {
      memWrites++;
    }

    return;
  } else {  // if its not in L1 and L2
    unsigned long tempHex;
    tempHex = insertNode(array1, hex, blockSize, cacheSize1, sets1, cacheLines1, associativity1, cachePolicy1, tagValue1, setValue1);

    if (tempHex != 0) {
      int setVal = getSet(tempHex, blockSize, cacheSize2, associativity2);
      insertNode(array2, tempHex, blockSize, cacheSize2, sets2, cacheLines2, associativity2, cachePolicy2, tagValue2, setVal);
    }

    l1cacheMiss++;
    l2cacheMiss++;
  }

  if (command == 'R') {
    memReads++;
  } else {
    memReads++;
    memWrites++;
  }
}

int main(int argc, char* argv[argc+1]) {
  FILE* f = fopen(argv[8], "r");

  if (f == 0) {
   printf("error\n");
   return EXIT_SUCCESS;
  }

  if (argc != 9) {
   printf("error\n");
   fclose(f);
   return EXIT_SUCCESS;
  }
  

  int cacheSize1 = (atoi(argv[1]));
  char associativity1[10];
  strcpy(associativity1, argv[2]);
  char cachePolicy1[20];
  strcpy(cachePolicy1, argv[3]);
  int blockSize = (atoi(argv[4]));

  int cacheSize2 = (atoi(argv[5]));
  char associativity2[10];
  strcpy(associativity2, argv[6]);
  char cachePolicy2[20];
  strcpy(cachePolicy2, argv[7]);

  char command;
  unsigned long hex;

//  FILE* f = fopen(argv[8], "r");

  if (argc != 9 || cacheSize1 == 0 || cacheSize2 == 0 || blockSize == 0 || powerOfTwo(cacheSize1) == false || powerOfTwo(cacheSize2) == false || powerOfTwo(blockSize) == false) {
    printf("error\n");
    fclose(f);
    return EXIT_SUCCESS;
  }

  int sets1;
  int sets2;

  int cacheLines1;
  int cacheLines2;

  int setBits1;
  int setBits2;

  int setVal1;
  int setVal2;

  int blockOffset = log2(blockSize);

  bool firstTime = true;
  bool firstTime2 = true;

  while (fscanf(f, "%c %lx\n", &command, &hex) != EOF) {
    //first cache
    if (associativity1[0] == 'd') {
      sets1 = (cacheSize1 / blockSize);
      cacheLines1 = 1;
      setBits1 = log2(sets1);
      setVal1 = ((hex >> blockOffset) & ((1 << setBits1) - 1));
      if (firstTime == true) {
        array1 = allocateCache(array1, sets1);
        firstTime = false;
      }
    } else if (associativity1[5] == 0) {
      sets1 = 1;
      cacheLines1 = (cacheSize1 / blockSize);
      setBits1 = log2(sets1);
      setVal1 = ((hex >> blockOffset) & ((1 << setBits1) - 1));
      if (firstTime == true) {
        array1 = allocateCache(array1, sets1);
        firstTime = false;
      }
    } else {
      int n = atoi(&associativity1[strlen(associativity1) - 1]);
      sets1 = (cacheSize1 / (blockSize * n));
      cacheLines1 = n;
      setBits1 = log2(sets1);
      setVal1 = ((hex >> blockOffset) & ((1 << setBits1) - 1));
      if (firstTime == true) {
        array1 = allocateCache(array1, sets1);
        firstTime = false;
      }
    }

    //second cache

    if (associativity2[0] == 'd') {
      sets2 = (cacheSize2 / blockSize);
      cacheLines2 = 1;
      setBits2 = log2(sets2);
      setVal2 = ((hex >> blockOffset) & ((1 << setBits2) - 1));
      if (firstTime2 == true) {
        array2 = allocateCache(array2, sets2);
        firstTime2 = false;
      }
    } else if (associativity2[5] == 0) {
      sets2 = 1;
      cacheLines2 = (cacheSize2 / blockSize);
      setBits2 = log2(sets2);
      setVal2 = ((hex >> blockOffset) & ((1 << setBits2) - 1));
      if (firstTime2 == true) {
        array2 = allocateCache(array2, sets2);
        firstTime2 = false;
      }
    } else {
      int n = atoi(&associativity2[strlen(associativity2) - 1]);
      sets2 = (cacheSize2 / (blockSize * n));
      cacheLines2 = n;
      setBits2 = log2(sets2);
      setVal2 = ((hex >> blockOffset) & ((1 << setBits2) - 1));
      if (firstTime2 == true) {
        array2 = allocateCache(array2, sets2);
        firstTime2 = false;
      }
    }

    createCache(hex, blockSize, cacheSize1, cacheSize2, setVal1, sets1, cacheLines1, setVal2, sets2, cacheLines2, command, cachePolicy1, cachePolicy2, associativity1, associativity2);
  }

  printf("memread:%d\n", memReads);
  printf("memwrite:%d\n", memWrites);
  printf("l1cachehit:%d\n", l1cacheHit);
  printf("l1cachemiss:%d\n", l1cacheMiss);
  printf("l2cachehit:%d\n", l2cacheHit);
  printf("l2cachemiss:%d\n", l2cacheMiss);

  if (associativity1[0] == 'd') {
    freeList(array1, (cacheSize1 / blockSize));
  } else if (associativity1[5] == 0) {
    freeList(array1, 1);
  } else {
    freeList(array1, (cacheSize1 / (blockSize * (atoi(&associativity1[strlen(associativity1) - 1])))));
  }

  if (associativity2[0] == 'd') {
    freeList(array2, (cacheSize2 / blockSize));
  } else if (associativity2[5] == 0) {
    freeList(array2, 1);
  } else {
    freeList(array2, (cacheSize2 / (blockSize * (atoi(&associativity2[strlen(associativity2) - 1])))));
  }

  fclose(f);
  return EXIT_SUCCESS;
}

