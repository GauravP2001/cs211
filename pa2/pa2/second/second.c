#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
  char name[30];
  struct Edge* next;
};

struct Edge {
  char edgeName[30];
//  struct Node* originalVertex;
  struct Edge* next;
	int weight;
};

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

void degreeOut(struct Node* head, char vertex[], int index) {
  if (index == -1) {
    return;
  }

  struct Edge* ptr = head[index].next;
  int counter = 0;

  if (ptr == 0) {
    printf("%d\n", counter);
    return;
  }

  while (ptr != 0) {
    counter++;
    ptr = ptr->next;
  }

  printf("%d\n", counter);
}

void degreeIn(struct Node* head, char vertex[], int num) {
	int counter = 0;

	for (int i = 0; i < num; i++) {
		if (strcmp(head[i].name, vertex) == 0) {
			continue;
		} else {
				for (struct Edge* temp = head[i].next; temp != 0; temp = temp->next) {
					if (strcmp(temp->edgeName, vertex) == 0) {
						counter++;
					}
				}
		}
	}

	printf("%d\n", counter);
}

void printSingleList(struct Node* head, char vertex[], int index) {
  for (struct Edge* edge = head[index].next; edge != 0; edge = edge->next) {
    printf("%s ", edge->edgeName);
  }
  printf("\n");
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
      printf("REACHED THE ELSE STATEMENT");
    }
  }

  return head;
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

  char command[20];
  char commandTo[20];

  while (fscanf(f2, "%s %s\n", command, commandTo) != EOF) {
    if (strcmp("o", command) == 0) {
      degreeOut(head, commandTo, indexOfVertex(head, commandTo, num));
    } else if (strcmp("a", command) == 0) {
      printSingleList(head, commandTo, indexOfVertex(head, commandTo, num));
    } else if (strcmp("i", command) == 0) {
			degreeIn(head, commandTo, num);
		}
  }

  fclose(f);
  fclose(f2);
  freeList(head, num);
  return EXIT_SUCCESS;
}
