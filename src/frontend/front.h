#ifndef _TETRIS_FRONT_H_
#define _TETRIS_FRONT_H_

#include "../../brick_game/tetris/tetris.h"

void print_start_image(WINDOW *start_window);
void start_window(tetris_state *state);
void print_cell(int h, int w, GameInfo_t *tetris, WINDOW *window);
void print_field(GameInfo_t info, WINDOW *window);
void print_end_image(WINDOW *end_window);
void end_window(tetris_state *state);
void print_pause_image(WINDOW *pause_window);
void pause_window(tetris_state *state);
void print_cell_next_figure(WINDOW *param_window, GameInfo_t tetris, int h,
                            int w);
void print_next_figure(WINDOW *param_window, GameInfo_t tetris);
void print_params(WINDOW *param_window, GameInfo_t tetris);
void print_params_window(GameInfo_t tetris, WINDOW *param_window);
void update_windows(GameInfo_t tetris, WINDOW *window, WINDOW *param_window);
void init_window();

#endif
