#include <ctype.h>
#include <stdio.h>
#include <string.h>

long int byte_count(FILE *file) {
  fseek(file, 0L, SEEK_END);
  long int byte_count = ftell(file);
  fseek(file, 0L, SEEK_SET);
  return byte_count;
}

int count(FILE *file, long int *data) {
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
  return 0;
}

int print_results(long int *data, int argc, char *argv[], long int byte_size) {
  if (argc == 2) {
    printf("    %ld   %ld  %ld %s\n", data[0], data[1], byte_size, argv[1]);
    return 0;
  }
  for (int i = 1; i < argc; i++) {
    if (i == 1) {
      printf("    ");
    }
    if (strcmp(argv[i], "-l") == 0) {
      printf("%ld  ", data[0]);
    }
    if (strcmp(argv[i], "-w") == 0) {
      printf("%ld  ", data[1]);
    }
    if (strcmp(argv[i], "-c") == 0) {
      printf("%ld  ", byte_size);
    }
    if (strcmp(argv[i], "-m") == 0) {
      printf("%ld  ", data[2]);
    }
    if (i == argc - 1) {
      printf("%s\n", argv[1]);
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("    usage: %s <filename>\n", argv[0]);
    return 1;
  }
  char *filename = argv[argc - 1];
  FILE *file = fopen(filename, "r");
  if (file == 0) {
    printf("    Could not open file\n");
    return 1;
  }
  long int data[] = {0, 0, 0};
  long int byte_size = byte_count(file);
  count(file, data);
  fclose(file);
  print_results(data, argc, argv, byte_size);
  return 0;
}
