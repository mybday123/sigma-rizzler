// SIGMA RIZZLER

/*

 Progressnya sejauh ini :
    Kami sudah membuat gameplay sederhana dari game Sigma Rizzler yang akan kami tambahkan lagi beberapa mekanisme
    agar game yang dibuat menjadi lebih menarik, seperti chatbox yang interaktif dan juga fitur fitur menarik lainnya

 */

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#define SLEEP(ms) usleep((ms) * 1000)
#endif

#define CLEAR_SCREEN_REGEX "\e[1;1H\e[2J"

enum Difficulty
{
    easy,
    normal,
    hard
};

enum Difficulty diff = easy;
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
    int isVulnerable;
    char diffculty[20];
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
    int isVulnerable;
} Enemy;

Person *user;
Enemy *enemy;
// free the memory
void freeMemory()
{
    free(user);
    free(enemy);
}

Enemy enemies[5] = {
    {"Ambatron", 100, 100, 500, 0, 0},
    {"Rusdi", 120, 120, 1000, 0, 0},
    {"Ambatukers", 140, 140, 1500, 0, 0},
    {"Ngamutron", 160, 160, 2000, 0, 0},
    {"Gambatron", 180, 180, 2500, 0, 0}};

void clearScreen()
{
    printf(CLEAR_SCREEN_REGEX);
}

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
        int notice = (rand() % 11);
        if (notice <= 3)
        {
            if (diff == easy || diff == normal)
            {
                printf("Enemy is vulnurable! Bring in the Damage!\n");
                enemy->isVulnerable = 1;
                return;
            }
            if (diff == hard)
            {
                if (notice <= 1)
                {
                    printf("Enemy is vulnurable! Bring in the Damage!\n");
                    enemy->isVulnerable = 1;
                }
                else
                {
                    printf("You are vulnurable! Bring in the Damage!\n");
                    user->isVulnerable = 1;
                }
            }
        }
        else if (notice > 3 && notice <= 6)
        {
            if (diff == hard)
            {
                if (notice == 6)
                {
                    printf("The enemy has been blessed by Goddess of Abundance! Healing now has improved for this round!\n");
                    enemy->abundant = 1;
                    return;
                }
            }
            if (notice <= 5)
            {
                printf("Goddess of Abundance have blessed you! Healing now has improved for this round!\n");
                user->abundant = 1;
            }
        }
        else if (notice >= 7 && notice <= 9)
        {
            if (notice <= 8)
            {
                printf("The enemy have been afflicted by poison! They will take damage overtime for 3 rounds\n");
                enemy->poison = 1;
                enemy->poison_counter = 2;
            }
            if (diff == hard || diff == normal)
            {
                if (notice > 8)
                {
                    printf("You have been afflicted by poison! You will take damage overtime for 3 rounds\n");
                    user->poison = 1;
                    user->poison_counter = 2;
                }
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
    if (enemy->isVulnerable == 1 && turn == 1 && round >= 3)
    {
        buff_atk = buff() + buff_atk;
        buff_atk += gachaVulnerability();
        enemy->isVulnerable = 0;
    }
    if (user->isVulnerable == 1 && turn == 2 && round >= 3)
    {
        buff_atk = buff() + buff_atk;
        buff_atk += gachaVulnerability();
        user->isVulnerable = 0;
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
        if (enemy->abundant == 1)
        {
            heal += gachaBuffHeal();
            if (enemy->poison == 1)
            {
                enemy->poison = 0;
                printf("Enemy's poison has been cured!\n");
            }
        }
        if (enemy->health + heal > enemy->max_health)
        {
            enemy->health = enemy->max_health;
        }
        else
        {
            enemy->health += heal;
        }
        enemy->abundant = 0;
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
        user->abundant = 0;
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
        if (diff == easy)
        {
            user->score += enemy->scoreObtained * 0.5;
        }
        else if (diff == normal)
        {
            user->score += enemy->scoreObtained * 0.7;
        }
        else if (diff == hard)
        {
            user->score += enemy->scoreObtained;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}
void quitGame()
{
    clearScreen();
    const char *quitText =
        " /$$$$$$$$ /$$   /$$ /$$   /$$       /$$$$$$$  /$$$$$$$$ /$$     /$$  /$$$$$$  /$$   /$$  /$$$$$$  /$$$$$$$$\n"
        "|__  $$__/| $$  | $$| $$  / $$      | $$__  $$| $$_____/|  $$   /$$/ /$$__  $$| $$$ | $$ /$$__  $$| $$_____/\n"
        "   | $$   | $$  | $$|  $$/ $$/      | $$  \\ $$| $$       \\  $$ /$$/ | $$  \\ $$| $$$$| $$| $$  \\__/| $$      \n"
        "   | $$   | $$$$$$$$ \\  $$$$/       | $$$$$$$ | $$$$$     \\  $$$$/  | $$  | $$| $$ $$ $$| $$      | $$$$$   \n"
        "   | $$   | $$__  $$  >$$  $$       | $$__  $$| $$__/      \\  $$/   | $$  | $$| $$  $$$$| $$      | $$__/   \n"
        "   | $$   | $$  | $$ /$$/\\  $$      | $$  \\ $$| $$          | $$    | $$  | $$| $$\\  $$$| $$    $$| $$      \n"
        "   | $$   | $$  | $$| $$  \\ $$      | $$$$$$$/| $$$$$$$$    | $$    |  $$$$$$/| $$ \\  $$|  $$$$$$/| $$$$$$$$\n"
        "   |__/   |__/  |__/|__/  |__/      |_______/ |________/    |__/     \\______/ |__/  \\__/ \\______/ |________/\n";
    printf(quitText);
    puts("\nPress Enter to exit.");
    getchar();
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
        printf("        *****        \n");
        printf("     *         *     \n");
        printf("   *    O   O    *   \n");
        printf("  *      \\_/      *  \n");
        printf("   *             *   \n");
        printf("     *         *     \n");
        printf("        *****        \n\n");
    }
    else if (user->health > 50 && user->health <= 75)
    {
        printf("        *****        \n");
        printf("     *         *     \n");
        printf("   *    O   O    *   \n");
        printf("  *      ---      * \n");
        printf("   *             *   \n");
        printf("     *         *     \n");
        printf("        *****        \n\n");
    }
    else if (user->health > 0 && user->health <= 50)
    {
        printf("        *****        \n");
        printf("     *         *     \n");
        printf("   *    O   O    *   \n");
        printf("  *      ---      * \n");
        printf("   *      / \\     * \n");
        printf("     *         *     \n");
        printf("        *****        \n\n");
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
    float enemyHealthHalf = enemy->health * 0.6;
    if ((float)enemy->health < enemyHealthHalf)
    {
        if (random > 5)
        {
            amount = attack(round);
            printf("The bot rizzed you with %d gyatt damage\n", amount);
        }
        else
        {
            amount = heal(round);
            printf("The rizzler use healing from Ohio and regen %d HP\n", amount);
        }
    }
    else
    {
        amount = attack(round);
        printf("The rizzler rizzed you with %d GYATT damage\n", amount);
    }
}
void save_score(Person player)
{
    FILE *fp = fopen("scoreboard.save", "a");
    if (diff == easy)
        strcpy(player.diffculty, "NPC");
    if (diff == normal)
        strcpy(player.diffculty, "SIGMA");
    if (diff == hard)
        strcpy(player.diffculty, "GIGA-CHAD");
    fprintf(fp, "%s#%d#%s\n", player.username, player.score, player.diffculty);
    fclose(fp);
}

int stop = 0;
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
             "(S)kip\n"
             "(G)ive up");
        scanf("%c", &decision);
        getchar();
        if (!strchr("hasgHASG", decision))
        {
            printf("Ermmm, What the Sigma?\n");
        }
    } while (!strchr("hasgHASG", decision));
    int amount = 0;
    switch (tolower(decision))
    {
    case 'a':
        amount = attack(round);
        printf("You rizz the rizzler and do %d GYATT damage\n", amount);
        break;
    case 'h':
        amount = heal(round);
        printf("You got back up from the Kai Cenat and heal %d HP\n", amount);
        break;
    case 's':
        turn = 0;
        break;
    case 'g':
        stop = 1;
        user->health = 0;
        if (user->score == 0)
        {
            printf("\"-69696969 aura\" ahh moment\n");
        }
        else
        {
            clearScreen();
            SLEEP(1000);
            printf("You use Skibidi Toilet to ran away with %d aura\n", user->score);
            SLEEP(1000);
            if (user->score >= 1000 && user->score < 2000)
            {
                printf("Hmmm, not bad than i thought!\n");
            }
            else if (user->score >= 2000 && user->score < 3000)
            {
                printf("Wow, great job!\n");
            }
            else if (user->score >= 3000)
            {
                printf("OMG, YOU'RE SO SIGMA!\n");
            }
            SLEEP(1000);
            printf("Saving your aura in the leaderboard...\n");
            if (user->score > 0)
                save_score(*user);
            SLEEP(1000);
            printf("Press any key to continue...");
            getchar();
        }
        break;
    }
}

