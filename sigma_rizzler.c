// SIGMA RIZZLER

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#define SLEEP(ms) usleep((ms) * 1000)
#endif

#define UP 72
#define DOWN 80
#define ENTER 13
#define LEFT 75
#define RIGHT 77
#define ESC 27

#define PAGE_SIZE 10

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
    int isVulnerable;
} Enemy;

Person *user;
Enemy *enemy;

int modeStory = 0;
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

 
 int menuSize;
 int stop = 0;
int key(){
    int c = getch();
    if (c == UP) return 1;
    else if(c == DOWN) return 2;
    else if(c == ENTER) return 3;
    else return 0;
}

void arrowMenu(const char *string[], int *menuSize , int select){
    for(int i = 0; i < *menuSize; i++){
        if(i+1 == select){
            printf("-> %s", string[i]);
        }
        else{
            printf("   %s", string[i]);
        }
    }
}


void printChar(const char *s, int ms_t)
{
    int stringLen = strlen(s);
    for (int i = 0; i < stringLen; i++)
    {
        putchar(s[i]);
        fflush(stdout); // Make sure it shows immediately
        SLEEP(ms_t);
    }
}

void clearScreen()
{
    printf(CLEAR_SCREEN_REGEX);
}

void generateEnemy(int levelEnemy)
{
    srand(time(NULL));
    Enemy newEnemy;
    if (levelEnemy != -1)
    {
        newEnemy = enemies[levelEnemy];
    }
    else
    {
        newEnemy = enemies[(rand() % 5)];
    }
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

        printChar("The arrow of the hunt has assisted you with a buff of ", 20);
        printf("%d\n", buff);
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
        printChar("Both buff and debuffs for you and the enemy are active! Push on with care!\n", 20);
        srand(time(NULL));
        int notice = (rand() % 11);
        if (notice <= 3)
        {
            if (diff == easy || diff == normal)
            {
                printChar("Enemy is vulnurable! Bring in the Damage!\n", 20);
                enemy->isVulnerable = 1;
                return;
            }
            if (diff == hard)
            {
                if (notice <= 1)
                {
                    printChar("Enemy is vulnurable! Bring in the Damage!\n", 20);
                    enemy->isVulnerable = 1;
                }
                else
                {
                    printChar("You are vulnurable! Bring in the Damage!\n", 20);
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
                    printChar("The enemy has been blessed by Goddess of Abundance! Healing now has improved for this round!\n",20);
                    enemy->abundant = 1;
                    return;
                }
            }
            if (notice <= 5)
            {
                printChar("Goddess of Abundance have blessed you! Healing now has improved for this round!\n",20);
                user->abundant = 1;
            }
        }
        else if (notice >= 7 && notice <= 10)
        {
            if (diff == easy)
            {
                if (notice <= 10)
                {
                    printChar("The enemy have been afflicted by poison! They will take damage overtime for 3 rounds\n", 20);
                    enemy->poison = 1;
                    enemy->poison_counter = 2;
                }
            }
            else
            {
                if (notice <= 7)
                {
                    printChar("The enemy have been afflicted by poison! They will take damage overtime for 3 rounds\n", 20);
                    enemy->poison = 1;
                    enemy->poison_counter = 2;
                }
            }
            if (diff == hard || diff == normal)
            {
                if (notice > 9)
                {
                    printChar("You have been afflicted by poison! You will take damage overtime for 3 rounds\n", 20);
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
    printChar("Poison damage of ", 20);
    printf("%d ", poison_damage); 
    printChar("has been applied! Remaining poison rounds: ", 20);
    printf("%d\n", *poison_counter);
    (*poison_counter)--;
    if (*poison_counter == 0)
    {
        *poison = 0;
    }
}

void applyPoison()
{
    if (user->poison > 0 && user->poison == 1){
        handlePoison(&user->health, &user->poison_counter, &user->poison);
        SLEEP(500);
    }
    else if (enemy->poison > 0 && enemy->poison == 1){
        handlePoison(&enemy->health, &enemy->poison_counter, &enemy->poison);
        SLEEP(500);
    }
}
// Grouped by poison

int attack(int round)
{
    srand(time(NULL));

    int random = (rand() % 12) + 10;
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
                printChar("Enemy's poison has been cured!\n", 20);
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
            printChar("Poison has been cured!\n", 20);
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
    SLEEP(500);
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
    printf("%s", quitText);
    puts("\nPress any key to continue.");
    getch();
}

void getPlayerName()
{
    char agreement;
    int rejectionLevel = 1;
    do
    {
        clearScreen();
        switch (rejectionLevel)
        {
        case 1:
            printChar("Hey, you're so skibidi, can i get your name ?\n", 50);
            break;
        case 2:
            printf("Hey, you're more skibidii than i think, so i need your name!\n", 50);
            break;
        case 3:
            printf("HEYY, I NEED YOUR NAME PLEASE \?\?!?!\n", 50);
            break;
        default:
            printf("To be honest, i really need your name, so please!!\n", 50);
            break;
        }
        printf("Your answer (y/n): ");
        agreement = tolower(getch());
        rejectionLevel++;
    } while (tolower(agreement) != 'y');

    int isNameFilled = 0;
    do
    {
        clearScreen();
        printChar("Add your username, skibidi soldier : ", 10);
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
            printChar("The bot rizzed you with ",20); printf("%d ", amount); printChar("GYATT damage\n", 20);
        }
        else
        {
            amount = heal();
            printChar("The rizzler use healing from Ohio and regen ",20); printf("%d ", amount); printChar("HP\n", 20);
        }
    }
    else
    {
        amount = attack(round);
        printChar("The rizzler rizzed you with ",20); printf("%d ", amount); printChar("GYATT damage\n", 20);
    }
}
void save_score(Person player)
{
    FILE *fp = fopen("scoreboard.save", "a");
    if (diff == easy)
        strcpy(player.difficulty, "NPC");
    if (diff == normal)
        strcpy(player.difficulty, "SIGMA");
    if (diff == hard)
        strcpy(player.difficulty, "GIGA-CHAD");
    fprintf(fp, "%s#%d#%s\n", player.username, player.score, player.difficulty);
    fclose(fp);
}


void playerTurn(Enemy *enemy, int round)
{
    int  t = 0;
    do{
         char decision = ' ';
    do
    {
        clearScreen();
        printf("Enemy %s\n", enemy->name);
        printf("Enemy's health : %d\n", enemy->health);
        printf("\n-----------------------------------------------------------------------------------------------------\n\n");
        print_user_condition();
        printf("Player \n");
        if(modeStory) strcpy(user->username, "Rusdi");
        printf("Name : %s\n", user->username);
        printf("Health : %d\n", user->health);
        if (!modeStory)
        {
            printf("Score : %d\n", user->score);
        }
            puts("(H)eal\n"
                "(A)ttack\n"
                "(S)kip\n"
                "(G)ive up");
            printf("Choice: ");
        decision = tolower(getch());
        if (!strchr("hasgHASG", decision))
        {
            puts("");
            printChar("Ermmm, What the Sigma?\n", 40);
        }
    } while (!strchr("hasgHASG", decision));
    int amount = 0;
        char yesNo;
    switch (tolower(decision))
    {
    case 'a':
        puts("");
        amount = attack(round);
       printChar("You rizz the rizzler and do ",20); printf("%d ", amount); printChar("GYATT damage\n", 20);
        break;
    case 'h':
        puts("");
        amount = heal();
        printChar("You got back up from the Kai Cenat and heal ",20); printf("%d ", amount); printChar("HP\n", 20);
        break;
    case 's':
        puts("");
        printChar("random poeple with w GYATT passing by and making you skip the turn\n", 20);
        turn = 0;
        break;
    case 'g':
        clearScreen();
        printChar("Are you sure?\n", 20);
        printf("y/n: ");
        do{
        yesNo = tolower(getch());
        if(yesNo == 'y'){
            if(modeStory){
                stop = 1;
                user->health = 0;
                puts("");
                printChar(
                    "Rusdi yang kesulitan bertarung memilih untuk kabur dari pertarungannya melawan\n"
                    "musuhnya, ia kembali ke rumahnya, depresi akan dunia yang sudah berubah\n" 
                    "menjadi robotron, termasuk orang-orang kesayangannya yang bahkan dirinya\n" 
                    "merasa  tidak mampu untuk mengalahkannya . tak kuasa mengalahkan gamba si\n" 
                    "musuh terakhir, dirinya yang sedang terlelap di kesedihan perlahan juga berubah\n" 
                    "menjadi robotron. Akhir Rusdi yang harusnya menyelamatkan dunia, malah\n"
                    "menjadi pengecut yang tidak berani mengalahkan musuhnya dan hancur olehnya\n" 
                    "menjadi hal yang harusnya dia kalahkan.\n"
                 , 50);
                 printf("Press any key to continue...");
                 getchar();
            }
            else{
                stop = 1;
            user->health = 0;
            if (user->score == 0 && !modeStory)
            {
                puts("");
                printChar("\"-69696969 aura\" ahh moment\n", 20);
            }
            else
            {
                clearScreen();
                SLEEP(1000);
                printChar("You use Skibidi Toilet to ran away with ", 20);
                printf("%d aura\n", user->score);
                SLEEP(1000);
                if (user->score >= 1000 && user->score < 2000)
                {
                    printChar("Hmmm, not bad than i thought!\n", 20);
                }
                else if (user->score >= 2000 && user->score < 3000)
                {
                    printChar("Wow, great job!\n", 20);
                }
                else if (user->score >= 3000)
                {
                    printChar("OMG, YOU'RE SO SIGMA!\n", 20);
                }
                SLEEP(1000);
                printChar("Saving your aura in the leaderboard...\n", 20);
                if (user->score > 0)
                    save_score(*user);
                SLEEP(1000);
                printf("Press any key to continue...");
                getchar();
            }
            }
            return;
        }
        else if(yesNo == 'n'){
            t = 1;
            break;
        }
        else continue;
        }while(tolower(getch()) != 'y');
        continue;
    }
        t = 0;
    }while(t == 1);
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
    if(modeStory){
        clearScreen();
        printf("%s", DEATHLOGO);
        printChar(
            "Tanpa diketahui olehnya, Kaki Rusdi tiba-tiba jatuh ke tanah, tak kuasa menahan\n" 
            "Luka yang ia terima do sekujur tubuhnya, dirinya Masih ingin bertarung,\n" 
            "menyelamatkan Dunia yang harusnya ia selamatkan, namun jatuh di hadapan\n" 
            "musuhnya. Rusdi mencoba untuk bangun tapi tidak bisa, sampai dirinya\n" 
            "menghembuskan nafas terakhirnya dihadapan musuhnya. Dirinya tidak pernah\n" 
            "menyerah sampai terakhir, kamu juga kan?\n"
        , 20);
        printf("Press any key to continue...");
        getchar();
    }
    else{
        clearScreen();
        printf("%s", DEATHLOGO);
        SLEEP(1000);
        printChar("Not so skibidi", 20);
        puts("");
        SLEEP(1000);
        printChar("Saving your aura in the leaderboard.....\n", 20);
        SLEEP(1000);
        printf("Press any key to continue...");
        getchar();
    }
}

void resetPlayer()
{
    user->abundant = 0;
    user->poison = 0;
    user->poison_counter = 0;
    user->isVulnerable = 0;
    user->health = 100;

}

const char *diffMenu[] = {
    "(N)PC\n",
    "(S)IGMA\n",
    "(G)IGA-CHAD\n",
    "(E)xit\n"
};

void playGame()
{
    int select = 1;
    int isPlayerDie = 0;
    user->score = 0;
    int t = 0;
    getPlayerName();
    do
    {
        clearScreen();
        printf("Information of each difficulty :\n");
        printf("NPC. Damn, be chill like an NPC bruh\n");
        printf("SIGMA. Wow, so you're a gentle SIGMA\n");
        printf("GIGA-CHAD. You're the grind king, lifting the world on your back! 🏋️‍♂️\n\n");
        printf("Choose the difficulty\n");
        menuSize = sizeof(diffMenu) / sizeof(diffMenu[0]);
        arrowMenu(diffMenu, &menuSize, select);
        switch(key()){
            case 1:
                select--;
                if (select < 1) select = menuSize;
                break;
            case 2:
                select++;
                if (select > menuSize) select = 1;
                break;
            case 3:
                switch(select){
                    case 1:
                        diff = easy;
                        break;
                    case 2:
                        diff = normal;
                        break;
                    case 3:
                        diff = hard;
                        break;
                    case 4:
                        return;
                    default:
                    break;
                }
                t=0;
                break;
            default:
                t = 1;
                break;
        }
    } while (t == 1);

    printChar("Entering a battle", 10);
    do
    {
        int defeatedEnemy = 0;
        generateEnemy(-1);
        int round = 1;
        resetPlayer();
        turn = 1;
        while (defeatedEnemy != 1)
        {
            SLEEP(1000);
            clearScreen();
            if (round % 3 == 0)
            takeTurn(round);
            applyPoison();
            playerTurn(enemy, round);
            SLEEP(1000 * 2);
            int result = checkCondition();
            // Checking
            if (result == 1)
            {
                defeatedEnemy = 1;
                if (defeatedEnemy == 1)
                {
                    printChar("Your rizz was approved by ", 20);
                    printf("%s ", enemy->name);
                    printChar("and it gives you ", 20); 
                    printf("%d ", enemy->scoreObtained); 
                    printChar("aura\n", 20);
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
                if (!stop)
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

const char *leaderboardMenu[] = {
    "Show all user\n",
    "Search username\n",
    "Exit\n"
};

int contains_ignore_case(const char *haystack, const char *needle) {
    size_t haystack_len = strlen(haystack);
    size_t needle_len = strlen(needle);

    if (needle_len == 0) {
        return 1; 
    }

    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        size_t j;
        for (j = 0; j < needle_len; j++) {
            if (tolower((unsigned char)haystack[i + j]) != tolower((unsigned char)needle[j])) {
                break;
            }
        }
        if (j == needle_len) {
            return 1; 
        }
    }
    return 0; 
}

void highlight_match(const char *username, const char *searchTerm) {
    size_t username_len = strlen(username);
    size_t searchTerm_len = strlen(searchTerm);

    int printed = 0;

    for (size_t i = 0; i < username_len; i++) {
        int is_match = 1;
        for (size_t j = 0; j < searchTerm_len && (i + j) < username_len; j++) {
            if (tolower((unsigned char)username[i + j]) != tolower((unsigned char)searchTerm[j])) {
                is_match = 0;
                break;
            }
        }

        if (is_match) {
            printf("\033[1;32m");
            printf("%.*s", (int)searchTerm_len, username + i);
            printf("\033[0m");   
            i += searchTerm_len - 1;
            printed += searchTerm_len;
        } else {
            printf("%c", username[i]);
            printed++;
        }
    }

    for (int i = printed; i < 14; i++) {
        printf(" ");
    }
}

void leaderboard() {
    int select = 1;
    int showAll = 0;
    int t = 0;
    FILE *fp = fopen("scoreboard.save", "r");
    if (fp == NULL) {
        clearScreen();
        printf("Leaderboard empty. No one has played yet :(\n");
        getchar();
        freopen("scoreboard.save", "w", stdout);
        fclose(fp);
        return;
    }

    Person player[500];
    int count = 0;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
    if (strlen(line) == 0 || line[0] == '\n') {
        continue;
    }

    if (sscanf(line, "%[^#]#%d#%s", player[count].username, &player[count].score, player[count].difficulty) == 3) {
        count++;
    }
}

    fclose(fp);
    qsort(player, count, sizeof(Person), compare);
    menuSize = sizeof(leaderboardMenu) / sizeof(leaderboardMenu[0]);
    do{
        clearScreen();
        arrowMenu(leaderboardMenu, &menuSize, select);
        switch(key()){
            case 1:
                select--;
                if (select < 1) select = menuSize;
                break;
            case 2:
                select++;
                if (select > menuSize) select = 1;
                break;
            case 3:
                switch(select){
                    case 1:
                        showAll = 1;
                        break;
                    case 2:
                        showAll = 0;
                        break;
                    case 3:
                        return;
                    default:
                    break;
                }
                t = 0;
                break;
            default:
            t = 1;
            break;
        }
    }while (t==1);

    
    if(showAll){
        int currentPage = 0;
        int totalPages = (count + PAGE_SIZE - 1) / PAGE_SIZE; 
        if (totalPages == 0) totalPages = 1;
        while (1) {
        clearScreen();
        if(count == 0){
        puts("=================================================");
        puts("|                 Scoreboard                    |");
        puts("=================================================");
        puts("|   Username   |    Score   |     Difficulty    |");
        puts("=================================================");
        puts("|               There is no Player              |");
        }
        else{
        puts("=================================================");
        puts("|                 Scoreboard                    |");
        puts("=================================================");
        puts("|   Username   |    Score   |     Difficulty    |");
        puts("=================================================");
        }
        

        int start = currentPage * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > count) {
            end = count;
        }

        for (int i = start; i < end; i++) {
            printf("|%-14.14s|%12d|%19s|\n", player[i].username, player[i].score, player[i].difficulty);
        }

        puts("=================================================");
        printf("Page %d/%d. Use Left/Right arrow keys to navigate. Press esc to quit.\n", currentPage + 1 , totalPages);

        char key = getch(); 
        if (key == ESC) { 
            break;
        } 
        else if (key == LEFT) { 
            currentPage--;
            if (currentPage < 1) currentPage = 0;
        }
        else if(key == RIGHT) {
            currentPage++;
            if (currentPage >= totalPages) currentPage = totalPages-1;
        }
        else continue;
    }
    }
    else{
        char searchTerm[50];
        printf("Enter username: ");
        scanf("%s", searchTerm);
        getchar(); 

        int found = 0;

        puts("");
        puts("=================================================");
        puts("|                 Scoreboard                    |");
        puts("=================================================");
        puts("|   Username   |    Score   |     Difficulty    |");
        puts("=================================================");

        
        for (int i = 0; i < count; i++) {
            if (contains_ignore_case(player[i].username, searchTerm)) {
                printf("|");
                highlight_match(player[i].username, searchTerm);
                printf("|%12d|%19s|\n", player[i].score, player[i].difficulty);
                found = 1;
            }
        }

        if (!found) {
            puts("|           Username not found                  |");
        }
        puts("=================================================");
        printf("Press any key to continue\n");
        getch();
}
}

void difficultyInformation()
{
    clearScreen();
    printf("Difficulty of :\n");
    printChar("1. NPC\n", 10);
    printf("- You will only get 50%% of the score from the enemy of this difficulty\n");
    printf("- The enemy will have so many debuff but no buff for the enemy\n");
    printf("- Enemy's attack will be decreased by 10-20%%\n");
    printf("- Enemy's heal will be decreased by 10-20%%\n");
    printChar("2. SIGMA\n", 10);
    printf("- You will only get 70%% of the score from the enemy of this difficulty\n");
    printf("- You will have more chance to be afflicted by poison for 2 turns\n");
    printf("- Enemy's attack will be normal\n");
    printf("- Enemy's heal will be normal\n");
    printf("\n");
    printChar("3. GIGA-CHAD\n", 10);
    printf("- You will only get 100%% of the score from the enemy of this difficulty\n");
    printf("- You will have more chance to be afflicted by poison for 2 turns\n");
    printf("- You will have more chance to be vulnerable for 1 round\n");
    printf("- Enemy's attack will be increased by 10%%\n");
    printf("- Enemy's heal will be increased by 10%%\n");
    printf("\n");
    printChar("Press any key to continue", 10);
}

void printLogo()
{
    const char *LOGO = "   _____ ____________  ______ \n"
                       "  / ___//  _/ ____/  |/  /   |\n"
                       "  \\__ \\ / // / __/ /|_/ / /| |\n"
                       " ___/ // // /_/ / /  / / ___ |\n"
                       "/____/___/\\____/_/  /_/_/  |_|\n\n";

    printf("%s", LOGO);
}


const char *menu[] = {
        "   (S)tory mode\n",
        "   (P)lay game\n",
        "   (L)eaderboard\n",
        "   (D)ifficulty Information\n",
        "   (Q)uit game\n"
};

const char *menu2[] = {
        "(S)tory mode\n",
        "(P)lay game\n",
        "(L)eaderboard\n",
        "(D)ifficulty Information\n",
        "(Q)uit game\n"
};



void storyMode()
{
    clearScreen();
    int isPlayerDie = 0;
    int defeatedEnemy = 0;
    int round = 1;
    turn = 1;
    printChar("Suatu hari, rusdi, seorang pejuang intergalaktik pergi untuk menemui temannya, mas ambatukers, yang juga seorang pejuang intergalaktik teman baik rusdi.\n", 80);
    getchar();
    printChar("“Hai amba! Cepat buka pintunya dasar pengoc-“", 80);
    getchar();
    clearScreen();
    printChar("Sebelum menyelesaikan apa yang ingin dia katakan, sebuah tembakan laser menembus pintunya hampir mengenai rusdi", 80);
    getchar();
    printChar("“Hey, kau gila ya?ˮ Rusdi langsung mendobrak buka pintunya, yang ia temui bukanlah amba yang ia kenal, namun sebuah robot imitasi si amba", 80);
    getchar();
    printChar(" “A- apa apaan… dimana amba yang kukenal? Kembalikan dia!ˮ", 80);
    getchar();
    clearScreen();
    printChar("Fight 1 with ambatron\n", 80);
    fflush(stdout);
    generateEnemy(0);
    resetPlayer();
    while (defeatedEnemy != 1)
    {
        SLEEP(1000);
        clearScreen();
        if (round % 3 == 0)
            takeTurn(round);
        applyPoison();
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
    if (isPlayerDie == 1)
    {
        if(!stop){
            deathText();
        }
        return;
    }
    clearScreen();
    printChar("Setelah pertarungan usai, dia mendapatkan bahwa orangˮ yang dikenali mas rusdi telah dikutuk menjadi robotron oleh gamba.\n", 80);
    getchar();
    printChar("“Sial! Guru ngamu pasti terkena kutukan bodoh ini juga!ˮ\n", 80);
    getchar();
    printChar("karena khawatir akan gurunya, Dirinya pun bergegas ke tempat gurunya, Mas ngamu.\n", 80);
    getchar();
    printChar("Setelah dirinya sampai, dia juga disambut Hal yang sama, gurunya menjadi sebuah robot yang dipanggil ngamutron.\n", 80);
    getchar();
    clearScreen();
    printChar("Fight 2 with ngamutron\n", 50);
    fflush(stdout);
    defeatedEnemy = 0;
    generateEnemy(3);
    round = 1;
    resetPlayer();
    turn = 1;
    while (defeatedEnemy != 1)
    {
        SLEEP(1000);
        clearScreen();
        if (round % 3 == 0)
            takeTurn(round);
        applyPoison();
        playerTurn(enemy, round);
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
        playerTurn(enemy, round);
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
    if (isPlayerDie == 1)
    {
        if(!stop){
            deathText();
        }
        return;
    }
    clearScreen();
    printChar("Gurunya pun juga terkapar di tanah pingsan, namun di samping badan gurunya itu, dia mendapatkan peta ke markas gamba. \n", 80);
    getchar();
    printChar("Dia Tanpa menghabiskan banyak waktu, dia pergi ke markas gamba. \n", 80);
    getchar();
    printChar("Sesampainya disana, dia mendengar suara gamba “Oh pahlawan rusdi Dari planet sigma, anda terlalu lambat,\n", 80);
    getchar();
    printChar("Dunia ini sudah kuubah menjadi robotron, kau takkan bisa menghentikanku.\n", 80);
    getchar();
    clearScreen();
    printChar("Bahkan diriku sendiri sudah kuubah menjadi robotron, bentuk makhluk hidup yang paling sempurna di alam semesta!\n", 80);
    getchar();
    printChar("Berterimakasihlah padaku Rusdi!ˮ\n", 80);
    getchar();
    printChar("Rusdi yang sangat marah pun langsung bertarung dengannya.ˮ\n", 80);
    getchar();
    clearScreen();
    printChar("Fight 3 with gambatron\n", 50);
    getchar();
    fflush(stdout);
    defeatedEnemy = 0;
    round = 1;
    turn = 1;
    generateEnemy(4);
    resetPlayer();
    while (defeatedEnemy != 1)
    {
        SLEEP(1000);
        clearScreen();
        if (round % 3 == 0)
            takeTurn(round);
        applyPoison();
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
    if (isPlayerDie == 1)
    {
        if(!stop){
            deathText();
        }
        return;
    }
    clearScreen();
    printChar("“Tidak!!!!!ˮ\n", 70);
    getchar();
    printChar("Gamba pun berteriak layaknya penjahat tahun 90-an saat kalah. \n", 70);
    getchar();
    printChar("Dan dengan kemenangan itu, semua makhluk hidup yang diubah menjadi robotron kembali menjadi normal, Dan untuk kesekian Kalinya Rusdi menjadi pahlawan kosmik terbaik di alam semesta\n", 100);
    getchar();
    fflush(stdout);
    clearScreen();
    printChar("Terimakasih telah memainkan story mode yang telah kami buat ini", 80);
}

int main()
{
    user = (Person *)malloc(sizeof(Person));
    enemy = (Enemy *)malloc(sizeof(Enemy));
    int t = 0;
    int select = 1;
    clearScreen();
     menuSize = sizeof(menu) / sizeof(menu[0]);
    printLogo();
    for(int i = 0; i<menuSize; i++){
        printChar(menu[i], 20);
    }
    do
    {   
        menuSize = sizeof(menu) / sizeof(menu[0]);
        clearScreen();
        printLogo();
        arrowMenu(menu2, &menuSize, select);
        switch(key()){
            case 1:
                select--;
                if (select < 1) select = menuSize;
                break;
            case 2:
                select++;
                if (select > menuSize) select = 1;
                break;
            case 3:
                switch(select){
                    case 1:
                        modeStory = 1;
                        storyMode();
                        modeStory = 0;
                        break;
                    case 2:
                        strcpy(user->username, " ");
                        playGame();
                        break;
                    case 3:
                        leaderboard();
                        break;
                    case 4:
                        difficultyInformation();
                        getch();
                        break;
                    case 5:
                        quitGame();
                        return 0;
                }
                break;
            case 0:
                continue;
            default:
                t = 1;
                break;
        }
    } while (t != 1);
    // free memory
    freeMemory();
    return 0;
}
