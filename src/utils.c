#include "headers/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int get_choice() {
  int choice;
  while (scanf("%d", &choice) != 1) {
    printf("Invalid input. Please enter a number: ");
    while (getchar() != '\n')
      ; // Clear input buffer
  }
  getchar(); // Remove newline character left in buffer after input
  return choice;
}

void init_json_file(const char *filename) {
  struct stat buffer;
  if (stat(filename, &buffer) != 0) { // File does not exist
    FILE *file = fopen(filename, "w");
    if (file) {
      fprintf(file, "[]"); // Create an empty JSON array
      fclose(file);
      printf("\033[1;32mInitialized empty transaction file at %s\033[0m\n",
             filename);
    } else {
      printf("\033[1;31mError: Could not create file %s\033[0m\n", filename);
      exit(EXIT_FAILURE);
    }
  }
}
