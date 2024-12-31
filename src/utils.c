#include "headers/utils.h"
#include "headers/config.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char *get_cache_file_path() {
  const char *home = getenv("HOME");
  if (!home) {
    fprintf(stderr, "HOME environment variable not set.\n");
    exit(1);
  }

  // Allocate memory for the full path
  size_t path_len =
      strlen(home) + strlen(CACHE_DIR) + strlen(JSON_FILENAME) + 1;
  char *path = malloc(path_len);

  snprintf(path, path_len, "%s%s%s", home, CACHE_DIR, JSON_FILENAME);
  return path;
}

void ensure_cache_dir() {
  const char *home = getenv("HOME");
  if (!home) {
    fprintf(stderr, "HOME environment variable not set.\n");
    exit(1);
  }

  // Construct the directory path
  size_t dir_len = strlen(home) + strlen(CACHE_DIR) + 1;
  char *dir = malloc(dir_len);

  snprintf(dir, dir_len, "%s%s", home, CACHE_DIR);

  // Create the directory if it doesn't exist
  if (mkdir(dir, 0755) && errno != EEXIST) {
    fprintf(stderr, "Failed to create directory: %s\n", dir);
    free(dir);
    exit(1);
  }
  free(dir);
}
