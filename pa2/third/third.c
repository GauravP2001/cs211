#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Edge* queue = 0;

struct Node {
  char name[30];
  struct Edge* next;
  int visited;
};

struct Edge {
  char edgeName[30];
  struct Node* originalVertex;
  struct Edge* next;
};

struct Edge* allocate(struct Node* head, char vertex[], int index) {
  struct Edge* temp = malloc(sizeof(struct Edge));

  strcpy(temp->edgeName, vertex);
  temp->originalVertex = &(head[index]);
  temp->next = 0;

  return temp;
}

struct Node* allocateList(struct Node* head, int num) {
  head = malloc(sizeof(struct Node) * num);

  for (int i = 0; i < num; i++) {
    head[i].next = 0;
  }

  return head;
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

// void printList(struct Node* head, int num) {
//   printf("FINAL LIST\n");
//   for (int i = 0; i < num; i++) {
//     printf("%s -> ", head[i].name);
//     for (struct Edge* edge = head[i].next; edge != 0; edge = edge->next) {
//       printf("%s ", edge->edgeName);
//     }
//
//     printf("\n");
//   }
//   printf("\n");
// }
//
// void printSingleList(struct Edge* visitedQueue) {
//   printf("PRINTING SINGLE LIST\n");
//   for (struct Edge* edge = visitedQueue; edge != 0; edge = edge->next) {
//     printf("%s ", edge->edgeName);
//   }
//   printf("\n");
// }

int indexOfVertex(struct Node* head, char vertexName[], int num) {
  for (int i = 0; i < num; i++) {
    if (strcmp(head[i].name, vertexName) == 0) {
      return i;
    }
  }

  return -1;
}

struct Node* addEdge(struct Node* head, char vertexName[], char eName[], int num) {
  int index = indexOfVertex(head, vertexName, num);
  if (index == -1) {
    return head;
  }

  struct Edge* temp = malloc(sizeof(struct Edge));
  strcpy(temp->edgeName, eName);
  temp->next = 0;
  temp->originalVertex = &(head[indexOfVertex(head, eName, num)]);

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
//      printf("REACHED THE ELSE STATEMENT");
    }
  }

  return head;
}

void enqueue(struct Edge* edge) {
  struct Edge* temp = malloc(sizeof(struct Edge));
  strcpy(temp->edgeName, edge->edgeName);
  temp->next = 0;
  temp->originalVertex = edge->originalVertex;


  if (queue == 0) { // if queue is empty, just return the edge as the queue
    queue = temp;
  //  free(temp);
    return;
  }

  struct Edge* ptr = queue;

  while (ptr->next != 0) { // go to the last element
    ptr = ptr->next;
  }


  ptr->next = temp;
//  free(temp);
//  free(ptr);
  return;
}

struct Edge* dequeue() {
  if (queue == 0) {
    return queue;
  }

  struct Edge* temp = queue;
  queue = queue->next;
  temp->next = 0;

  return temp;
}

void BFS(struct Node* head, char vertex[], int num) {

  int index = indexOfVertex(head, vertex, num);

  struct Edge* firstVertex = 0;
  firstVertex = allocate(head, vertex, index);

  enqueue(firstVertex);  // adding the first vertex
  free(firstVertex);
  while (queue != 0) { // change this to while queue is empty
  //  printf("DEQUEUEING %s\n", queue->edgeName);
    struct Edge* ptr = dequeue();  // dequeueing first vertex
    printf("%s ", ptr->edgeName);

    ptr->originalVertex->visited = 1;

    for (struct Edge* edge = ptr->originalVertex->next; edge != 0; edge = edge->next) {
//      printf("Pointing to %s\n", edge->edgeName);
      if (edge->originalVertex->visited == 0) {
//        printf("Adding %s\n", edge->edgeName);
//        printSingleList(queue);
        enqueue(edge);
        edge->originalVertex->visited = 1;
      }
    }

    free(ptr);
  }
  printf("\n");

  queue = 0;

  for (int i = 0; i < num; i++) {
    head[i].visited = 0;
  }
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

  while (fscanf(f, "%s %s\n", vertexName, edgeName) != EOF) {
    head = addEdge(head, vertexName, edgeName, num);
    head = addEdge(head, edgeName, vertexName, num);
  }

  char v[20];

  while (fscanf(f2, "%s\n", v) != EOF) {
    BFS(head, v, num);
  }

  fclose(f);
  fclose(f2);
  freeList(head, num);
  free(queue);
  return EXIT_SUCCESS;
}
