#include "front.h"

int shapes[7][4][4] = {
    {{2, 2, 0, 0}, {2, 2, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{2, 0, 0, 0}, {2, 0, 0, 0}, {2, 0, 0, 0}, {2, 0, 0, 0}},
    {{2, 0, 0, 0}, {2, 2, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 2, 0}, {2, 2, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 2, 2, 0}, {2, 2, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{2, 2, 0, 0}, {0, 2, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{2, 2, 2, 0}, {0, 2, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

void print_start_image(WINDOW *start_window) {
  wattron(start_window, COLOR_PAIR(3));
  int i_index = 5;
  int j_index = 2;
  mvwprintw(start_window, i_index + 0, j_index,
            "   _____ ____ _____ ______ ____  ____ ");
  mvwprintw(start_window, i_index + 1, j_index,
            "  /_  _// __//_  _// __  //_ _/ / __/ ");
  mvwprintw(start_window, i_index + 2, j_index,
            "   / / / /_   / / / /_/ /  //  //__  ");
  mvwprintw(start_window, i_index + 3, j_index,
            "  / / / __/  / / / _  _/  //  /__ / ");
  mvwprintw(start_window, i_index + 4, j_index,
            " / / / /_   / / / // /  _//_  __// ");
  mvwprintw(start_window, i_index + 5, j_index,
            "/_/ /___/  /_/ /_//_/  /___//___/ ");

  wattron(start_window, COLOR_PAIR(1));
  mvwprintw(start_window, i_index + 15, j_index + 10, "_________________");
  wattron(start_window, COLOR_PAIR(4));
  mvwprintw(start_window, i_index + 17, j_index + 10, "    S T A R T    ");
  wattron(start_window, COLOR_PAIR(1));
  mvwprintw(start_window, i_index + 23, j_index + 10, "_________________");
  mvwprintw(start_window, i_index + 21, j_index + 10, "     E X I T   ");
}

void start_window(tetris_state *state) {
  init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA);
  WINDOW *start_window =
      newwin(BOARD_HEIGHT * 2 + 2, BOARD_WIDTH * 4 + 2, 0, 0);
  wattron(start_window, COLOR_PAIR(1));
  box(start_window, 0, 0);
  int menu_flag = 1;
  print_start_image(start_window);
  keypad(start_window, TRUE);
  int c;
  while ((c = wgetch(start_window)) != '\n') {
    if (c == KEY_DOWN || c == KEY_UP) menu_flag = !menu_flag;
    if (menu_flag)
      wattron(start_window, COLOR_PAIR(4));
    else
      wattron(start_window, COLOR_PAIR(1));
    mvwprintw(start_window, 22, 12, "    S T A R T    ");
    if (!menu_flag)
      wattron(start_window, COLOR_PAIR(4));
    else
      wattron(start_window, COLOR_PAIR(1));
    mvwprintw(start_window, 26, 12, "     E X I T     ");
    wrefresh(start_window);
  }
  if (!menu_flag)
    *state = GAMEOVER;
  else
    *state = SPAWN;
  delwin(start_window);
  endwin();
}

void print_cell(int h, int w, GameInfo_t *tetris, WINDOW *window) {
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 4; ++j)
      if (tetris->field[h][w] == CURRENT_FIGURE)
        mvwaddch(window, h * 2 + i + 1, w * 4 + j + 1, ' ' | A_REVERSE);
      else if (tetris->field[h][w] == OLD_FIGURE)
        mvwaddch(window, h * 2 + i + 1, w * 4 + j + 1, ' ' | COLOR_PAIR(2));
      else
        mvwaddch(window, h * 2 + i + 1, w * 4 + j + 1, ' ');
}

void print_field(GameInfo_t info, WINDOW *window) {
  for (int h = 0; h < BOARD_HEIGHT; ++h) {
    for (int w = 0; w < BOARD_WIDTH; ++w) {
      print_cell(h, w, &info, window);
    }
  }
  wrefresh(window);
}

void print_end_image(WINDOW *end_window) {
  wattron(end_window, COLOR_PAIR(3));
  int i_index = 4;
  int j_index = 3;

  mvwprintw(end_window, i_index + 0, j_index,
            "     _____ ______ _     _ ____ ");
  mvwprintw(end_window, i_index + 1, j_index, "    /____// __  ///_  _/// __/");
  mvwprintw(end_window, i_index + 2, j_index, "   //___ / /_/ //  /_/ // /_ ");
  mvwprintw(end_window, i_index + 3, j_index, "  ///_ // __  // ___  // __/");
  mvwprintw(end_window, i_index + 4, j_index, " //__/// / / // /  / // /_ ");
  mvwprintw(end_window, i_index + 5, j_index, "/____//_/ /_//_/  /_//___/");

  i_index += 6;
  j_index += 9;

  mvwprintw(end_window, i_index + 0, j_index, "     ____ __  __ ____ ______ ");
  mvwprintw(end_window, i_index + 1, j_index, "    /__ // / / // __// __  /");
  mvwprintw(end_window, i_index + 2, j_index, "   // /// / / // /_ / /_/ /");
  mvwprintw(end_window, i_index + 3, j_index, "  // /// /_/ // __// _  _/");
  mvwprintw(end_window, i_index + 4, j_index, " //_///_  __// /_ / // /");
  mvwprintw(end_window, i_index + 5, j_index, "/___/  /_/  /___//_//_/");
  i_index = 5;
  j_index = 2;
  wattron(end_window, COLOR_PAIR(1));
  mvwprintw(end_window, i_index + 15, j_index + 10, "_________________");
  wattron(end_window, COLOR_PAIR(4));
  mvwprintw(end_window, i_index + 17, j_index + 10, "  N E W  G A M E ");
  wattron(end_window, COLOR_PAIR(1));
  mvwprintw(end_window, i_index + 23, j_index + 10, "_________________");
  mvwprintw(end_window, i_index + 21, j_index + 10, "   G I V E  U P     ");
}

void end_window(tetris_state *state) {
  WINDOW *end_window = newwin(BOARD_HEIGHT * 2 + 2, BOARD_WIDTH * 4 + 2, 0, 0);
  wattron(end_window, COLOR_PAIR(1));
  box(end_window, 0, 0);
  int menu_flag = 1;
  print_end_image(end_window);
  keypad(end_window, TRUE);
  int c;
  while ((c = wgetch(end_window)) != '\n') {
    if (c == KEY_DOWN || c == KEY_UP) menu_flag = !menu_flag;
    if (menu_flag)
      wattron(end_window, COLOR_PAIR(4));
    else
      wattron(end_window, COLOR_PAIR(1));
    mvwprintw(end_window, 22, 12, "  N E W  G A M E ");
    if (!menu_flag)
      wattron(end_window, COLOR_PAIR(4));
    else
      wattron(end_window, COLOR_PAIR(1));
    mvwprintw(end_window, 26, 12, "   G I V E  U P  ");
    wrefresh(end_window);
  }
  if (!menu_flag)
    *state = GAMEOVER;
  else
    *state = START;
  delwin(end_window);
  endwin();
}

void print_pause_image(WINDOW *pause_window) {
  wattron(pause_window, COLOR_PAIR(3));
  int i_index = 5;
  int j_index = 2;

  mvwprintw(pause_window, i_index + 0, j_index,
            "     ______ ______ __   __ _____ _____ ");
  mvwprintw(pause_window, i_index + 1, j_index,
            "    / __  // __  // /  / // ___// ___/");
  mvwprintw(pause_window, i_index + 2, j_index,
            "   / /_/ // /_/ // /  / ///___ / /__  ");
  mvwprintw(pause_window, i_index + 3, j_index,
            "  / ____// __  // /  / //___ // ___/");
  mvwprintw(pause_window, i_index + 4, j_index,
            " / /    / / / // /__/ / ___/// /__  ");
  mvwprintw(pause_window, i_index + 5, j_index,
            "/_/    /_/ /_//______//____//____/ ");

  wattron(pause_window, COLOR_PAIR(1));
  mvwprintw(pause_window, i_index + 15, j_index + 10, "_________________");
  wattron(pause_window, COLOR_PAIR(4));
  mvwprintw(pause_window, 22, 12, " C O N T I N U E ");
  wattron(pause_window, COLOR_PAIR(1));
  mvwprintw(pause_window, i_index + 23, j_index + 10, "_________________");
  mvwprintw(pause_window, i_index + 21, j_index + 10, "     E X I T   ");
}

void pause_window(tetris_state *state) {
  WINDOW *pause_window =
      newwin(BOARD_HEIGHT * 2 + 2, BOARD_WIDTH * 4 + 2, 0, 0);
  wtimeout(pause_window, -1);
  wattron(pause_window, COLOR_PAIR(1));
  box(pause_window, 0, 0);
  print_pause_image(pause_window);
  int menu_flag = 1;
  keypad(pause_window, TRUE);
  int c;
  while ((c = wgetch(pause_window)) != '\n') {
    if (c == KEY_DOWN || c == KEY_UP) menu_flag = !menu_flag;
    if (menu_flag)
      wattron(pause_window, COLOR_PAIR(4));
    else
      wattron(pause_window, COLOR_PAIR(1));
    mvwprintw(pause_window, 22, 12, " C O N T I N U E ");
    if (!menu_flag)
      wattron(pause_window, COLOR_PAIR(4));
    else
      wattron(pause_window, COLOR_PAIR(1));
    mvwprintw(pause_window, 26, 12, "     E X I T     ");
    wrefresh(pause_window);
  }
  if (!menu_flag)
    *state = GAMEOVER;
  else
    *state = SHIFTING;
  delwin(pause_window);
  endwin();
}

void print_cell_next_figure(WINDOW *param_window, GameInfo_t tetris, int h,
                            int w) {
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 4; ++j)
      if (tetris.next.piece[h][w])
        mvwaddch(param_window, h * 2 + i + 1, w * 4 + j + 1, ' ' | A_REVERSE);
      else
        mvwaddch(param_window, h * 2 + i + 1, w * 4 + j + 1, ' ');
}

void print_next_figure(WINDOW *param_window, GameInfo_t tetris) {
  for (int h = 0; h < PIECE_SIZE; ++h)
    for (int w = 0; w < PIECE_SIZE; ++w)
      print_cell_next_figure(param_window, tetris, h, w);
  wrefresh(param_window);
}

void print_params(WINDOW *param_window, GameInfo_t tetris) {
  wattron(param_window, COLOR_PAIR(1));
  mvwprintw(param_window, 18, 8, "<<<<<<<<<<<<<<<<");
  wattroff(param_window, COLOR_PAIR(1));

  mvwprintw(param_window, 20, 8, "L E V E L : %d", tetris.level);
  mvwprintw(param_window, 23, 8, "S C O R E : %d", tetris.score);
  mvwprintw(param_window, 26, 8, "R E C O R D : %d", tetris.high_score);

  wattron(param_window, COLOR_PAIR(1));
  mvwprintw(param_window, 28, 8, "<<<<<<<<<<<<<<<<");
  wattron(param_window, COLOR_PAIR(3));

  mvwprintw(param_window, 34, 8, "G O O D  L U C K");
}

void print_params_window(GameInfo_t tetris, WINDOW *param_window) {
  wattroff(param_window, COLOR_PAIR(3));
  mvwprintw(param_window, 3, 4, " N E X T  F I G U R E :");
  WINDOW *next_figure_window = newwin(10, 18, 5, 49);
  box(next_figure_window, 0, 0);
  wattron(next_figure_window, COLOR_PAIR(1));
  print_next_figure(next_figure_window, tetris);
  print_params(param_window, tetris);
  wrefresh(param_window);
}

void update_windows(GameInfo_t tetris, WINDOW *window, WINDOW *param_window) {
  print_field(tetris, window);
  print_params_window(tetris, param_window);
}

void init_window() {
  initscr();
  start_color();
  noecho();
  curs_set(0);
  srand(time(0));
  init_color(COLOR_CYAN, 0, 900, 900);
  init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_CYAN);
  init_pair(3, COLOR_CYAN, COLOR_BLACK);
}
