#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
  char name[30];
  struct Edge* next;
  int visited;
};

struct Edge {
  char edgeName[30];
  struct Node* originalVertex;
  struct Edge* next;
  int weight;
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

void DFSMain(struct Node* head, int num, int index) {
  head[index].visited = 1;
  printf("%s ", head[index].name);

  for(struct Edge* ptr = head[index].next; ptr != 0; ptr = ptr->next) {
    if (ptr->originalVertex->visited == 0) {
      DFSMain(head, num, indexOfVertex(head, ptr->originalVertex->name, num));
    }
  }
}


void DFS(struct Node* head, int num) {
  for (int i = 0; i < num; i++) {
    head[i].visited = 0;
  }

  for (int i = 0; i < num; i++) {
    if (head[i].visited == 0) {
      DFSMain(head, num, i);
    }
  }
}


int main(int argc, char* argv[argc+1]) {
  int num;
  char vertex[30];

  FILE* f = fopen(argv[1], "r");

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
	int weight;

  while (fscanf(f, "%s %s %d\n", vertexName, edgeName, &weight) != EOF) {
    head = addEdge(head, vertexName, edgeName, weight, num);
  }

  DFS(head, num);
  printf("\n");

  fclose(f);
  freeList(head, num);
  return EXIT_SUCCESS;
}
