#include "tetris.h"

void user_input(WINDOW *window, GameInfo_t *tetris, tetris_state *state) {
  keypad(window, TRUE);
  wtimeout(window, tetris->speed);
  int ch = wgetch(window);
  switch (ch) {
    case KEY_LEFT:
      move_left(tetris);
      break;
    case KEY_DOWN:
      speedup(tetris);
      break;
    case KEY_RIGHT:
      move_right(tetris);
      break;
    case ' ':
      spin(tetris);
      break;
    case 'q':
      *state = GAMEOVER;
      break;
    case 'p':
      *state = PAUSE;
      break;
  }
}

void game_loop(WINDOW *window, WINDOW *param_window, GameInfo_t tetris) {
  tetris_state state = START;
  int break_flag = FALSE;
  while (!break_flag) {
    if (state == START)
      start_window(&state);
    else if (state == GAMEOVER) {
      end_window(&state);
      if (state == START)
        start_new_game(&tetris);
      else
        break_flag = TRUE;
    } else if (state == SPAWN) {
      new_spawn(&tetris);
      state = MOVING;
    } else if (state == MOVING) {
      state = SHIFTING;
      user_input(window, &tetris, &state);
    } else if (state == SHIFTING) {
      if (down_check(tetris) == FORBID)
        state = ATTACHING;
      else {
        move_down(&tetris);
        state = MOVING;
      }
    } else if (state == ATTACHING)
      crashing(&tetris, &state);
    else if (state == PAUSE)
      pause_window(&state);
    update_windows(tetris, window, param_window);
  }
  delwin(param_window);
  endwin();
}

int main() {
  init_window();
  WINDOW *window = newwin(BOARD_HEIGHT * 2 + 2, BOARD_WIDTH * 4 + 2, 0, 0);
  wattron(window, COLOR_PAIR(1));
  box(window, 0, 0);
  WINDOW *param_window =
      newwin(BOARD_HEIGHT * 2 + 2, BOARD_WIDTH * 3 + 2, 0, 42);
  wattron(param_window, COLOR_PAIR(1));
  box(param_window, 0, 0);
  GameInfo_t tetris;
  start_new_game(&tetris);
  game_loop(window, param_window, tetris);
  delwin(window);
  return 0;
}
