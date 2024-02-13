#include "../defs.h"
#include <string.h>

#define COPIES_COUNT_LEN 16

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

  Spliter  lines = LINER(fc);
  str_view line;
  size_t   total_score = 0, total_scatchCards = 0;
  u32      card_copies[COPIES_COUNT_LEN];
  size_t   copies_idx = 0;
  for (size_t i = 0; i < COPIES_COUNT_LEN; ++i) {
    card_copies[i] = 1;
  }

  while (next_token(&lines, &line)) {
    Spliter  parts = SPLITTER(line, '|');
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
    char  *av_num       = available_num_part.data;
    u32    score        = 1;
    size_t wining_count = 0;
    while (av_num < available_num_part.data + available_num_part.len) {
      u32 num = strtoul(av_num, NULL, 10);
      av_num += 3;
      // since the wining numbers count are not very large, I guess linear search is fine.
      // checking if the number is wining
      for (size_t i = 0; i < win_p_len; ++i) {
        if (wining_numbers[i] == num) {
          score <<= 1;
          wining_count++;
          card_copies[(copies_idx + wining_count) & (COPIES_COUNT_LEN - 1)] += card_copies[copies_idx];
        }
      }
    }
    score                   = score >> 1;
    total_score             = total_score + score;
    total_scatchCards       = total_scatchCards + card_copies[copies_idx];
    card_copies[copies_idx] = 1;
    copies_idx              = (copies_idx + 1) & (COPIES_COUNT_LEN - 1);
  }

  printf("Total score: %zu\n", total_score);
  printf("Total scatchCards: %zu\n", total_scatchCards);

  free(fc.data);
}
