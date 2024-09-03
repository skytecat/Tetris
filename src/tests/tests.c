#include "tests.h"

#include "../brick_game/tetris/tetris.h"
#include "../gui/cli/front.h"

START_TEST(test_clear_field) {
  GameInfo_t tetris;

  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 10; x++) {
      tetris.field[y][x] = 1;
    }
  }

  clean_field(&tetris);

  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 10; x++) {
      fail_unless(tetris.field[y][x] == 0);
    }
  }
}
END_TEST

START_TEST(test_left) {
  GameInfo_t tetris = {0};

  tetris.current.start_x = 5;

  move_left(&tetris);

  fail_unless(tetris.current.start_x == 4);

  tetris.field[5][4] = CURRENT_FIGURE;

  move_left(&tetris);

  fail_unless(tetris.field[5][3] == CURRENT_FIGURE);
}
END_TEST

START_TEST(test_left_check) {
  GameInfo_t tetris = {0};

  tetris.current.start_x = 1;
  clean_field(&tetris);
  tetris.field[1][3] = 2;

  fail_unless(left_check(tetris) == 0);

  clean_field(&tetris);
  tetris.field[3][0] = 2;

  tetris.current.start_x = 0;

  fail_unless(left_check(tetris) == 1);
}
END_TEST

START_TEST(test_right) {
  GameInfo_t tetris = {0};
  tetris.current.start_x = 4;

  move_right(&tetris);

  fail_unless(tetris.current.start_x == 5);

  tetris.field[3][5] = CURRENT_FIGURE;

  move_right(&tetris);

  fail_unless(tetris.field[3][6] == CURRENT_FIGURE);
}
END_TEST

START_TEST(test_right_check) {
  GameInfo_t tetris = {0};

  tetris.current.start_x = 19;
  clean_field(&tetris);
  tetris.field[1][19] = 2;

  fail_unless(right_check(tetris) == 1);

  tetris.current.start_x = 4;

  clean_field(&tetris);
  tetris.field[1][8] = 2;

  fail_unless(right_check(tetris) == 0);
}
END_TEST

START_TEST(test_down) {
  GameInfo_t tetris = {0};

  tetris.current.start_x = 5;
  tetris.current.start_y = 5;

  move_down(&tetris);

  fail_unless(tetris.current.start_y == 6);
  tetris.field[6][3] = CURRENT_FIGURE;

  move_down(&tetris);

  fail_unless(tetris.field[7][3] == CURRENT_FIGURE);
}
END_TEST

START_TEST(test_down_check) {
  GameInfo_t tetris = {0};

  tetris.current.start_x = 3;
  clean_field(&tetris);
  tetris.field[19][3] = 2;

  fail_unless(down_check(tetris) == 1);
}
END_TEST

START_TEST(test_speedup) {
  GameInfo_t tetris = {0};

  tetris.current.start_x = 5;
  tetris.current.start_y = 5;

  speedup(&tetris);

  fail_unless(tetris.current.start_y == 8);

  speedup(&tetris);

  fail_unless(tetris.current.start_y == 11);
}
END_TEST

START_TEST(test_random_figure) {
  figure_name figure = 0;

  figure = random_figure();

  fail_unless(figure >= 0 && figure <= 6);

  figure = random_figure();

  fail_unless(figure >= 0 && figure <= 6);
}
END_TEST

START_TEST(test_full_line) {
  GameInfo_t tetris;
  int line = 17;
  for (int j = 0; j < 10; ++j) tetris.field[line][j] = OLD_FIGURE;

  fail_unless(full_line_check(tetris, line));
}
END_TEST

START_TEST(test_delete_line) {
  GameInfo_t tetris;
  clean_field(&tetris);
  int line = 17;
  for (int j = 0; j < 10; ++j) tetris.field[line][j] = OLD_FIGURE;
  delete_line(&tetris, line);

  int check = 1;

  for (int j = 0; j < 10; ++j)
    if (tetris.field[line][j] != EMPTY_PLACE) check = 0;
  fail_unless(check);
}
END_TEST

START_TEST(test_is_game_end) {
  GameInfo_t tetris;
  clean_field(&tetris);
  tetris.field[0][7] = OLD_FIGURE;
  fail_unless(is_game_end(tetris));
}
END_TEST

START_TEST(test_crash) {
  GameInfo_t tetris;
  clean_field(&tetris);
  tetris.field[19][7] = CURRENT_FIGURE;
  crash(&tetris);
  fail_unless(tetris.field[19][7] == OLD_FIGURE);
}
END_TEST

