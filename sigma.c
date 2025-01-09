#include "sigma.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

Person *user;
Enemy *enemy;
bool is_story_mode = false;
int turn = -1;

// Default difficulty
enum Difficulty difficulty = EASY;
void free_memory() {
    free(user);
    free(enemy);
}

void init_screen() {
    initscr();
    printw("test\n");
    getch();
}

// TODO
