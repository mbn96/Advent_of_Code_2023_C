#include "defs.h"

bool next_token(Spliter *liner, str_view *line) {
  if (liner->pos >= (liner->fc.data + liner->fc.len)) {
    return false;
  }
  char *start = liner->pos;
  char *end   = strchr(start, liner->delim);
  if (end == NULL || (end >= (liner->fc.data + liner->fc.len))) {
    line->data = start;
    line->len  = liner->fc.len - (liner->pos - liner->fc.data);
    liner->pos = liner->fc.data + liner->fc.len;
  } else {
    line->data = start;
    line->len  = end - start;
    liner->pos = end + 1;
  }

  return true;
}

int str_find_char(const str_view *sv, predicate pred) {
  for (size_t i = 0; i < sv->len; i++) {
    if (pred(sv->data[i])) {
      return i;
    }
  }
  return -1;
}

int str_find_last_char(const str_view *sv, predicate pred) {
  for (int i = sv->len - 1; i >= 0; i--) {
    if (pred(sv->data[i])) {
      return i;
    }
  }
  return -1;
}
