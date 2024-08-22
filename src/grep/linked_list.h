#ifndef LINKED_LIST
#define LINKED_LIST

struct node {
  struct node *pointer;
  char *string;
};

typedef struct node node;

node *init();
node *add_node(node *NODE, char *str, int len_str);
void free_ls(node *HEAD);

#endif
