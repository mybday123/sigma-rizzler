/*
 * SIGMA RIZZLER
 * 
 * A simple turn-based game about Sigma and Rizz
 */

#include <stdio.h>
#include <ctype.h>

void printAuthentication()
{

}

void authRegister()
{

}
void printLogo()
{
    const char* LOGO = "   _____ ____________  ______ \n"
                       "  / ___//  _/ ____/  |/  /   |\n"
                       "  \\__ \\ / // / __/ /|_/ / /| |\n"
                       " ___/ // // /_/ / /  / / ___ |\n"
                       "/____/___/\\____/_/  /_/_/  |_|\n\n";

    printf(LOGO);
}
void menu() {
    puts("(P)lay game\n"
         "(L)eaderboard\n"
         "(Q)uit game");
}


int main()
{   
    char confirmation;
    printLogo();
    menu();
    printf("Enter your choice: ");
    scanf(" %c", &confirmation);
    switch (tolower(confirmation)) {
        case 'p':    /*Play*/
            break;
        case 'l':    /*Leaderbord*/
            break;
        case 'q':    /*Quit*/
            return 0;
        default:
            puts("Invalid choice");
    }
    return 0;
}