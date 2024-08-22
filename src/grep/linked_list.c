#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>

node *init() {
  node *HEAD = malloc(sizeof(node));
  HEAD->pointer = NULL;
  HEAD->string = NULL;
  return HEAD;
}

node *add_node(node *NODE, char *str, int len_str) {
  char *buff = malloc(sizeof(char) * (len_str + 1));
  for (int i = 0; i < len_str; i++) buff[i] = str[i];
  buff[len_str] = '\0';

  node *NEXT = malloc(sizeof(node));
  NEXT->pointer = NULL;
  NEXT->string = buff;
  NODE->pointer = NEXT;
  return NEXT;
}

void free_ls(node *HEAD) {
  if (HEAD != NULL) {
    node *NODE = NULL;
    NODE = HEAD->pointer;
    free(HEAD);
    node *NEXT = NULL;
    while (NODE != NULL) {
      NEXT = NODE->pointer;
      if (NODE->string != NULL) free(NODE->string);
      free(NODE);
      NODE = NEXT;
    }
  }
}