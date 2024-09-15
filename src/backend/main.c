#include "tetris.h"

int random_figure() {
  int upper = 6;
  int random_figure = rand() % (upper + 1);
  return random_figure;
}

void fill_figures(GameInfo_t *info) {
  for (int i = 0; i < PIECE_SIZE; ++i) {
    for (int j = 0; j < PIECE_SIZE; ++j) {
      info->current.piece[i][j] = shapes[info->current.name][i][j];
      info->next.piece[i][j] = shapes[info->next.name][i][j];
    }
  }
}

void new_spawn(GameInfo_t *info) {
  info->current.name = info->next.name;
  info->next.name = random_figure(info);
  fill_figures(info);
  info->current.start_x = SPAWN_X;
  info->current.start_y = SPAWN_Y;
  for (int h = 0; h < PIECE_SIZE; h++)
    for (int w = 0; w < PIECE_SIZE; w++) {
      if (info->current.piece[h][w] == CURRENT_FIGURE)
        info->field[h + SPAWN_Y][w + SPAWN_X] = CURRENT_FIGURE;
    }
}

int left_check(GameInfo_t info) {
  int check = ALLOW;
  for (int h = UP_BOARD; h <= DOWN_BOARD; ++h)
    for (int w = LEFT_BOARD; w <= RIGHT_BOARD; ++w) {
      if (info.field[h][w] == CURRENT_FIGURE) {
        if (w == LEFT_BOARD || info.field[h][w - 1] != EMPTY_PLACE) {
          check = FORBID;
        }
        ++h;
        w = LEFT_BOARD;
      }
    }
  return check;
}

void move_left(GameInfo_t *info) {
  if (left_check(*info) == ALLOW) {
    for (int h = UP_BOARD; h <= DOWN_BOARD; ++h)
      for (int w = LEFT_BOARD; w <= RIGHT_BOARD; ++w) {
        if (info->field[h][w] == CURRENT_FIGURE) {
          info->field[h][w] = EMPTY_PLACE;
          info->field[h][w - 1] = CURRENT_FIGURE;
        }
      }
    info->current.start_x -= 1;
  }
}

int right_check(GameInfo_t info) {
  int check = ALLOW;
  for (int h = UP_BOARD; h <= DOWN_BOARD; ++h)
    for (int w = RIGHT_BOARD; w >= LEFT_BOARD; --w) {
      if (info.field[h][w] == CURRENT_FIGURE) {
        if (w == RIGHT_BOARD || info.field[h][w + 1] != EMPTY_PLACE) {
          check = FORBID;
        }
        ++h;
        w = RIGHT_BOARD;
      }
    }
  return check;
}

void move_right(GameInfo_t *info) {
  if (right_check(*info) == ALLOW) {
    for (int h = UP_BOARD; h <= DOWN_BOARD; ++h)
      for (int w = RIGHT_BOARD; w >= LEFT_BOARD; --w) {
        if (info->field[h][w] == CURRENT_FIGURE) {
          info->field[h][w] = EMPTY_PLACE;
          info->field[h][w + 1] = CURRENT_FIGURE;
        }
      }
    info->current.start_x += 1;
  }
}

int down_check(GameInfo_t info) {
  int check = ALLOW;
  for (int w = LEFT_BOARD; w <= RIGHT_BOARD; ++w)
    for (int h = DOWN_BOARD; h >= UP_BOARD; --h) {
      if (info.field[h][w] == CURRENT_FIGURE) {
        if (h == DOWN_BOARD || info.field[h + 1][w] != EMPTY_PLACE) {
          check = FORBID;
        }
        ++w;
        h = DOWN_BOARD;
      }
    }
  return check;
}

void crash(GameInfo_t *info) {
  for (int h = UP_BOARD; h <= DOWN_BOARD; ++h)
    for (int w = RIGHT_BOARD; w >= LEFT_BOARD; --w) {
      if (info->field[h][w] == CURRENT_FIGURE) info->field[h][w] = OLD_FIGURE;
    }
}

void move_down(GameInfo_t *info) {
  if (down_check(*info) == ALLOW) {
    for (int h = DOWN_BOARD; h >= UP_BOARD; --h)
      for (int w = RIGHT_BOARD; w >= LEFT_BOARD; --w) {
        if (info->field[h][w] == CURRENT_FIGURE) {
          info->field[h][w] = EMPTY_PLACE;
          info->field[h + 1][w] = CURRENT_FIGURE;
        }
      }
    info->current.start_y += 1;
  }
}

void speedup(GameInfo_t *info) {
  move_down(info);
  move_down(info);
  move_down(info);
}

int full_line_check(GameInfo_t info, int line) {
  int check = 1;
  for (int w = LEFT_BOARD; w <= RIGHT_BOARD; ++w)
    if (info.field[line][w] != OLD_FIGURE) check = 0;
  return check;
}

void move_down_field(GameInfo_t *info, int base_line) {
  for (int h = base_line; h > UP_BOARD; --h) {
    for (int w = LEFT_BOARD; w <= RIGHT_BOARD; ++w) {
      if (info->field[h - 1][w] != CURRENT_FIGURE)
        info->field[h][w] = info->field[h - 1][w];
    }
  }
}

void delete_line(GameInfo_t *info, int line) {
  for (int w = LEFT_BOARD; w <= RIGHT_BOARD; ++w)
    info->field[line][w] = EMPTY_PLACE;
  move_down_field(info, line);
}

