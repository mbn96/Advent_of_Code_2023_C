#include "../defs.h"

/*
 * Structs for storing x and y coordinates and gear data, for a 64-bit machine,
 * I might change it to support 32-bit later. I am trying to keep it as simple
 * as possible. only allocate memory on the stack. We could change the hash_map
 * implementation later to support for sized-types since we already use
 * malloc for bucket nodes.
 * But for now I'm casting a 64-bit value as a void* so I don't need to allocate
 * memory on the heap for data structs.
 * TODO: support 32-bit
 */
typedef union {
  size_t val;
  struct {
    u32 x, y;
  };
} coord;

typedef union {
  size_t val;
  struct {
    size_t count : 8;
    size_t ratio : 56;
  };
} gear_data;

// comp func for coord
static bool coord_cmp_func(const void *a, const void *b) { return a == b; }

static bool isDigit(char c) { return c >= '0' && c <= '9'; }

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
  char  *firstLine = strchr(fc.data, '\n');
  width            = firstLine - fc.data;
  height           = (fc.len + 1 /* for last \n*/) / (width + 1);

  char *grid[height];
  for (size_t i = 0; i < height; i++) {
    grid[i]        = fc.data + i * (width + 1);
    grid[i][width] = '\0'; // the last line is null terminated so it's not out of bounds.
  }
  size_t    part_sum       = 0;
  size_t    gear_ratio_sum = 0;
  hash_map *hm             = hash_map_new(hash_int, coord_cmp_func);
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
        int   top, bottom;
        left        = row != grid[i] ? left - 1 : left;
        top         = i != 0 ? -1 : 0;
        bottom      = i != height - 1 ? 1 : 0;

        bool isPart = false;
        for (; top <= bottom; ++top) {
          char *start = left + top * (width + 1);
          char *end   = right + top * (width + 1);
          for (; start <= end; ++start) {
            if (!isDigit(*start) && *start != '.' && *start != '\0') {
              isPart = true;
              if (*start == '*') {
                gear_data gear_d;
                coord     c           = {.y = i + top, .x = start - grid[i + top]};
                bool      hasGear     = false;
                void     *gear_d_temp = hash_map_get(hm, (void *)c.val, &hasGear);
                if (!hasGear) {
                  gear_d.ratio = num;
                  gear_d.count = 1;
                } else {
                  gear_d = *(gear_data *)&gear_d_temp;
                  gear_d.count++;
                }
                if (gear_d.count == 2) {
                  gear_d.ratio *= num;
                  gear_ratio_sum += gear_d.ratio;
                } else if (gear_d.count == 3) {
                  gear_ratio_sum -= gear_d.ratio;
                }
                hash_map_put(hm, (void *)c.val, (void *)gear_d.val);
              }
            }
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
  printf("Part sum: %zu\n", part_sum);
  printf("Gear ratio sum: %zu\n", gear_ratio_sum);

  hash_map_free(hm); // Not nessessary because we're exiting but good practice
  free(fc.data);
  return EXIT_SUCCESS;
}