void deathText()
{
    const char *DEATHLOGO =
        "__     __   ____    _    _     _____    _____   ______   _____\n"
        "\\ \\   / /  / __ \\  | |  | |   |  __ \\  |_   _| |  ____| |  __ \\\n"
        " \\ \\_/ /  | |  | | | |  | |   | |  | |   | |   | |__    | |  | |\n"
        "  \\   /   | |  | | | |  | |   | |  | |   | |   |  __|   | |  | |\n"
        "   | |    | |__| | | |__| |   | |__| |  _| |_  | |____  | |__| |\n"
        "   |_|     \\____/   \\____/    |_____/  |_____| |______| |_____/\n\n";

    clearScreen();
    printf(DEATHLOGO);
    SLEEP(1000);
    printf("Not so Skibidi\n");
    SLEEP(1000);
    printf("Saving your aura in the leaderboard.....\n");
    SLEEP(1000);
    printf("Press any key to continue...");
    getchar();
}
void resetPlayer()
{
    user->abundant = 0;
    user->poison = 0;
    user->poison_counter = 0;
    user->isVulnerable = 0;
    user->health = 100;
}

void playGame()
{
    int isPlayerDie = 0;
    user->score = 0;
    getPlayerName();
    int difficultValidated = 0;
    do
    {
        char difficult;
        clearScreen();
        printf("Information of each difficulty :\n");
        printf("NPC. Damn, be chill like an NPC bruh\n");
        printf("SIGMA. Wow, so you're a gentle SIGMA\n");
        printf("GIGA-CHAD. You're the grind king, lifting the world on your back! 🏋️‍♂️\n\n");
        printf("Choose the difficulty\n");
        printf("(N)PC\n");
        printf("(S)IGMA\n");
        printf("(G)IGA-CHAD\n");
        printf("Your choice : ");
        scanf("%c", &difficult);
        getchar();
        switch (tolower(difficult))
        {
        case 'n':
            diff = easy;
            break;
        case 's':
            diff = normal;
            break;
        case 'g':
            diff = hard;
            break;
        default:
            printf("Invalid choice! Please select a valid difficulty.\n");
            continue;
        }
        difficultValidated = 1;

    } while (difficultValidated == 0);

    do
    {
        int defeatedEnemy = 0;
        generateEnemy();
        int round = 1;
        resetPlayer();
        turn = 1;
        while (defeatedEnemy != 1)
        {
            SLEEP(1000 * 2);
            clearScreen();
            if (round % 3 == 0)
                takeTurn(round);
            applyPoison();
            printf("Enemy %s\n", enemy->name);
            printf("Enemy's health : %d\n", enemy->health);
            printf("\n-----------------------------------------------------------------------------------------------------\n\n");
            playerTurn(enemy, round);
            SLEEP(1000 * 2);
            int result = checkCondition();
            // Checking
            if (result == 1)
            {
                defeatedEnemy = 1;
                if (defeatedEnemy == 1)
                {
                    printf("Your rizz was approved by %s and it gives you %d aura\n", enemy->name, enemy->scoreObtained);
                }
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
                if (stop == 0)
                {
                    deathText();
                }

                if (user->score > 0)
                    save_score(*user);
                break;
            }
            // Checking
            round++;
        }
    } while (isPlayerDie != 1);
}

