// SIGMA RIZZLER

/*
 *   Problem saat ini :
 *   - Saat healing harus atur agar tidak bentrok
 *   - Saat attack harus atur juga agar tidak bentrok buffnya dengan user
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#elif __linux__
#include <unistd.h>
#define SLEEP(ms) sleep((ms) / 1000)
#endif
#define CLEAR_SCREEN_REGEX "\e[1;1H\e[2J"

int turn = 1;

// Struct of person and enemy section
typedef struct
{
    char username[100];
    int health;
    int score;
    int poison;
    int abundant;
    int poison_counter;
    int inflictVulnerable;
} Person;

typedef struct
{
    char name[100];
    int max_health;
    int health;
    int scoreObtained;
    int poison;
    int poison_counter;
} Enemy;

Person *user;
Enemy *enemy;

Enemy enemies[5] =
    {
        {"Ambatron", 100, 100, 500, 0, 0},
        {"Rusdi", 120, 120, 1000, 0, 0},
        {"Ambatukers", 140, 140, 1500, 0, 0},
        {"Ngamutron", 160, 160, 2000, 0, 0},
        {"Gambatron", 180, 180, 2500, 0, 0}};

void generateEnemy()
{
    srand(time(NULL));
    Enemy newEnemy = enemies[(rand() % 5)];
    newEnemy.health = newEnemy.max_health;
    newEnemy.poison = 0;
    newEnemy.poison_counter = 0;
    *enemy = newEnemy;
}

int gacha()
{
    srand(time(NULL));

    int random = (rand() % 10) + 1;
    return random;
}
int gachaVulnerability()
{
    srand(time(NULL));

    int random = (rand() % 7) + 4;
    return random;
}

int gachaPoison()
{
    srand(time(NULL));

    int random = (rand() % 7) + 2;
    return random;
}

int buff()
{
    int random = gacha();
    if (random >= 5)
    {
        srand(time(NULL));
        int buff = (rand() % 6) + 5;

        printf("The arrow of the hunt has assisted you with a buff of %d\n", buff);
        return buff;
    }
    else
    {
        return 0;
    }
}
void pregameAnnouncement(int round)
{
    if (round % 3 == 0)
    {
        printf("Both buff and debuffs for you and the enemy are active! Push on with care!\n");
        srand(time(NULL));
        int notice = (rand() % 10);
        if (notice <= 3)
        {
            printf("Enemy is vulnurable for this round! Bring in the Damage!\n");
            user->inflictVulnerable = 1;
        }
        else if (notice > 3 && notice <= 6)
        {
            printf("Goddess of Abundance have blessed you! Healing now has improved for this round!\n");
            user->abundant = 1;
        }
        else if (notice >= 7 && notice <= 9)
        {
            if (notice == 8)
            {
                printf("The enemy have been afflicted by poison! They will take damage overtime for 3 rounds\n");
                enemy->poison = 1;
                enemy->poison_counter = 2;
            }
            else if (notice == 9)
            {
                printf("You have been afflicted by poison! You will take damage overtime for 3 rounds\n");
                user->poison = 1;
                user->poison_counter = 2;
            }
        }
    }
}

// Grouped by poison
void handlePoison(int *hp, int *poison_counter, int *poison)
{
    int poison_damage = gachaPoison();
    *hp -= poison_damage;
    printf("Poison damage of %d applied! Remaining poison rounds: %d\n", poison_damage, *poison_counter);
    (*poison_counter)--;
    if (*poison_counter == 0)
    {
        *poison = 0;
    }
}

void applyPoison()
{
    if (user->poison > 0 && user->poison == 1)
        handlePoison(&user->health, &user->poison_counter, &user->poison);
    else if (enemy->poison > 0 && enemy->poison == 1)
        handlePoison(&enemy->health, &enemy->poison_counter, &enemy->poison);
}
// Grouped by poison

int attack(int round)
{
    srand(time(NULL));

    int random = (rand() % 16) + 5;
    int atk = random;
    int buff_atk = atk;
    if (user->inflictVulnerable == 1 && turn == 1 && round >= 3)
    {
        buff_atk = buff() + buff_atk;
        buff_atk += gachaVulnerability();
        user->inflictVulnerable = 0;
    }
    if (turn == 0)
    {
        user->health -= buff_atk;
        turn = 1;
    }
    else
    {
        enemy->health -= buff_atk;
        turn = 0;
    }
    return buff_atk;
}

int gachaBuffHeal()
{
    srand(time(NULL));
    int random = (rand() % 11) + 10;
    return random;
}

int heal()
{
    srand(time(NULL));
    int random = (rand() % 9) + 5;
    int heal = random;

    if (turn == 0)
    {
        if (enemy->health + heal > enemy->max_health)
        {
            enemy->health = enemy->max_health;
        }
        else
        {
            enemy->health += heal;
        }
        turn = 1;
        return heal;
    }

    if (user->abundant == 1)
    {
        heal += gachaBuffHeal();
        if (user->poison == 1)
        {
            user->poison = 0;
            printf("Poison has been cured!\n");
        }
    }

    if (user->health + heal > 100)
    {
        user->health = 100;
    }
    else
    {
        user->health += heal;
    }
    turn = 0;
    return heal;
}
void takeTurn(int round)
{
    pregameAnnouncement(round);
}

int checkCondition()
{
    if (user->health <= 0)
    {
        return -1;
    }
    else if (enemy->health <= 0)
    {
        user->score += enemy->scoreObtained;
        return 1;
    }
    else
    {
        return 0;
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
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
        if (strlen(temp) <= 10)
        {
            isNameFilled = 1;
            strcpy(user->username, temp);
        }
    } while (isNameFilled == 0);
}

void print_user_condition()
{
    if (user->health > 75)
    {
        printf("        ***        \n");
        printf("     *         *     \n");
        printf("   *    O   O    *   \n");
        printf("  *      \\_/      *  \n");
        printf("   *             *   \n");
        printf("     *         *     \n");
        printf("        ***        \n\n");
    }
    else if (user->health > 50 && user->health <= 75)
    {
        printf("        ***        \n");
        printf("     *         *     \n");
        printf("   *    O   O    *   \n");
        printf("  *      ---      * \n");
        printf("   *             *   \n");
        printf("     *         *     \n");
        printf("        ***        \n\n");
    }
    else if (user->health > 0 && user->health <= 50)
    {
        printf("        ***        \n");
        printf("     *         *     \n");
        printf("   *    O   O    *   \n");
        printf("  *       ---      * \n");
        printf("   *      / \\     * \n");
        printf("     *         *     \n");
        printf("        ***        \n\n");
    }
    else
    {
        printf("        ***        \n");
        printf("     *         *     \n");
        printf("   *    X   X    *   \n");
        printf("  *      ---      * \n");
        printf("   *             *   \n");
        printf("     *         *     \n");
        printf("        ***        \n");
    }
}

void enemyTurn(int round)
{
    srand(time(NULL));
    int random = (rand() % 9);
    int amount = 0;
    if (random > 5)
    {
        amount = attack(round);
        printf("The bot attacked you with %d damage\n", amount);
    }
    else
    {
        amount = heal(round);
        printf("The bot healed himself with %d HP\n", amount);
    }
}

void playerTurn(Enemy *enemy, int round)
{
    char decision = ' ';
    print_user_condition();
    printf("Player \n");
    printf("Name : %s\n", user->username);
    printf("Health : %d\n", user->health);
    printf("Score : %d\n", user->score);
    do
    {
        puts("(H)eal\n"
             "(A)ttack\n"
             "(S)kip ");
        scanf("%c", &decision);
        getchar();
    } while (tolower(decision) != 'a' && tolower(decision) != 'h' && tolower(decision) != 's');
    int amount = 0;
    switch (tolower(decision))
    {
    case 'a':
        amount = attack(round);
        printf("You attacked the enemy with %d damage\n", amount);
        break;
    case 'h':
        amount = heal(round);
        printf("You healed yourself with %d HP\n", amount);
        break;
    case 's':
        break;
    }
}

void resetPlayer()
{
    user->abundant = 0;
    user->poison = 0;
    user->poison_counter = 0;
    user->inflictVulnerable = 0;
    user->health = 100;
}

void playGame()
{
    int isPlayerDie = 0;
    user->score = 0;
    getPlayerName();
    do
    {
        int defeatedEnemy = 0;
        generateEnemy();
        int round = 1;
        resetPlayer();
        turn = 1;
        while (defeatedEnemy != 1)
        {
            if (round % 3 == 0)
                takeTurn(round);
            applyPoison();
            printf("Enemy's health : %d\n", enemy->health);
            playerTurn(enemy, round);
            SLEEP(1000 * 2);
            int result = checkCondition();
            // Checking
            if (result == 1)
            {
                defeatedEnemy = 1;
                break;
            }
            else if (result == -1)
            {
                isPlayerDie = 1;
                break;
            }
            // Checking

            enemyTurn(round);
            SLEEP(1000 * 2);
            result = checkCondition();

            // Checking
            if (result == 1)
            {
                defeatedEnemy = 1;
                break;
            }
            else if (result == -1)
            {
                isPlayerDie = 1;
                break;
            }
            // Checking
            round++;
        }
    } while (isPlayerDie != 1);
    if (user->score > 1000)
    {
        printf("Hmmm, not bad than i thought!\n");
    }
    else if (user->score > 2000)
    {
        printf("Wow, great job!\n");
    }
    else if (user->score > 3000)
    {
        printf("OMG, YOU'RE SO SIGMA!\n");
    }
}

int compare(const void *a, const void *b)
{
    return ((Person *)b)->score - ((Person *)a)->score;
}

void save_score(Person player)
{
    FILE *fp = fopen("scoreboard.txt", "a");
    fprintf(fp, "%s#%d\n", player.username, player.score);
    fclose(fp);
}

void leaderboard()
{
    FILE *fp = fopen("scoreboard.save", "r");
    if (fp == NULL)
    {
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
    while (fscanf(fp, "%[^#]#%d\n", player[count].username, &player[count].score) != EOF)
    {
        count++;
    }
    qsort(player, count, sizeof(Person), compare);
    for (int i = 0; i < count; i++)
    {
        printf("|%-14.14s|%12d|\n", player[i].username, player[i].score);
    }
    puts("=============================");
    fclose(fp);
}

void printLogo()
{
    const char *LOGO = "   __ _____  __ \n"
                       "  / _//  / ___/  |/  /   |\n"
                       "  \\_ \\ / // / _/ /|_/ / /| |\n"
                       " _/ // // // / /  / / __ |\n"
                       "/_/_/\\_//  ///  ||\n\n";

    printf(LOGO);
}

void menu()
{
    puts("(P)lay game\n"
         "(L)eaderboard\n"
         "(Q)uit game");
}

void clearScreen()
{
    printf(CLEAR_SCREEN_REGEX);
}

int main()
{
    user = (Person *)malloc(sizeof(Person));
    enemy = (Enemy *)malloc(sizeof(Enemy));
    char confirmation;
    printLogo();

    do
    {
        menu();
        printf("Enter your choice: ");
        scanf(" %c", &confirmation);
        getchar();
        switch (tolower(confirmation))
        {
        case 'p':
            playGame();
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