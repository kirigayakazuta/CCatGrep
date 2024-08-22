#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b, e, n, s, t, v, E, T;
} options;

void print_with_E(FILE *file) {
  char symbol;
  while ((symbol = fgetc(file)) != EOF) {
    if (symbol == '\n')
      printf("$\n");
    else
      printf("%c", symbol);
  }
}

void print_with_T(FILE *file) {
  char symbol;
  while ((symbol = fgetc(file)) != EOF) {
    if (symbol == '\t')
      printf("^I");
    else
      printf("%c", symbol);
  }
}

void print_with_e(FILE *file) {
  int symbol;
  while ((symbol = fgetc(file)) != EOF) {
    if (symbol == '\n')
      printf("$\n");
    else if (0 <= symbol && symbol <= 31 && symbol != '\t')
      printf("^%c", symbol + 64);
    else if (symbol == 127)
      printf("^?");
    else if (128 <= symbol && symbol <= 159)
      printf("M-^%c", symbol - 64);
    else
      printf("%c", symbol);
  }
}

void print_with_tt(FILE *file) {
  int symbol;
  while ((symbol = fgetc(file)) != EOF) {
    if (symbol == '\t')
      printf("^I");
    else if (0 <= symbol && symbol <= 31 && symbol != '\n')
      printf("^%c", symbol + 64);
    else if (symbol == 127)
      printf("^?");
    else if (128 <= symbol && symbol <= 159)
      printf("M-^%c", symbol - 64);
    else
      printf("%c", symbol);
  }
}

void print_with_v(FILE *file) {
  int symbol;
  while ((symbol = fgetc(file)) != EOF) {
    if (0 <= symbol && symbol <= 31 && symbol != '\n' && symbol != '\t')
      printf("^%c", symbol + 64);
    else if (symbol == 127)
      printf("^?");
    else if (128 <= symbol && symbol <= 159)
      printf("M-^%c", symbol - 64);
    else
      printf("%c", symbol);
  }
}

void print_with_n(FILE *file) {
  int counter = 1;
  char symbol;
  char pre = '\0';
  if (fgetc(file) != EOF) {
    printf("%6d\t", counter);
    fseek(file, 0, SEEK_SET);
  }
  while ((symbol = fgetc(file)) != EOF) {
    if (pre == '\n') {
      counter++;
      printf("%6d\t", counter);
    }
    pre = symbol;
    printf("%c", pre);
  }
}

void print_with_b(FILE *file) {
  int counter = 1;
  char symbol;
  char pre = '\n';
  while ((symbol = fgetc(file)) != EOF) {
    if (symbol != '\n' && pre == '\n') {
      printf("%6d\t", counter);
      counter++;
    }
    printf("%c", symbol);
    pre = symbol;
  }
}

void print_with_s(FILE *file) {
  char symbol;
  char pre1 = '\n';
  char pre2;
  while ((symbol = fgetc(file)) != EOF) {
    if (!(symbol == '\n' && pre1 == '\n' && pre2 == '\n')) printf("%c", symbol);
    pre2 = pre1;
    pre1 = symbol;
  }
}

void print_no_flags(FILE *file) {
  char symbol;
  while ((symbol = fgetc(file)) != EOF) printf("%c", symbol);
}

void open_file(char *file_name, options flags) {
  FILE *file;
  file = fopen(file_name, "r");
  if (file == NULL) {
    printf("cat: %s: No such file or directory", file_name);
    exit(1);
  }
  if (flags.b + flags.e + flags.b + flags.n + flags.s + flags.t + flags.E +
          flags.T + flags.v ==
      0)
    print_no_flags(file);
  else if (flags.n == 1)
    print_with_n(file);
  else if (flags.b == 1)
    print_with_b(file);
  else if (flags.s == 1)
    print_with_s(file);
  else if (flags.e == 1)
    print_with_e(file);
  else if (flags.v == 1)
    print_with_v(file);
  else if (flags.t == 1)
    print_with_tt(file);
  else if (flags.E == 1)
    print_with_E(file);
  else if (flags.T == 1)
    print_with_T(file);
  else
    printf("no flags");
  fclose(file);
}

int main(int argc, char **argv) {
  options flags;
  flags.b = 0;
  flags.E = 0;
  flags.e = 0;
  flags.n = 0;
  flags.s = 0;
  flags.T = 0;
  flags.t = 0;
  flags.v = 0;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--number-nonblank") == 0)
      flags.b = 1;
    else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--number") == 0)
      flags.n = 1;
    else if (strcmp(argv[i], "-s") == 0 ||
             strcmp(argv[i], "--squeeze-blank") == 0)
      flags.s = 1;
    else if (strcmp(argv[i], "-e") == 0)
      flags.e = 1;
    else if (strcmp(argv[i], "-E") == 0)
      flags.E = 1;
    else if (strcmp(argv[i], "-t") == 0)
      flags.t = 1;
    else if (strcmp(argv[i], "-T") == 0)
      flags.T = 1;
    else if (strcmp(argv[i], "-v") == 0)
      flags.v = 1;
  }

  for (int i = 1; i < argc; i++)
    if (argv[i][0] != '-') open_file(argv[i], flags);

  return 0;
}