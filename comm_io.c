#include "defs.h"

bool read_file(const char *path, file_content *fc) {
  bool res = true;
  FILE *file = fopen(path, "r");
  if (!file) {
    perror("File read Err:");
    return false;
  }
  char buff[1024];
  char *content = NULL;
  size_t n = 0, curr_len = 0;
  while ((n = fread(buff, 1, sizeof(buff) - 1, file))) {
    content = realloc(content, curr_len + n + 1);
    if (!content) {
      fprintf(stderr, "Could not allocate memory");
      res = false;
      break;
    }
    buff[n] = '\0';
    content[curr_len] = '\0';
    strcat(content, buff);
    curr_len += n;
  }
  fc->data = content;
  fc->len = curr_len;

  fclose(file);
  return res;
}
