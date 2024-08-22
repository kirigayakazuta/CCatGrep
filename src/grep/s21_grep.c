#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#define D_GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

typedef struct {
  char e, i, v, c, l, n, h, s, f, o;
} options;

typedef struct {
  options flags;
  node *head;
  node *now_node;
  char *file_name;
  char *file_str;
  int count_files;
  int count_str;
  int count_finded_str;
} arguments;

FILE *open_file(char *file_name, options flag, node *head) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    if (flag.s == 0)
      printf("s21_grep: %s: No such file or directory", file_name);
    free_ls(head);
    exit(1);
  }
  return file;
}

void print_finded_str_o(char *str, int str_start, int str_end) {
  for (int i = str_start; i < str_end && str[i] != '\n'; i++)
    putc(str[i], stdout);
  putc('\n', stdout);
}

void print_res_c_l(arguments arg) {
  if (arg.flags.l == 1 && arg.count_finded_str > 0)
    printf("%s\n", arg.file_name);
  else if (arg.flags.c == 1 && arg.count_files == 1)
    printf("%d\n", arg.count_finded_str);
  else if (arg.flags.c == 1 && arg.count_files >= 1)
    printf("%s:%d\n", arg.file_name, arg.count_finded_str);
}

void print_finded_str(arguments arg) {
  if (arg.flags.o == 0 || arg.flags.v == 0) {
    if (arg.flags.h == 0 && arg.count_files > 1) printf("%s:", arg.file_name);
    if (arg.flags.n == 1) printf("%d:", arg.count_str);
    if (arg.flags.o == 0) {
      printf("%s", arg.file_str);
      if (arg.file_str[strlen(arg.file_str) - 1] != '\n') putc('\n', stdout);
    }
  }
}

int find_str(arguments arg) {
  int res = 0;
  char *buffer = arg.file_str;
  arg.now_node = arg.head->pointer;

  int regex_flags = REG_EXTENDED;
  if (arg.flags.i == 1) regex_flags |= REG_ICASE;

  while (arg.now_node != NULL) {
    regex_t regex;
    if (regcomp(&regex, arg.now_node->string, regex_flags) != 0) {
      printf("REGCOMP ERROR");
      exit(1);
    }

    if ((arg.flags.v == 0 &&
         regexec(&regex, arg.file_str, 0, NULL, regex_flags) == 0) ||
        (arg.flags.v == 1 &&
         regexec(&regex, arg.file_str, 0, NULL, regex_flags) != 0)) {
      if (arg.flags.c == 0 && arg.flags.l == 0) print_finded_str(arg);
      res = 1;
    }

    if (arg.flags.o == 1 && arg.flags.v == 0 && arg.flags.c == 0 &&
        arg.flags.l == 0) {
      size_t nmatch = 1;
      regmatch_t pmatch[1];
      while (regexec(&regex, buffer, nmatch, pmatch, regex_flags) == 0) {
        print_finded_str_o(buffer, pmatch[0].rm_so, pmatch[0].rm_eo);
        buffer += pmatch[0].rm_eo;
      }
    }

    regfree(&regex);
    arg.now_node = arg.now_node->pointer;
  }
  return res;
}

void s21_grep(arguments arg) {
  arg.count_str = 1;
  arg.count_finded_str = 0;
  FILE *file = open_file(arg.file_name, arg.flags, arg.head);

  size_t bufsize = 0;
  int str_file_len = 0;
  arg.file_str = NULL;
  while ((str_file_len = getline(&arg.file_str, &bufsize, file)) != -1) {
    if (find_str(arg) == 1) arg.count_finded_str++;
    arg.count_str++;
  }
  free(arg.file_str);
  fclose(file);

  print_res_c_l(arg);
}

int main(int argc, char *argv[]) {
  options flag;
  flag.e = 0;
  flag.i = 0;
  flag.v = 0;
  flag.c = 0;
  flag.l = 0;
  flag.n = 0;
  flag.h = 0;
  flag.s = 0;
  flag.f = 0;
  flag.o = 0;

  arguments arg;
  arg.head = init();
  arg.now_node = arg.head;

  int rez = 0;
  while (rez = getopt_long(argc, argv, "e:f:ivclnhso", NULL, NULL), rez != -1) {
    switch (rez) {
      case 'e':
        flag.e = 1;
        arg.now_node = add_node(arg.now_node, optarg, strlen(optarg));
        break;
      case 'f':
        flag.f = 1;
        FILE *file = open_file(optarg, flag, arg.head);
        size_t bufsize = 0;
        int str_file_len = 0;
        char *str_file = NULL;
        while ((str_file_len = getline(&str_file, &bufsize, file)) != -1)
          arg.now_node = add_node(arg.now_node, str_file, str_file_len);
        free(str_file);
        fclose(file);
        break;
      case 'i':
        flag.i = 1;
        break;
      case 'v':
        flag.v = 1;
        break;
      case 'c':
        flag.c = 1;
        break;
      case 'l':
        flag.l = 1;
        break;
      case 'n':
        flag.n = 1;
        break;
      case 'h':
        flag.h = 1;
        break;
      case 's':
        flag.s = 1;
        break;
      case 'o':
        flag.o = 1;
        break;
      case '?':
        printf("ERROR\n");
        break;
    }
  }

  arg.flags = flag;
  if (optind < argc) {
    arg.count_files = 0;
    int shift_optind = 1;
    if (flag.e == 1 || flag.f == 1)
      shift_optind = 0;
    else
      arg.now_node = add_node(arg.now_node, argv[optind], strlen(argv[optind]));

    for (int i = optind + shift_optind; i < argc; i++) arg.count_files++;
    for (int i = optind + shift_optind; i < argc; i++) {
      arg.file_name = argv[i];
      s21_grep(arg);
    }
  }
  free_ls(arg.head);
  return (0);
}