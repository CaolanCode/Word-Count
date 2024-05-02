#include <ctype.h>
#include <stdio.h>
#include <string.h>

long int byte_count(FILE *file) {
  fseek(file, 0L, SEEK_END);
  long int byte_count = ftell(file);
  fseek(file, 0L, SEEK_SET);
  return byte_count;
}

void count(FILE *file, long int *data) {
  char ch;
  long int char_count = 0;
  long int word_count = 0;
  int in_word = 0;
  long int line_count = 0;
  while ((ch = fgetc(file)) != EOF) {
    if (ch > -64) {
      char_count++;
    }
    if (isspace(ch) || ch == '\0') {
      if (in_word) {
        word_count++;
      }
      in_word = 0;
    } else {
      in_word = 1;
    }
    if (ch == '\n') {
      line_count++;
    }
  }
  if (in_word) {
    word_count++;
  }
  *data++ = line_count;
  *data++ = word_count;
  *data = char_count;
}

void print_options(long int *data, int argc, char *argv[]) {
  printf("    ");
  if (strcmp(argv[1], "-l") == 0) {
    printf("%ld  ", data[0]);
  } else if (strcmp(argv[1], "-w") == 0) {
    printf("%ld  ", data[1]);
  } else if (strcmp(argv[1], "-m") == 0) {
    printf("%ld  ", data[2]);
  }
}

void print_results(long int *data, int argc, char *argv[], long int byte_size) {
  if (argc == 2) {
    printf("    %ld   %ld  %ld %s\n", data[0], data[1], byte_size, argv[1]);
    return;
  }
  if (strcmp(argv[1], "-c") == 0) {
    printf("    %ld  ", byte_size);
  } else {
    print_options(data, argc, argv);
  }
  printf("%s\n", argv[2]);
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("    usage: %s <filename>\n", argv[0]);
    return 1;
  }
  char *filename = argv[argc - 1];
  FILE *file = fopen(filename, "r");
  long int data[] = {0, 0, 0};
  if (file == 0) {
    file = stdin;
    count(file, data);
    print_options(data, argc, argv);
    printf("\n");
  } else {
    long int byte_size = byte_count(file);
    count(file, data);
    print_results(data, argc, argv, byte_size);
    fclose(file);
  }
  return 0;
}
