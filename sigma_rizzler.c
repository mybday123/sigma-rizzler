/*
 * SIGMA RIZZLER
 * 
 * A simple turn-based game about Sigma and Rizz
 */

#include <stdio.h>

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
                       "/____/___/\\____/_/  /_/_/  |_|\n";

    printf(LOGO);
}

int main()
{   
    printLogo();
    printf("\n(P)lay game\n"
           "(L)eaderboard\n"
           "(Q)uit game\n");
    return 0;
}