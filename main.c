#include <stdio.h>

int main(int argc, char **argv) {
  if (argc > 1) {
    fprintf(stderr, "This program takes no arguments.\n");
    return 1;
  }

  printf("Hello, world!\n");
  return 0;
}
