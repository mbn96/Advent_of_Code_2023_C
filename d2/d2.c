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

  Spliter  games = LINER(fc);
  str_view game;
  size_t   gameID = 1, id_sum = 0, color_sum = 0;
  while (next_token(&games, &game)) {
    char *new_pos = strchr(game.data, ':') + 1;
    game.len -= new_pos - game.data;
    game.data           = new_pos;
    bool   possible     = true;
    size_t max_color[3] = {0, 0, 0};

    str_view pick;
    Spliter  picks = SPLITTER(game, ';');
    while (next_token(&picks, &pick)) {
      str_view color;
      Spliter  colors = SPLITTER(pick, ',');
      while (next_token(&colors, &color)) {
        ++color.data;
        --color.len;
        char *c_name = strchr(color.data, ' ');
        *c_name      = '\0';
        ++c_name;
        size_t count = atoi(color.data);
        switch (c_name[0]) {
        case 'r':
          possible     = possible && count <= 12;
          max_color[0] = count > max_color[0] ? count : max_color[0];
          break;
        case 'g':
          possible     = possible && count <= 13;
          max_color[1] = count > max_color[1] ? count : max_color[1];
          break;
        case 'b':
          possible     = possible && count <= 14;
          max_color[2] = count > max_color[2] ? count : max_color[2];
          break;
        default:
          fprintf(stderr, "Invalid color name: %s\n", c_name);
          return EXIT_FAILURE;
        }
        printf("%.*s\n", (int)color.len, color.data);
      }
      printf("--Pick End--\n");
    }
    printf("----Game End----\n");
    if (possible) {
      id_sum += gameID;
    }
    color_sum += (max_color[0] * max_color[1] * max_color[2]);
    ++gameID;
  }

  printf("GameID sum: %zu\n", id_sum);
  printf("Color sum: %zu\n", color_sum);

  free(fc.data);
  return EXIT_SUCCESS;
}
