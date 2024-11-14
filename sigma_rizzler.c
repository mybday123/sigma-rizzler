/*
 * SIGMA RIZZLER
 *
 * A simple turn-based game about Sigma and Rizz
 */

// Set library that is going to be needed
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define CLEAR_SCREEN_REGEX "\e[1;1H\e[2J"
////////////////////////////////////////////////

// Struct of person and enemy section
typedef struct
{
    char username[100];
    int health;
    int score;
} Person;

Person user;

typedef struct
{
    char name[100];
    int health;
}Enemy;

int isvulnurable = 0;
int abundant = 0;
int poison = 1;
int poison_counter = 0;


int gacha(){
    srand(time(NULL));

    int random = (rand() % 10) + 1;
    return random;
}
int gacha_vulnuraability(){
    srand(time(NULL));

    int random = (rand() % 7) + 4;
    return random;
}

int gacha_poison(){
    srand(time(NULL));

    int random = (rand() % 7) + 2;
    return random;
}


int buff(){
    int random = gacha();
    if (random >= 5){
        srand (time(NULL));
        int buff = (rand() % 6) + 5;

        return buff;
        printf("The arrow of the hunt has assisted you with a buff of %d\n", buff);

    }


}
int pregame_announcement(int turn){
    if (turn >= 3){
        printf("Both buff and debuffs for you and the enemy are active! Push on with care!\n");
        srand (time(NULL));
        int notice = gacha();
        if (notice <= 3){
            printf("Enemy is vulnurable for this turn! Bring in the Damage!\n");
            isvulnurable = 1;
        }
        else if(notice > 3 && notice <= 6){
            printf("Goddess of Abundance have blessed you! Healing now has improved for this turn!\n");
            abundant = 1;
        }
        else if(notice >= 7 && notice <= 9){
            printf("The enemy have been afflicted by poison! They will take damage overtime for 3 turns\n");
            poison = 1;
            poison_counter = 3;

        }
    }

}

void apply_poison(int *hp) {
    if (poison_counter > 0 && poison == 1) {
        int poison_damage = gacha_poison();
        *hp -= poison_damage;
        printf("Poison damage of %d applied! Remaining poison turns: %d\n", poison_damage, poison_counter);
        poison_counter--;
        if(poison_counter == 0){
            poison == 0;
        }
    }
    else {
        poison = 0;
    }
}






Enemy enemy[5] = {
    {"Ambatron", 100, 500},
    {"Rusdi", 120, 1000},
    {"Ambatukers", 140, 1500},
    {"Ngamutron", 160, 2000},
    {"Gambatron", 180, 2500}};
//////////////////////////////////////////////

Enemy *generateEnemy()
{
    srand(time(NULL));
    return &enemy[(rand() % 6)];
}

void printUser()
{
    if (user.health > 75)
    {
        printf("        *****        \n");
        printf("     *         *     \n");
        printf("   *    O   O    *   \n");
        printf("  *      \\_/      *  \n");
        printf("   *             *   \n");
        printf("     *         *     \n");
        printf("        *****        \n\n");
    }
    else if (user.health > 50 && user.health <= 75)
    {
        printf("        *****        \n");
        printf("     *         *     \n");
        printf("   *    O   O    *   \n");
        printf("  *       ---      * \n");
        printf("   *             *   \n");
        printf("     *         *     \n");
        printf("        *****        \n\n");
    }
    else if (user.health > 0 && user.health <= 50)
    {
        printf("        *****        \n");
        printf("     *         *     \n");
        printf("   *    O   O    *   \n");
        printf("  *       ---      * \n");
        printf("   *      / \\      * \n");
        printf("     *         *     \n");
        printf("        *****        \n\n");
    }
    else
    {
        printf("        *****        \n");
        printf("     *         *     \n");
        printf("   *    X   X    *   \n");
        printf("  *       ---      * \n");
        printf("   *             *   \n");
        printf("     *         *     \n");
        printf("        *****        \n");
    }
}

void getPlayerName()
{
    char agreement = ' ';
    int rejectionLevel = 1;
    do
    {
        printf("%s", CLEAR_SCREEN_REGEX);
        switch (rejectionLevel)
        {
        case 1:
            printf("Hey, you're so skibidi, can i get your name ?\n");
            break;
        case 2:
            printf("Hey, you're more skibidii than i think, so i need your name!\n");
            break;
        case 3:
            printf("HEYY, I NEED YOUR NAME PLEASE \?\?!?!\n");
            break;
        default:
            printf("To be honest, i really need your name, so please!!\n");
            break;
        }
        printf("Your answer (y/n): ");
        scanf("%c", &agreement);
        getchar();
        rejectionLevel++;
    } while (tolower(agreement) != 'y');

    int isNameFilled = 0;
    do
    {
        printf("Add your username, skibidi soldier : ");
        char temp[10];
        scanf("%9s", temp);
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        if (strlen(temp) <= 10)
        {
            isNameFilled = 1;
            strcpy(user.username, temp);
        }
    } while (isNameFilled == 0);
}


