/*
 * SIGMA RIZZLER
 *
 * A simple turn-based game about Sigma and Rizz
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void attack(int *hp){
    srand(time(NULL));
    int random = (rand() % 16) + 5;
    int atk = random;
    *hp -= atk;
}

void heal (int *hp){
    srand(time(NULL));
    int random = (rand() % 9) + 5;
    int heal = random;
    *hp += heal;
}


void printAuthentication()
{

}

void authRegister()
{

}
void printLogo()
{
    const char* LOGO = "________  ______  ________  ___        ___       ___      \n"
                       "__          __    __        ___ __  __ ___     __   __    \n"
                       "________    __    __   ___  ___  _  _  ___    __ ___ __   \n"
                       "      __    __    __    __  ___   __   ___   __       __  \n"
                       "________  ______  ________  ___        ___  __         __ \n";

    printf(LOGO);
}

int main()
{
    printLogo();
        // do{
        


        // }while();
    return 0;
}
