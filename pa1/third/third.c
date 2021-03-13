#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  int data;
  struct node* next;
};

struct node* allocate(int val) {
  struct node* temp = malloc(sizeof(struct node));
  temp->data = val;
  temp->next = 0;

  return temp;
}

// 1 2 3 4 5
struct node* enqueue(struct node* head, int val) {
  struct node* temp = allocate(val);

  if (head == 0) { // if the list is empty
    return temp;
  }

  struct node* ptr = head;

  while (ptr->next != 0) { // go to the last element
    ptr = ptr->next;
  }

  ptr->next = temp;

  return head;
}

struct node* pop(struct node* head) {
  if (head == 0) {
    return head;
  }

  struct node* temp = head;
  head = head->next;
  free(temp);

  return head;
}
// 2 3 4

struct node* push(struct node* head, int val) {
  struct node* temp = allocate(val);

  if (head == 0) { // if the list is empty
    return temp;
  }

  temp->next = head;
  return temp;
}

void freeLinkedList(struct node* head) {
  if (head == 0) {
    return;
  }

  struct node* temp, *temp2;

  temp = head;

  while (temp != 0) {
    temp2 = temp;
    temp = temp->next;
    free(temp2);
  }
}

void printList(struct node* head) {
  if (head == 0) {
    printf("EMPTY\n");
  } else {
    struct node* ptr = head;

    while (ptr != 0) {
      printf("%d ", ptr->data);
      ptr = ptr->next;
    }
     printf("\n");
  }
}


int main(int argc, char* argv[argc+1]) {

  struct node* head = 0;

  int val;
  char command[10];

  FILE* f = fopen(argv[1], "r");

  if (f == 0) {
    printf("error");
    return EXIT_SUCCESS;
  }


  while (fscanf(f, "%s %d", command, &val) != EOF) {
    if (strcmp(command, "ENQUEUE") == 0) {
      head = enqueue(head, val);
      printList(head);
    } else if (strcmp(command, "POP") == 0) {
      head = pop(head);
      printList(head);
    } else if (strcmp(command, "PUSH") == 0) {
      head = push(head, val);
      printList(head);
    }
  }

  fclose(f);
  freeLinkedList(head);
  return EXIT_SUCCESS;
}
