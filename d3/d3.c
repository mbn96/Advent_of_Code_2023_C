#include "../defs.h"
#include <string.h>

static bool isDigit(char c) { return c >= '0' && c <= '9'; }
static bool isNotDigit(char c) { return c < '0' || c > '9'; }

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  file_content fc;
  if (!read_file(argv[1], &fc)) {
    fprintf(stderr, "Could not read file\n");
    return EXIT_FAILURE;
  }

  // finding width and height
  size_t width = 0, height = 0;
  char *firstLine = strchr(fc.data, '\n');
  width = firstLine - fc.data;
  height = (fc.len + 1 /* for last \n*/) / (width + 1);

  char *grid[height];
  for (size_t i = 0; i < height; i++) {
    grid[i] = fc.data + i * (width + 1);
    grid[i][width] =
        '\0'; // the last line is null terminated so it's not out of bounds.
  }
  size_t part_sum = 0;
  for (size_t i = 0; i < height; i++) {
    char *row = grid[i];
    while (*row != '\0') {
      if (isDigit(*row)) {
        size_t j = 0;
        while (isDigit(*(row + j))) {
          j++;
        }
        size_t num = strtoul(row, NULL, 10);

        // num bounding box
        char *left = row, *right = row + j;
        int top, bottom;
        left = row != grid[i] ? left - 1 : left;
        top = i != 0 ? -1 : 0;
        bottom = i != height - 1 ? 1 : 0;

        bool isPart = false;
        for (; top <= bottom; ++top) {
          char *start = left + top * (width + 1);
          char *end = right + top * (width + 1);
          for (; start <= end; ++start) {
            if (isNotDigit(*start) && *start != '.' && *start != '\0') {
              isPart = true;
              break;
            }
          }
          if (isPart) {
            break;
          }
        }

        if (isPart) {
          part_sum += num;
        }
        row += j;
      } else {
        row++;
      }
    }
  }
  printf("%zu\n", part_sum);

  free(fc.data);
  return EXIT_SUCCESS;
}
