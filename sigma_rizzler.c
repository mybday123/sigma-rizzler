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
} Enemy;

//////////////////////////////////////////////

void generateEnemy()
{
}

void playerAttack(){

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
        switch (rejectionLevel)
        {
        case 1:
            printf("Hey, you're so skibidi, can i get your name ?\n");
            break;
        case 2:
            printf("Hey, you're more skibidii than i think, so i need your name!\n");
            break;
        case 3:
            printf("HEYY, I NEED YOUR NAME PLEASE ??!?!\n");
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
        char temp[10];
        scanf("%[^\n]", temp);
        getchar();
        if (strlen(temp) <= 10)
        {
            isNameFilled = 1;
            strcpy(user.username, temp);
        }
    } while (isNameFilled == 0);
}

void playGame()
{
    int isPlayerDie = 0;

    user.health = 100;
    getPlayerName();
    printf("%s", user.username);
    // do
    // {
    //     playerAttack();
    // } while (isPlayerDie != 1);
}

void attack(int *hp)
{
    srand(time(NULL));
    int random = (rand() % 16) + 5;
    int atk = random;
    *hp -= atk;
}

void heal(int *hp)
{
    srand(time(NULL));
    int random = (rand() % 9) + 5;
    int heal = random;
    *hp += heal;
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

int main()
{
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
        case 'p': /*Play*/
            playGame();
            break;
        case 'l': /*Leaderbord*/
            break;
        case 'q': /*Quit*/
            return 0;
        default:
            puts("Invalid choice");
        }
    } while (confirmation != 'q');
     return 0;
}
