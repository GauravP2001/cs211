#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<limits.h>

struct Ind* stack = 0;

struct Node {
  char name[30];
  struct Edge* next;
  int visited;
  int distance;
  int inRecursion;
};

struct Edge {
  char edgeName[30];
  struct Node* originalVertex;
  struct Edge* next;
  int weight;
};

struct Ind {
  int index;
  struct Ind* next;
};

struct Edge* allocate(struct Node* head, char vertex[], int index) {
  struct Edge* temp = malloc(sizeof(struct Edge));

  strcpy(temp->edgeName, vertex);
  temp->originalVertex = &(head[index]);
  temp->next = head[0].next;

  return temp;
}

struct Node* allocateList(struct Node* head, int num) {
  head = malloc(sizeof(struct Node) * num);

  for (int i = 0; i < num; i++) {
    head[i].next = 0;
  }

  return head;
}

void printList(struct Node* head, int num) {
  printf("FINAL LIST\n");
  for (int i = 0; i < num; i++) {
    printf("%s -> ", head[i].name);
    for (struct Edge* edge = head[i].next; edge != 0; edge = edge->next) {
      printf("%s(%d) ", edge->edgeName, edge->weight);
    }

    printf("\n");
  }
  printf("\n");
}

void freeList(struct Node* head, int num) {
  if (head == 0) {
    return;
  }

  struct Edge* temp, *temp2;

  for (int i = 0; i < num; i++) {
    temp = head[i].next;

    if (temp == 0) {
      continue;
    } else {
      while (temp != 0) {
        temp2 = temp;
        temp = temp->next;
        free(temp2);
      }
    }
  }

  free(head);
}
void freeStack(struct Ind* head) {
  if (head == 0) {
    return;
  }

  struct Ind* temp, *temp2;
  temp = head;

  if (temp == 0) {
    return;
  } else {
    while (temp != 0) {
      temp2 = temp;
      temp = temp->next;
      free(temp2);
    }
  }
}


int indexOfVertex(struct Node* head, char vertexName[], int num) {
  for (int i = 0; i < num; i++) {
    if (strcmp(head[i].name, vertexName) == 0) {
      return i;
    }
  }

  return -1;
}

struct Node* addEdge(struct Node* head, char vertexName[], char eName[], int weight, int num) {
  int index = indexOfVertex(head, vertexName, num);
  if (index == -1) {
    return head;
  }

  struct Edge* temp = malloc(sizeof(struct Edge));
  strcpy(temp->edgeName, eName);
  temp->next = 0;
	temp->weight = weight;
  temp->originalVertex = &(head[indexOfVertex(head, eName, num)]);
  // printf("The edge added is %s and it points to %s\n", eName, temp->originalVertex->name);

  struct Edge* ptr = head[index].next;

  if (ptr == 0) {   // if its the first node
    head[index].next = temp;

    return head;
  } else if (ptr->next == 0) {
    if (strcmp(eName, ptr->edgeName) > 0) {
      ptr->next = temp;
    } else {
    temp->next = ptr;
    head[index].next = temp;
  }

    return head;
  }

  struct Edge* ptr2 = head[index].next->next;

  while (ptr2 != 0) { //if there are less than 2 nodes
    if (strcmp(eName, ptr2->edgeName) == 0) {  // if the items names match
      continue;
    } else if (strcmp(eName, ptr2->edgeName) < 0 && strcmp(eName, ptr->edgeName) < 0) { // if the vertex we are trying to enter is less than the one it is being compared to (check if its the first edge node using the index of the original node)
      if (strcmp(head[index].next->edgeName, ptr->edgeName) == 0) {
        temp->next = ptr;
        head[index].next = temp;

        return head;
      }

      return head;        // if it is not the first item, so it is in the middle

    } else if (strcmp(eName, ptr2->edgeName) < 0 && strcmp(eName, ptr->edgeName) > 0) {  // goes in the middle of ptrs
      temp->next = ptr2;
      ptr->next = temp;

      return head;
    } else if (strcmp(eName, ptr2->edgeName) > 0) { // if the item being inserted is greater than the one being compared to
      if (ptr2->next == 0) {
        ptr2->next = temp;

        return head;
      }

      ptr = ptr->next;
      ptr2 = ptr2->next;
    } else {

    }
  }

  return head;
}

void push(int index) {
  struct Ind* temp = 0;
  temp = malloc(sizeof(struct Ind));
  temp->index = index;
  temp->next = 0;

  if (stack == 0) {
    stack = temp;
    return;
  }

  temp->next = stack;
  stack = temp;
  return;
}

