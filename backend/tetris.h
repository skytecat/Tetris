#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define LEFT_BOARD 0
#define RIGHT_BOARD 9
#define UP_BOARD 0
#define DOWN_BOARD 19

#define PIECE_SIZE 4

#define ALLOW 0
#define FORBID 1

#define CURRENT_FIGURE 2
#define OLD_FIGURE 1
#define EMPTY_PLACE 0

#define SPAWN_X 4
#define SPAWN_Y 0

typedef enum { O, I, L, J, S, Z, T } figure_name;

typedef struct Shape {
  figure_name name;
  int piece[4][4];
  int start_x;
  int start_y;
} Shape;

extern int shapes[7][4][4];

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  PAUSE
} tetris_state;

typedef struct {
  int field[BOARD_HEIGHT][BOARD_WIDTH];
  Shape current;
  Shape next;
  int score;
  int high_score;
  int level;
  int speed;
} GameInfo_t;

#include "../../gui/cli/front.h"

int random_figure();
void fill_figures(GameInfo_t *info);
void new_spawn(GameInfo_t *info);
int left_check(GameInfo_t info);
void move_left(GameInfo_t *info);
int right_check(GameInfo_t info);
void move_right(GameInfo_t *info);
int down_check(GameInfo_t info);
void crash(GameInfo_t *info);
void move_down(GameInfo_t *info);
void speedup(GameInfo_t *info);
int full_line_check(GameInfo_t info, int line);
void move_down_field(GameInfo_t *info, int base_line);
void delete_line(GameInfo_t *info, int line);
void level_change(GameInfo_t *tetris);
void max_score_save(GameInfo_t *tetris);
void max_score_find(GameInfo_t *tetris);
void scoring(GameInfo_t *info, int row_count);
void destroy_full_line(GameInfo_t *info);
int is_game_end(GameInfo_t info);
void turn(GameInfo_t *info);
void clean_current_figure(GameInfo_t *info);
int check_figure_in_field(GameInfo_t *info);
int fix_figure_in_field(GameInfo_t *info);
void figure_print(GameInfo_t *info);
void cut_figure_left(GameInfo_t *info);
void spin(GameInfo_t *info);
void user_input(WINDOW *window, GameInfo_t *tetris, tetris_state *state);
void crashing(GameInfo_t *tetris, tetris_state *state);
void clean_field(GameInfo_t *tetris);
void start_new_game(GameInfo_t *info);
void game_loop(WINDOW *window, WINDOW *param_window, GameInfo_t tetris);

#endif