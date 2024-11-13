/*
 * SIGMA RIZZLER
 *
 * A simple turn-based game about Sigma and Rizz
 */

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
    char username[100];
    int health;
    int score;
} Person;

typedef struct {
    char name[100];
    int health;
} Enemy;

void attack(int *hp) {
    srand(time(NULL));
    int random = (rand() % 16) + 5;
    int atk = random;
    *hp -= atk;
}

void heal(int *hp) {
    srand(time(NULL));
    int random = (rand() % 9) + 5;
    int heal = random;
    *hp += heal;
}

int compare(const void* a, const void* b) {
   return ((Person*)b)->score - ((Person*)a)->score;
}

void save_score(Person player){
    FILE *fp = fopen("scoreboard.txt", "a");
    fprintf(fp, "%s#%d\n", player.username, player.score);
    fclose(fp);
}

void leaderboard() {
    FILE *fp = fopen("scoreboard.save", "r");
    if (fp == NULL) {
        printf("Leaderboard empty. No one has played yet :(\n");
        return;
    }
     
    Person player[500]; 
    int count = 0;
    puts("=============================");
    puts("|         Scoreboard        |");
    puts("=============================");
    puts("|   Username   |    Score   |");
    puts("=============================");
    while (fscanf(fp, "%[^#]#%d\n", player[count].username, &player[count].score) != EOF) {
        count++;
    }
    qsort(player, count, sizeof(Person), compare);
    for(int i = 0; i<count;i++){
         printf("|%-14.14s|%12d|\n", player[i].username, player[i].score);
    }
    puts("=============================");
    fclose(fp);
}


void print_authentication() {

}

void auth_register() {

}
void print_logo() {
    const char* LOGO = "   _____ ____________  ______ \n"           
                       "  / ___//  _/ ____/  |/  /   |\n"               
                       "  \\__ \\ / // / __/ /|_/ / /| |\n"
                       " ___/ // // /_/ / /  / / ___ |\n"
                       "/____/___/\\____/_/  /_/_/  |_|\n\n";

    printf(LOGO);
}
void print_menu() {
    puts("(P)lay game\n"
         "(L)eaderboard\n"
         "(Q)uit game");
}


int main() {   
    char confirmation;
    print_logo();
    do{
        print_menu();
        printf("Enter your choice: ");
        scanf(" %c", &confirmation);
        switch (tolower(confirmation)) {
            case 'p':
                Person player;
                return 0;
                //save_score(player);
                break;
            case 'l':
                leaderboard();
                puts("Press any key to continue...");
                getchar(); getchar();
                break;
            case 'q':
                printf("Thanks for playing");
                return 0;
            default:
                puts("Invalid choice!");
    }
    }while(1);
    
    return 0;
}