int pop() {
  if (stack == 0) {
    return -1;
  }

  struct Ind* temp = stack;
  stack = stack->next;
  int ind = temp->index;
  free(temp);
  return ind;
}

void topologicalSort(struct Node* head, int index, int num) {
  head[index].visited = 1;

  for (struct Edge* ptr = head[index].next; ptr != 0; ptr = ptr->next) {
    if (ptr->originalVertex->visited == 0) {
      topologicalSort(head, indexOfVertex(head, ptr->edgeName, num), num);
    }
  }

  push(index);
}

int cyclicUtil(struct Node* head, int num, int index) {
  if (head[index].inRecursion == 1){
    return 1;
  }

  if (head[index].visited == 1) {
    return 0;
  }

  head[index].visited = 1;
  head[index].inRecursion = 1;

  for (struct Edge* edge = head[index].next; edge != 0; edge = edge->next) {
    if (cyclicUtil(head, num, indexOfVertex(head, edge->edgeName, num)) == 1) {
      return 1;
    }
  }

  head[index].inRecursion = 0;
  return 0;

}

int cyclic(struct Node* head, int num) {
  for (int i = 0 ; i < num; i++) {
    head[i].visited = 0;
    head[i].inRecursion = 0;
  }

  for (int i = 0; i < num; i++) {
    if (cyclicUtil(head, num, i) == 1) {
      return 1;
    }
  }

  return 0;
}

void shortestPath(struct Node* head, char source[], int num) {
  printf("\n");

  stack = 0;

  for (int i = 0; i< num; i++) {
    head[i].visited = 0;
  }

  for (int i = 0; i < num; i++) {
    if (head[i].visited == 0) {
      // printf("Passing in the edges of %s node and the first edge is %s\n", head[i].name, head[i].next->edgeName);
      topologicalSort(head, i, num);
    }
  }

  for (int i = 0; i < num; i++) {
    head[i].distance = INT_MAX;
  }

  head[indexOfVertex(head, source, num)].distance = 0;

  while (stack != 0) {
    // struct Edge* temp = pop();
    struct Node* node = &(head[pop()]);

    if (node->distance != INT_MAX) {
      for (struct Edge* ptr = node->next; ptr != 0; ptr = ptr->next) {
        if (ptr->originalVertex->distance > (node->distance + ptr->weight)) {
          ptr->originalVertex->distance = (node->distance + ptr->weight);
        }
      }
    }
  }

  for (int i = 0; i < num; i++) {
    if (head[i].distance == INT_MAX) {
      printf("%s INF\n", head[i].name);
    } else {
      printf("%s %d\n", head[i].name, head[i].distance);
    }
  }

  freeStack(stack);
}


int main(int argc, char* argv[argc+1]) {
  int num;
  char vertex[30];

  FILE* f = fopen(argv[1], "r");
  FILE* f2 = fopen(argv[2], "r");

  fscanf(f, "%d\n", &num);  // take in the number of vertex's

  struct Node* head = 0;
  head = allocateList(head, num); // allocating n spaces for the nodes

  for (int i = 0; i < num; i++) {   // scanning the vertex's and adding them to the array
    fscanf(f, "%s\n", vertex);
    strcpy(head[i].name, vertex);
    head[i].visited = 0;
    head[i].distance = 0;
  }

  for (int i = 0; i < num - 1; i++) {
    for (int j = 0; j < num - 1 - i; j++) {
      if (strcmp(head[j].name, head[j+1].name) > 0) { // second item is bigger
        char temp[30];
        strcpy(temp, head[j].name);

        strcpy(head[j].name, head[j+1].name);
        strcpy(head[j+1].name, temp);
      }
    }
  }

  char vertexName[30];
  char edgeName[30];
	int weight;

  while (fscanf(f, "%s %s %d\n", vertexName, edgeName, &weight) != EOF) {
    head = addEdge(head, vertexName, edgeName, weight, num);
  }

  char source[30];


  if (cyclic(head, num) == 1) {
    printf("\n");
    printf("CYCLE\n");

    fclose(f);
    fclose(f2);
    freeList(head, num);
    freeStack(stack);
    free(stack);
    return EXIT_SUCCESS;
  }


  while (fscanf(f2, "%s\n", source) != EOF) {
    shortestPath(head, source, num);
  }

  fclose(f);
  fclose(f2);
  freeList(head, num);
  freeStack(stack);
  free(stack);
  return EXIT_SUCCESS;
}