START_TEST(test_level_change) {
  GameInfo_t tetris;
  clean_field(&tetris);
  tetris.level = 1;
  tetris.score = 700;
  level_change(&tetris);
  fail_unless(tetris.level == 2);
}
END_TEST

START_TEST(test_scoring) {
  GameInfo_t tetris;
  start_new_game(&tetris);
  tetris.score = 0;
  scoring(&tetris, 1);
  fail_unless(tetris.score == 100);
  tetris.score = 0;
  scoring(&tetris, 2);
  fail_unless(tetris.score == 300);
  tetris.score = 0;
  scoring(&tetris, 3);
  fail_unless(tetris.score == 700);
  tetris.score = 0;
  scoring(&tetris, 4);
  fail_unless(tetris.score == 1500);
}
END_TEST

START_TEST(test_crashing) {
  GameInfo_t tetris;
  tetris_state state = MOVING;
  start_new_game(&tetris);
  tetris.field[0][7] = OLD_FIGURE;
  crashing(&tetris, &state);
  fail_unless(state == GAMEOVER);
}
END_TEST

START_TEST(test_new_spawn) {
  GameInfo_t tetris;
  tetris.next.name = 3;
  new_spawn(&tetris);
  fail_unless(tetris.current.name == 3);
}
END_TEST

START_TEST(test_clean_current_figure) {
  GameInfo_t tetris;
  tetris.field[2][3] = CURRENT_FIGURE;
  clean_current_figure(&tetris);
  fail_unless(tetris.field[2][3] == EMPTY_PLACE);
}
END_TEST

START_TEST(test_cut_figure) {
  GameInfo_t tetris;
  tetris.current.piece[3][3] = CURRENT_FIGURE;
  cut_figure_left(&tetris);
  fail_unless(tetris.current.piece[0][3] == CURRENT_FIGURE);
}
END_TEST

START_TEST(test_spin) {
  GameInfo_t tetris;
  tetris.current.piece[0][0] = CURRENT_FIGURE;
  tetris.current.piece[1][0] = CURRENT_FIGURE;
  tetris.current.piece[2][0] = CURRENT_FIGURE;
  tetris.current.piece[3][0] = CURRENT_FIGURE;
  turn(&tetris);
  fail_unless(tetris.current.piece[0][1] == CURRENT_FIGURE);
}
END_TEST

START_TEST(test_figure_in_field) {
  GameInfo_t tetris;
  tetris.current.start_x = -1;
  tetris.current.piece[0][0] = CURRENT_FIGURE;
  tetris.current.piece[1][0] = CURRENT_FIGURE;
  tetris.current.piece[2][0] = CURRENT_FIGURE;
  tetris.current.piece[3][0] = CURRENT_FIGURE;
  fail_unless(check_figure_in_field(&tetris));
}
END_TEST

START_TEST(test_print_figure) {
  GameInfo_t tetris;
  tetris.current.start_x = 0;
  tetris.current.start_y = 0;
  tetris.current.piece[0][0] = CURRENT_FIGURE;
  tetris.current.piece[1][0] = CURRENT_FIGURE;
  tetris.current.piece[2][0] = CURRENT_FIGURE;
  tetris.current.piece[3][0] = CURRENT_FIGURE;
  figure_print(&tetris);
  fail_unless(tetris.field[2][0]);
}
END_TEST

int main(void) {
  Suite *s = suite_create("\033[45m-=S21_GAME_TEST=-\033[0m");
  TCase *tc;

  tc = tcase_create("s21_game");
  tcase_add_test(tc, test_clear_field);
  tcase_add_test(tc, test_left);
  tcase_add_test(tc, test_right);
  tcase_add_test(tc, test_down);
  tcase_add_test(tc, test_speedup);
  tcase_add_test(tc, test_random_figure);
  tcase_add_test(tc, test_left_check);
  tcase_add_test(tc, test_right_check);
  tcase_add_test(tc, test_full_line);
  tcase_add_test(tc, test_delete_line);
  tcase_add_test(tc, test_is_game_end);
  tcase_add_test(tc, test_crash);
  tcase_add_test(tc, test_level_change);
  tcase_add_test(tc, test_scoring);
  tcase_add_test(tc, test_crashing);
  tcase_add_test(tc, test_new_spawn);
  tcase_add_test(tc, test_down_check);
  tcase_add_test(tc, test_clean_current_figure);
  tcase_add_test(tc, test_cut_figure);
  tcase_add_test(tc, test_spin);
  tcase_add_test(tc, test_figure_in_field);
  tcase_add_test(tc, test_print_figure);

  suite_add_tcase(s, tc);

  SRunner *runner = srunner_create(s);
  srunner_run_all(runner, CK_NORMAL);
  int fail = 0;
  fail = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (fail == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
