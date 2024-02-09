#include "../defs.h"

static bool isDigit(char c) { return c >= '0' && c <= '9'; }

struct letterDigits {
  char d1, d2;
  int d1_idx, d2_idx;
};
typedef struct letterDigits ld_t;

typedef struct {
  u16 first3LettersSum;
  u8 len;
  char *spell;
} digit_spell_t;

static ld_t find_letter_digits(const str_view *s) {

  static const digit_spell_t spells[10] = {
      {0, 0, "zero"},
      {'o' + 'n' + 'e', 3, "one"},
      {'t' + 'w' + 'o', 3, "two"},
      {'t' + 'h' + 'r', 5, "three"},
      {'f' + 'o' + 'u', 4, "four"},
      {'f' + 'i' + 'v', 4, "five"},
      {'s' + 'i' + 'x', 3, "six"},
      {'s' + 'e' + 'v', 5, "seven"},
      {'e' + 'i' + 'g', 5, "eight"},
      {'n' + 'i' + 'n', 4, "nine"},
  };

  if (s->len < 3) {
    return (ld_t){-1, -1, -1, -1};
  }

  ld_t ld;
  ld.d1_idx = ld.d2_idx = -1;
  u16 sum = s->data[0] + s->data[1];
  for (size_t c = 0; c < s->len - 2; ++c) {
    sum += s->data[c + 2];
    for (int i = 1; i < 10; i++) {
      if (sum == spells[i].first3LettersSum &&
          ((s->len - c) >= spells[i].len) &&
          strncmp(s->data + c, spells[i].spell, spells[i].len) == 0) {
        if (ld.d1_idx == -1) {
          ld.d1 = i + '0';
          ld.d1_idx = c;
        }
        ld.d2 = i + '0';
        ld.d2_idx = c;
        break;
      }
    }
    sum -= s->data[c];
  }
  return ld;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  file_content fc;
  if (!read_file(argv[1], &fc)) {
    fprintf(stderr, "Could not read file!\n");
    return EXIT_FAILURE;
  }

  LINER(lines, fc);
  str_view line;
  size_t sum = 0;
  while (next_token(&lines, &line)) {
    if (line.len != 0) {
      int firstDigit = str_find_char(&line, isDigit);
      int lastDigit = str_find_last_char(&line, isDigit);
      ld_t lds = find_letter_digits(&line);
      if (firstDigit == -1 && lds.d1_idx == -1) {
        fprintf(stderr, "Invalid line: %.*s\n", (int)line.len, line.data);
        return EXIT_FAILURE;
      }
      // find actual first and last digit
      char num[3] = {0, 0, 0};
      lds.d1_idx = lds.d1_idx == -1 ? firstDigit : lds.d1_idx;
      if (firstDigit == -1 || firstDigit > lds.d1_idx) {
        num[0] = lds.d1;
      } else {
        num[0] = line.data[firstDigit];
      }
      if (lastDigit == -1 || lastDigit < lds.d2_idx) {
        num[1] = lds.d2;
      } else {
        num[1] = line.data[lastDigit];
      }
      int lineNum = atoi(num);
      printf("%.*s: %d\n", (int)line.len, line.data /*has no null terminator*/,
             lineNum);
      sum += lineNum;
    }
  }
  printf("Sum: %zu\n", sum);

  free(fc.data);
  return EXIT_SUCCESS;
}
