#ifndef SIGMA_H
#define SIGMA_H

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#define SLEEP(ms) usleep((ms) * 1000)
#endif

enum Difficulty {
    EASY,
    NORMAL,
    HARD,
};

// Default Difficulty

typedef struct {
    char username[100];
    int health;
    int score;
    int poison;
    int abundant;
    int poison_counter;
    int is_vulnerable;
    char difficulty[20];
} Person;

typedef struct
{
    char name[100];
    int max_health;
    int health;
    int scoreObtained;
    int poison;
    int poison_counter;
    int abundant;
    int is_vulnerable;
} Enemy;

void free_memory();
void init_screen();

#endif // !SIGMA_H
