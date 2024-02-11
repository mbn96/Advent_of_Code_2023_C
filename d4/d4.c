#include "../defs.h"

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

  LINER(lines, fc);
  str_view line;
  size_t   total_score = 0;
  while (next_token(&lines, &line)) {
    SPLITTER(parts, line, '|');
    str_view wining_part, available_num_part;
    next_token(&parts, &wining_part);
    next_token(&parts, &available_num_part);

    // finding wining_part length
    char  *wining_part_data = strchr(wining_part.data, ':') + 2 /* skip ": " */;
    size_t win_p_len =
        (wining_part.len - (wining_part_data - wining_part.data)) / 3 /* 3 ch/num(one of which is the space) */;
    u32 wining_numbers[win_p_len];
    // filling wining_numbers
    for (size_t i = 0; i < win_p_len; ++i) {
      char *num         = wining_part_data + (i * 3);
      wining_numbers[i] = strtoul(num, NULL, 10);
    }

    // finding available numbers
    available_num_part.data++;
    available_num_part.len--;
    char *av_num = available_num_part.data;
    u32   score  = 1;
    while (av_num < available_num_part.data + available_num_part.len) {
      u32 num = strtoul(av_num, NULL, 10);
      av_num += 3;
      // since the wining numbers count are not very large, I think linear search is more efficient than hash-map
      // checking if the number is wining
      for (size_t i = 0; i < win_p_len; ++i) {
        if (wining_numbers[i] == num) {
          score <<= 1;
        }
      }
    }
    score >>= 1;
    total_score += score;
  }

  printf("Total score: %zu\n", total_score);

  free(fc.data);
}
