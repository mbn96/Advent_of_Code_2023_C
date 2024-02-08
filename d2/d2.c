#include "../defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  LINER(games, fc);
  str_view game;
  size_t gameID = 1, sum = 0;
  while (next_token(&games, &game)) {
    char *new_pos = strchr(game.data, ':') + 1;
    game.len -= new_pos - game.data;
    game.data = new_pos;

    bool possible = true;

    str_view pick;
    SPLITTER(picks, game, ';');
    while (next_token(&picks, &pick)) {
      str_view color;
      SPLITTER(colors, pick, ',');
      while (next_token(&colors, &color)) {
        ++color.data;
        --color.len;
        char *c_name = strchr(color.data, ' ');
        *c_name = '\0';
        ++c_name;
        int count = atoi(color.data);
        switch (c_name[0]) {
        case 'r':
          possible = count <= 12;
          break;
        case 'g':
          possible = count <= 13;
          break;
        case 'b':
          possible = count <= 14;
          break;
        }
        if (!possible) {
          goto game_end;
        }
        printf("%.*s\n", (int)color.len, color.data);
      }
      printf("--Pick End--\n");
    }
  game_end:
    printf("----Game End----\n");
    if (possible) {
      sum += gameID;
    }
    ++gameID;
  }

  printf("GameID sum: %zu\n", sum);

  free(fc.data);
  return EXIT_SUCCESS;
}