void attack(int *hp, int turn){
    srand(time(NULL));

    int random = (rand() % 16) + 5;
     int atk = random;
    if (turn < 3){
    *hp -= atk;
    }
    else {
        int buff_atk = buff(atk) + atk;
        if(isvulnurable == 1){
            buff_atk += gacha_vulnuraability();
            isvulnurable = 0;
        }
        *hp -= buff_atk;

    }
}

int gacha_buffheal(){
    srand (time(NULL));
    int random = (rand() % 11) + 10;
    return random;
}

void heal(int *hp)
{
    srand(time(NULL));
    int random = (rand() % 9) + 5;
    int heal = random;
    if (abundant == 1){
        heal += gacha_buffheal();
        if (poison == 1){
            poison == 0;
            printf("Poison has been cured!\n");

        }
    }
    *hp += heal;
}
void take_turn(int *hp, int turn) {
    apply_poison(hp);
    pregame_announcement(turn); // Check for any new effects or buffs

}



void playerTurn(Enemy *enemy)
{
    char decision = ' ';
    printUser();
    printf("Player : \n");
    printf("Name : %s\n", user.username);
    printf("Health : %d\n", user.health);
    printf("Score : %d\n", user.score);
    do
    {
        puts("(H)eal\n"
             "(A)ttack\n"
             "(S)kip ");
        scanf("%c", &decision);
        getchar();
    } while (tolower(decision) != 'a' && tolower(decision) != 'h' && tolower(decision) != 's');
    switch(tolower(decision)) {
        case 'a' :
            attack(&enemy->health);
            break;
        case 'h' :
            heal(&user.health);
            break;
        case 's' :
            break;
    }
}

void enemyTurn(Enemy *enemy){
    srand(time(NULL));
    int random = (rand() % 9);
    if(random > 5){
        heal(&enemy->health);
    }else{
        attack(&user.health);
    }
    int heal = random;
}



int checkCondition(Enemy *enemy)
{
    if(user.health <= 0){
        return -1;
    }else if(enemy->health <= 0) {
        user.score += enemy->scoreObtained;
        return 1;
    }else {
        return 0;
    }
}

void playGame()
{
    int isPlayerDie = 0;

    user.health = 100;
    getPlayerName();
    printf("%s", user.username);
    do
    {
        int defeatedEnemy = 0;
        Enemy *enemy = generateEnemy();
        while (defeatedEnemy != 1)
        {
            printf("Enemy's health : %d", enemy->health);
            playerTurn(enemy);
            int result = checkCondition(enemy);
            if(result == 1){
                defeatedEnemy = 1;
                break;
            }else if(result == -1){
                isPlayerDie = 1;
                break;
            }
            enemyTurn(enemy);
            result = checkCondition(enemy);
            if(result == 1){
                defeatedEnemy = 1;
                break;
            }else if(result == -1){
                isPlayerDie = 1;
                break;
            }
        }
    } while (isPlayerDie != 1);
    if(user.score > 1000){
        printf("Hmmm, not bad than i thought!\n");
    }else if(user.score > 2000){
        printf("Wow, great job!\n");
    }else if(user.score > 3000){
        printf("OMG, YOU'RE SO SIGMA!\n");
    }
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

void printLogo()
{
    const char *LOGO = "   _____ ____________  ______ \n"
                       "  / ___//  _/ ____/  |/  /   |\n"
                       "  \\__ \\ / // / __/ /|_/ / /| |\n"
                       " ___/ // // /_/ / /  / / ___ |\n"
                       "/____/___/\\____/_/  /_/_/  |_|\n\n";

    printf(LOGO);
}
void menu()
{
    puts("(P)lay game\n"
         "(L)eaderboard\n"
         "(Q)uit game");
}
void clear_screen() {
    printf(CLEAR_SCREEN_REGEX);
}

int main()
{
    char confirmation;
    printLogo();

    do
    {
        menu();
        printf("Enter your choice: ");
        scanf(" %c", &confirmation); getchar();
        switch (tolower(confirmation)) {
            case 'p':
                // Person player;
                return 0;
                //save_score(player);
                break;
            case 'l':
                leaderboard();
                puts("Press any key to continue...");
                getchar();
                break;
            case 'q':
                puts("Thanks for playing");
                return 0;
            default:
                puts("Invalid choice!");
        }
    } while (confirmation != 'q');
    return 0;
}
