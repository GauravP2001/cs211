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

int ifExists(struct node* head, int val) {
  struct node* ptr = head;

  while (ptr != 0) {
    if (ptr->data == val) {
      return 0;
    }

    ptr = ptr->next;
  }

  return 1;
}

// 1 3 7
struct node* insert(struct node* head, int val) {
  if (ifExists(head, val) == 0) {
    return head;
  }

  struct node* temp = allocate(val);

  if (head == 0) {
    return temp;
  }

  struct node* ptr = head;

  if (ptr->next == 0) { //
    if (val > ptr->data) {
      ptr->next = temp;
    } else {
      temp->next = head;
      head = temp;
    }
    return head;
  }

  struct node* ptr2 = head->next;

  do {
    if (ptr-> data == val || ptr2->data == val) { // if the value already exists
      return head;
    } else if (ptr->data < val && ptr2->data > val) {  // in the middle
      temp->next = ptr2;
      ptr->next = temp;

      ptr = ptr->next;
      ptr2 = ptr2->next;
      return head;
    } else if (ptr->data > val) { // less than first value
      temp->next = head;
      head = temp;
      return head;
    } else if (ptr->data < val && ptr2->data < val && ptr2->next == 0) { // last
      ptr2->next = temp;
      return head;
    } else {
      ptr = ptr->next;
      ptr2 = ptr2->next;
    }

  } while (ptr2 != 0);

  return head;
}

// 2 3 5
struct node* delete(struct node* head, int val) {
  if (head == 0) {
   // printf("EMPTY\n");
    return head;
  }

  if (ifExists(head, val) == 1) {
    return head;
  }

  if (val == head->data) { // if the first int is the value we are deleting - check for null error
    struct node* a = head;
    head = head->next;
    free(a);
    return head;
  }

  struct node* ptr = head;
  struct node* ptr2 = head->next;

// 1 2 3 4 5

while (ptr != 0) {
  if (val == ptr2->data) {  // if ptr2 pointer is the value
    if (ptr2->next == 0) {  // if its the last element
      ptr->next = 0;
      free(ptr2);
      return head;
    } else {
      struct node* t = ptr2;
      ptr->next = ptr2->next;
      free(t);
    }

    return head;
  } else {
    ptr = ptr->next;
    ptr2 = ptr2->next;
  }

  if (ptr2->next == 0) { // if it is the last element and nothing has been found
    if (val == ptr2->data) {
      ptr->next = 0;
      free(ptr2);
      return head;
    }

    return head;
  }
}

  return head;
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

int main(int argc, char* argv[argc+1]) {
  struct node* head = 0;

  int val;
  char command[10];

  FILE* f = fopen(argv[1], "r");

  if (f == 0) {
    printf("error\n");
    return EXIT_SUCCESS;
  }

  while (fscanf(f, "%s %d\n", command, &val) != EOF) {
    if (strcmp(command, "INSERT") == 0) {
      head = insert(head, val);
      printList(head);
    } else {
      head = delete(head, val);
      printList(head);
    }
  }

fclose(f);
freeLinkedList(head);
return EXIT_SUCCESS;
}