void level_change(GameInfo_t *tetris) {
  int level = tetris->score / 600 + 1;
  int level_dif = level - tetris->level;
  tetris->level += level_dif;
  tetris->speed -= level_dif * 100;
}

void max_score_save(GameInfo_t *tetris) {
  if (tetris->score > tetris->high_score) {
    FILE *file = fopen("score.txt", "w");
    if (file != NULL) {
      fprintf(file, "%d", tetris->score);
      fclose(file);
    }
  }
}

void max_score_find(GameInfo_t *tetris) {
  FILE *file = fopen("score.txt", "r");
  if (file != NULL) {
    int score = 0;
    fscanf(file, "%d", &score);
    tetris->high_score = score;
    fclose(file);
  }
}

void scoring(GameInfo_t *info, int row_count) {
  int points = 0;
  if (row_count == 1)
    points = 100;
  else if (row_count == 2)
    points = 300;
  else if (row_count == 3)
    points = 700;
  else if (row_count == 4)
    points = 1500;
  info->score += points;
  level_change(info);
  max_score_save(info);
  max_score_find(info);
}

void destroy_full_line(GameInfo_t *info) {
  int delete_count = 0;
  for (int h = DOWN_BOARD; h >= UP_BOARD; --h) {
    if (full_line_check(*info, h)) {
      delete_line(info, h);
      ++h;
      delete_count++;
    }
  }
  scoring(info, delete_count);
}

int is_game_end(GameInfo_t info) {
  int check = 0;
  for (int w = LEFT_BOARD; w <= RIGHT_BOARD; ++w)
    if (info.field[UP_BOARD][w] == OLD_FIGURE) check = 1;
  return check;
}

void turn(GameInfo_t *info) {
  Shape temp;
  for (int h = 0; h < PIECE_SIZE; h++) {
    for (int w = 0; w < PIECE_SIZE; w++) {
      temp.piece[w][PIECE_SIZE - h - 1] = info->current.piece[h][w];
    }
  }

  for (int h = 0; h < PIECE_SIZE; h++) {
    for (int w = 0; w < PIECE_SIZE; w++) {
      info->current.piece[h][w] = temp.piece[h][w];
    }
  }
}

void clean_current_figure(GameInfo_t *info) {
  for (int h = 0; h <= DOWN_BOARD; ++h)
    for (int w = 0; w <= RIGHT_BOARD; ++w)
      if (info->field[h][w] == CURRENT_FIGURE) info->field[h][w] = EMPTY_PLACE;
}

int check_figure_in_field(GameInfo_t *info) {
  int start_x = info->current.start_x;
  int check = ALLOW;
  for (int h = 0; h < PIECE_SIZE; ++h)
    for (int w = 0; w < PIECE_SIZE; ++w) {
      if (info->current.piece[h][w] == CURRENT_FIGURE)
        if (w + start_x > RIGHT_BOARD || w + start_x < LEFT_BOARD)
          check = FORBID;
    }
  return check;
}

int fix_figure_in_field(GameInfo_t *info) {
  int check = ALLOW;
  if (PIECE_SIZE + info->current.start_x > RIGHT_BOARD)
    if (left_check(*info) == ALLOW)
      info->current.start_x -= 1;
    else
      check = FORBID;
  else if (right_check(*info) == ALLOW)
    info->current.start_x += 1;
  else
    check = FORBID;

  while (check_figure_in_field(info) == FORBID && check == ALLOW) {
    fix_figure_in_field(info);
  }
  return check;
}

void figure_print(GameInfo_t *info) {
  clean_current_figure(info);
  for (int h = 0; h < PIECE_SIZE; h++)
    for (int w = 0; w < PIECE_SIZE; w++) {
      if (info->current.piece[h][w])
        info->field[h + info->current.start_y][w + info->current.start_x] =
            CURRENT_FIGURE;
    }
}

void cut_figure_left(GameInfo_t *info) {
  int shift = 0;
  for (int w = 0; w < PIECE_SIZE; ++w) {
    int check = ALLOW;
    for (int h = 0; h < PIECE_SIZE; ++h) {
      if (info->current.piece[h][w]) check = FORBID;
    }
    if (check == ALLOW) shift++;
  }
  if (shift > 0) {
    for (int w = 0; w + shift < PIECE_SIZE; ++w) {
      for (int h = 0; h < PIECE_SIZE; ++h) {
        info->current.piece[h][w] = info->current.piece[h][w + shift];
        info->current.piece[h][w + shift] = EMPTY_PLACE;
      }
    }
  }
}
void spin(GameInfo_t *info) {
  Shape temp = info->current;
  turn(info);
  cut_figure_left(info);
  if (check_figure_in_field(info) == ALLOW) {
    figure_print(info);
  } else {
    if (fix_figure_in_field(info) == ALLOW) {
      figure_print(info);
    } else {
      info->current = temp;
    }
  }
}

void crashing(GameInfo_t *tetris, tetris_state *state) {
  crash(tetris);
  destroy_full_line(tetris);
  *state = SPAWN;
  if (is_game_end(*tetris)) *state = GAMEOVER;
}

void clean_field(GameInfo_t *tetris) {
  for (int i = 0; i < BOARD_HEIGHT; ++i)
    for (int j = 0; j < BOARD_WIDTH; ++j) tetris->field[i][j] = EMPTY_PLACE;
}

void start_new_game(GameInfo_t *info) {
  clean_field(info);
  info->level = 1;
  info->score = 0;
  info->speed = 1000;
  max_score_find(info);
  info->current.name = random_figure(info);
  info->next.name = random_figure(info);
  fill_figures(info);
}