int compare(const void *a, const void *b)
{
    return ((Person *)b)->score - ((Person *)a)->score;
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

    puts("==================================================");
    puts("|                 Scoreboard                    |");
    puts("==================================================");
    puts("|   Username   |    Score   |     Difficulty    |");
    puts("==================================================");
    while (fscanf(fp, "%[^#]#%d#%s\n", player[count].username, &player[count].score, player[count].diffculty) != EOF)
    {
        count++;
    }
    qsort(player, count, sizeof(Person), compare);
    for (int i = 0; i < count; i++)
    {
        printf("|%-14.14s|%12d|%19s|\n", player[i].username, player[i].score, player[i].diffculty);
    }
    puts("===================================================");
    fclose(fp);
}

void difficultyInformation()
{
    clearScreen();
    printf("Difficulty of :\n");
    printf("1. NPC\n");
    printf("- You will only get 50%% of the score from the enemy of this difficulty\n");
    printf("- The enemy will have so many debuff but no buff for the enemy\n");
    printf("- Enemy's attack will be decreased by 10-20%%\n");
    printf("- Enemy's heal will be decreased by 10-20%%\n");
    printf("2. SIGMA\n");
    printf("- You will only get 70%% of the score from the enemy of this difficulty\n");
    printf("- You will have more chance to be afflicted by poison for 2 turns\n");
    printf("- Enemy's attack will be normal\n");
    printf("- Enemy's heal will be normal\n");
    printf("\n");
    printf("3. GIGA-CHAD\n");
    printf("- You will only get 100%% of the score from the enemy of this difficulty\n");
    printf("- You will have more chance to be afflicted by poison for 2 turns\n");
    printf("- You will have more chance to be vulnerable for 1 round\n");
    printf("- Enemy's attack will be increased by 10%%\n");
    printf("- Enemy's heal will be increased by 10%%\n");
    printf("\n");
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
         "(D)ifficulty Information\n"
         "(Q)uit game");
}

int main()
{
    user = (Person *)malloc(sizeof(Person));
    enemy = (Enemy *)malloc(sizeof(Enemy));
    char confirmation;

    do
    {
        clearScreen();
        printLogo();
        menu();
        printf("Enter your choice: ");
        scanf(" %c", &confirmation);
        getchar();
        switch (tolower(confirmation))
        {
        case 'p':
            playGame();
            stop = 0;
            break;
        case 'l':
            clearScreen();
            leaderboard();
            puts("Press any key to continue...");
            getchar();
            break;
        case 'd':
            difficultyInformation();
            puts("Press any key to continue...");
            getchar();
            break;
        case 'q':
            quitGame();
            return 0;
        default:
            puts("Invalid choice!");
        }
    } while (confirmation != 'q');
    // free memory
    freeMemory();
    return 0;
}